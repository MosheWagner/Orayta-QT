/* This program is free software; you can redistribute it and/or modify
* it under the terms of the GNU General Public License version 2
* as published by the Free Software Foundation.
*
* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
* GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License
* along with this program; if not, write to the Free Software
* Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307 USA
*
* Author: Moshe Wagner. <moshe.wagner@gmail.com>
*/

#include "mainwindow.h"
#include "bookdisplayer.h"
#include "bookviewinterface.h"
#include "bookview_orayta.h"

void MainWindow::on_bookmarkWidget_customContextMenuRequested(QPoint pos)
{
    QMenu menu(ui->bookmarkWidget);

    QAction *edit = new QAction(QIcon(":/Icons/edit.png"), tr("Edit bookmark title..."), &menu);
    QAction *del = new QAction(QIcon(":/Icons/edit-delete.png"), tr("Delete bookmark"), &menu);

    //Im to lazy to do this the right way...
    QObject::connect(edit, SIGNAL(triggered()), this , SLOT(on_bookmarkEdit_clicked()));
    QObject::connect(del, SIGNAL(triggered()), this , SLOT(on_removeBookmark_clicked()));

    menu.addAction(edit);
    menu.addAction(del);

    menu.setLayoutDirection(Qt::RightToLeft);

    //Open the menu to the left of the cursor's position
    QPoint point = QPoint(QCursor::pos().x() - menu.width(), QCursor::pos().y());
    menu.exec(point);
}

QString MainWindow::bookMarkTitle(QString lnk)
{
    //Format position for human display
    int p = lnk.indexOf(":");
    QString lnkdisplay = lnk.mid(p+1);
    lnkdisplay = unescapeFromBase64(lnkdisplay).replace("_", " ").replace("-", " ").replace("{", "").replace("}", "").replace(" 0", "");

    int id = -1;
    ToNum(lnk.mid(0,p), &id);

    //Find and add the book's name
    NodeBook* book = ui->treeWidget->findBookById(id);
    if ( !book )
    {
        qDebug() << "Invalid link!";
        return "";
    }
    else
    {
        lnkdisplay = book->getTreeDisplayName() + " " + lnkdisplay;
    }

    //Remove extra spaces that appear for some reason:
    lnkdisplay.replace(QRegExp("[ ][ ]*"), " ");

    return lnkdisplay;
}

//Opens the bookmark dialog
void MainWindow::bookMarkPosition(QString lnk)
{
    QString linkdisplay = bookMarkTitle(lnk);

    AddBookMark *t = new AddBookMark(this, lnk, linkdisplay);
    t->show();

    connect (t, SIGNAL(OK(QString,QString)), this, SLOT(addBookMark(QString,QString)));
}

//Adds the given book position to the bookmark list and file
// ########## conflit de noms ? (QPushButton dans l'ui)
void MainWindow::addBookMark(QString link, QString title)
{
    QString linkdisplay = bookMarkTitle(link);
    if ( linkdisplay == title) title = "";

    addBookMarkToList(link, title);

    //Write to bookmark file
    writetofile(USERPATH + "BookMarkList.txt", link + "|" + title + "\n", "UTF-8", false);
}


//Adds a bookmark to the bookmark list, (giving it's encoded link)
void MainWindow::addBookMarkToList(QString lnk, QString title)
{
    QListWidgetItem *item = new QListWidgetItem(ui->bookmarkWidget);

    QString linkdisplay = bookMarkTitle(lnk);

    if ( title == "" )
    {
        item->setText( linkdisplay);
        title = linkdisplay;
    }
    else
    {
        item->setText( title + " (" + linkdisplay + ")" );
    }

    //This is clearly not the right way to do this, but hey, it works.
    // ############### berk
    item->setData(Qt::StatusTipRole, lnk);
    item->setData(Qt::WhatsThisRole, title);
    item->setIcon(QIcon(":Icons/bookmarks.png"));
}

//Build the bookmark list from the bookmark file
void MainWindow::buildBookMarkList()
{
    QStringList lines;
    ReadFileToList(USERPATH + "BookMarkList.txt", lines, "UTF-8");

    for ( QStringList::const_iterator it = lines.begin(); it != lines.end(); ++it)
    {
        QString title = "";
        //Split title and link
        int p = it->indexOf("|"); //TODO: escape this guy
        if (p != -1) title = it->mid(p+1);
        addBookMarkToList(it->mid(0,p), title);
    }
}

//Open the clicked bookmark
void MainWindow::on_bookmarkWidget_itemDoubleClicked(QListWidgetItem* item)
{
    QString lnk = item->data(Qt::StatusTipRole).toString();

    QStringList parts = splittotwo(lnk, ":");
    int id;
    if(ToNum(parts[0], &id))
    {
        NodeBook* book = ui->treeWidget->findBookById(id);
        if ( book )
        {
            BookDisplayer* tabWidget = book->tabWidget();
            if ( tabWidget != 0 )
            {
                ui->viewTab->setCurrentWidget (tabWidget);
                tabWidget->setInternalLocation(parts[1]);
            }
            else
            {
                openBook(book, true);
                CurrentBookdisplayer()->setInternalLocation(parts[1]);
            }
        }
    }
}

//Opens a dialog allowing to edit the selected bookmark's title
void MainWindow::on_bookmarkEdit_clicked()
{
    if (ui->bookmarkWidget->currentIndex().isValid())
    {
        int ind = ui->bookmarkWidget->currentIndex().row();

        QString otitle = ui->bookmarkWidget->item(ind)->data(Qt::WhatsThisRole).toString();

        //Open the bookmark title giving dialog with this index and title
        AddBookMark *t = new AddBookMark(this, ind, otitle);
        t->show();

        //The dialog will call the editBookMarkTitle function when it's done
        connect (t, SIGNAL(RenameOK(int, QString)), this, SLOT(editBookMarkTitle(int, QString)));
    }
}

//Changes the title of the given bookmark (by it's index) to the given one
void MainWindow::editBookMarkTitle(int ind, QString newtitle)
{
    QString link = ui->bookmarkWidget->item(ind)->data(Qt::StatusTipRole).toString();

    QString otitle = ui->bookmarkWidget->item(ind)->data(Qt::WhatsThisRole).toString();
    if (otitle == bookMarkTitle(link)) otitle = "";  // ######## ?????

    //Edit the item title data (it's saved here...) to the new one
    ui->bookmarkWidget->item(ind)->setData(Qt::WhatsThisRole, newtitle);

    //Update file:
    QStringList lines;
    ReadFileToList(USERPATH + "BookMarkList.txt", lines, "UTF-8");

    lines[ind] = link + "|" + newtitle;

    writetofile(USERPATH + "BookMarkList.txt", lines.join("\n") + "\n", "UTF-8", true);

    //Update the new title in the list
    if (newtitle != "")
    {
        //Replace the titles
        if (otitle != "") ui->bookmarkWidget->item(ind)->setText( ui->bookmarkWidget->item(ind)->text().replace(otitle, newtitle) );
        //If there was an empty title before, add the newtitle and "("-s
        else ui->bookmarkWidget->item(ind)->setText( newtitle + " (" + bookMarkTitle(link) + ")");
    }
    //Empty title given
    else
    {
        //Remove the title and "("-s
        if (otitle != "")
            ui->bookmarkWidget->item(ind)->setText( ui->bookmarkWidget->item(ind)->text().replace(otitle + " (", "").replace(QRegExp("[)]$"), ""));
    }
}


//Remove the selected bookmark from the list and file
void MainWindow::on_removeBookmark_clicked()
{
    int ind = ui->bookmarkWidget->currentIndex().row();
    ui->bookmarkWidget->takeItem( ind );

    QStringList lines;
    ReadFileToList(USERPATH + "BookMarkList.txt", lines, "UTF-8");

    lines.removeAt(ind);

    writetofile(USERPATH + "BookMarkList.txt", (lines.empty() ? "" : lines.join("\n") + "\n"), "UTF-8", true);
}


//Move the selected bookmark up in the list by one position
void MainWindow::on_bookmarkUp_clicked()
{
    int ind = ui->bookmarkWidget->currentIndex().row();

    if (ind > 0)
    {
        //Swap the list elements:
        QListWidgetItem *tmp = ui->bookmarkWidget->takeItem( ind );
        ui->bookmarkWidget->insertItem( ind - 1 ,tmp);
        ui->bookmarkWidget->setCurrentRow( ind - 1 );

        QStringList lines;
        ReadFileToList(USERPATH + "BookMarkList.txt", lines, "UTF-8");

        lines.swap(ind, ind-1);

        writetofile(USERPATH + "BookMarkList.txt", lines.join("\n") + "\n", "UTF-8", true);
    }
}

//Move the selected bookmark down in the list by one position
void MainWindow::on_bookmarkDown_clicked()
{
    int ind = ui->bookmarkWidget->currentIndex().row();

    if ( ind < ui->bookmarkWidget->count() - 1 )
    {
        //Swap the list elements:
        QListWidgetItem *tmp = ui->bookmarkWidget->takeItem( ind );
        ui->bookmarkWidget->insertItem( ind + 1 ,tmp);
        ui->bookmarkWidget->setCurrentRow( ind + 1 );

        QStringList lines;
        ReadFileToList(USERPATH + "BookMarkList.txt", lines, "UTF-8");

        lines.swap(ind, ind+1);

        writetofile(USERPATH + "BookMarkList.txt", lines.join("\n") + "\n", "UTF-8", true);
    }
}

// appel depuis le menu principal
void MainWindow::menuBookMark()
{
    BookDisplayer* currentBookdisp = CurrentBookdisplayer();
    OraytaBookView* view = dynamic_cast<OraytaBookView*>(currentBookdisp->bookView());

    if (!view) return;

    QString link = view->activeLink();

    //Find book's id and add it to the link
    int id = currentBookdisp->book()->getRealUniqueId();

    if (id != -1) bookMarkPosition( QString::number(id) + ":" + link );
}

// widget du bas
void MainWindow::on_addBookMark_clicked()
{
    BookDisplayer* currentBookdisp = CurrentBookdisplayer();
    OraytaBookView* view = dynamic_cast<OraytaBookView*>(currentBookdisp->bookView());

    if (!view) return;

    QString link = view->activeLink();

    //Find book's id and add it to the link
    int id = currentBookdisp->book()->getRealUniqueId();

    if (id != -1) addBookMark(QString::number(id) + ":" + link, ui->bookmarkTitleLine->text());

    ui->bookmarkTitleLine->clear();
}
