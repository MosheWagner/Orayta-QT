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

#include "desktopapp.h"
#include "bookdisplayer.h"
#include "../OraytaBase/minibmark.h"

#include <QDebug>
#include <QInputDialog>

// This is just a very simple define. every place in the code,
//  "CURRENT_TAB" simply represents "ui->viewTab->currentIndex()".
#define CURRENT_TAB ui->viewTab->currentIndex()

void DesktopApp::on_bookMarkList_customContextMenuRequested(QPoint pos)
{
    QMenu menu(ui->treeWidget);

    QAction *edit = new QAction(tr("Edit bookmark title..."), &menu);
    edit->setIcon(QIcon(":/Icons/edit.png"));

    QAction *del = new QAction(tr("Delete bookmark"), &menu);
    del->setIcon(QIcon(":/Icons/edit-delete.png"));

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

//Adds the given book position to the bookmark list and file
void DesktopApp::addBookMarkHere(QString title = "")
{
    Book *b =  CurrentBookdisplayer()->book();
    BookIter itr = CurrentBookdisplayer()->currentLocation;

    if (!b)
    {
        qDebug() << "Invalid book for bookmark!";
        return;
    }

    MiniBMark * bm = ui->bookMarkList->addBookMark(b, itr);
    bm->setConstant(true);

    if (title.simplified() != "") bm->setTitle(title.simplified());
    ui->bookmarkTitleLine->clear();
}

void DesktopApp::menuBookMark()
{
    bool ok;
    QString title = QInputDialog::getText(this, tr("Bookmark title"), tr("Bookmark title:"), QLineEdit::Normal, "", &ok);
    if (ok && !title.isEmpty()) addBookMarkHere(title);
    else if(ok) addBookMarkHere();
}


//Open the clicked bookmark
void DesktopApp::on_bookMarkList_itemDoubleClicked(QListWidgetItem* item)
{
    MiniBMark *bm= dynamic_cast<MiniBMark *>(item);
    if (!bm || bm == 0) return;

    qDebug() << bm->getBook()->getName();

    //Add a new tab and open the link there
    addViewTab(false);
    ui->viewTab->setTabText(CURRENT_TAB, tr("Orayta"));

    CurrentBookdisplayer()->setInternalLocation("#" + bm->getBookIter().toEncodedString());
    openBook(bm->getBook());

}

//Opens a dialog allowing to edit the selected bookmark's title
void DesktopApp::on_bookmarkEdit_clicked()
{
    if (ui->bookMarkList->currentIndex().isValid())
    {
        int ind = ui->bookMarkList->currentIndex().row();

        QString otitle = static_cast<MiniBMark*>(ui->bookMarkList->currentItem())->title();

        bool ok;
        QString ntitle = QInputDialog::getText(this, tr("Change bookmark title"),
                                         tr("New bookmark title:"), QLineEdit::Normal, otitle, &ok);

        if (ok && !ntitle.isEmpty()) editBookMarkTitle(ind, ntitle);
    }
}

//Changes the title of the given bookmark (by it's index) to the given one
void DesktopApp::editBookMarkTitle(int ind, QString newtitle)
{
    MiniBMark* b = static_cast<MiniBMark*>(ui->bookMarkList->item(ind));
    b->setTitle(newtitle);
}


//Remove the selected bookmark from the list and file
void DesktopApp::on_removeBookmark_clicked()
{
    delete ui->bookMarkList->currentItem();
}


void DesktopApp::on_addBookMark_clicked()
{
    addBookMarkHere(ui->bookmarkTitleLine->text());
}
