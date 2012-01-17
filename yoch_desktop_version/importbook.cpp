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


#include "importbook.h"
#include "functions.h"
#include "ui_importbook.h"
#include "mainwindow.h"

#include <QFileDialog>


importBook::importBook(QWidget *parent) : QDialog(parent), ui(new Ui::importBook)
{
    ui->setupUi(this);

    ui->listWidget->setSelectionMode(QAbstractItemView::ExtendedSelection);
    ui->cancelBTN->hide();
    ui->importBTN->hide();
    ui->deleteButton->hide();
    ui->progressBar->hide();
    ui->fileCopied->hide();
}

importBook::~importBook()
{
    delete ui;
}

void importBook::on_addFolder_clicked()
{
    QString dirName = QFileDialog::getExistingDirectory(this, "", QDir::homePath(), QFileDialog::ShowDirsOnly);

    if (!dirName.isEmpty())
    {
        ui->listWidget->addItem(dirName);
        ui->cancelBTN->show();
        ui->importBTN->show();
        ui->deleteButton->show();
    }
}

void importBook::on_addBooks_clicked()
{
    QString filters = tr("All supported files "
                         "(*.html *.htm *.txt *.pdf);;"
                         "Html files(*.htm *.html);;"
                         "Text files(*.txt);;"
                         "Pdf files(*.pdf)");

    QStringList fileNames = QFileDialog::getOpenFileNames(this, "", QDir::homePath(), filters);

    if (!fileNames.empty())
    {
        for (int i=0; i<fileNames.size(); i++)
        {
            ui->listWidget->addItem(fileNames[i]);
        }
        ui->cancelBTN->show();
        ui->importBTN->show();
        ui->deleteButton->show();
    }
}

void importBook::on_deleteButton_clicked()
{
    QList<QListWidgetItem *> selected_items = ui->listWidget->selectedItems();
    qDeleteAll(selected_items.begin(), selected_items.end());

    if ( ui->listWidget->count() == 0 )
        ui->deleteButton->hide();
}

void importBook::on_label_linkActivated(QString link)
{
    //Open the project's site in a browser
    QDesktopServices::openUrl(QUrl(link));
}

void importBook::on_cancelBTN_clicked()
{
    close();
}

QList <QPair <QString,QString> > listAllFiles(const QString & srcPath, const QString & destPath, const QStringList & nameFilters)
{
    QList <QPair <QString,QString> > ret;
    QDirIterator it(srcPath, nameFilters, QDir::Files | QDir::NoDotAndDotDot | QDir::AllDirs, QDirIterator::Subdirectories);
    while (it.hasNext())
    {
        QString filename = it.next();
        ret << QPair<QString,QString>(filename, QString(filename).replace(srcPath, destPath));
    }
    return ret;
}

void importBook::on_importBTN_clicked()
{
    // A la place, on pourrait mettre une barre de progression, et fermer à la fin

    //Set a busy cursor before actually resizing the text, and restores it by the end
    QApplication::setOverrideCursor(Qt::WaitCursor);

    QString booksUserPath = USERPATH + "Books/";

    //Make sure the dir exists
    QDir d(booksUserPath) ;
    if ( !d.exists() )
        d.mkpath(booksUserPath);

    QStringList filters;
    filters << "*.html" << "*.htm" << "*.pdf" << "*.txt" << "*.conf";

    QList <QPair <QString,QString> > fileslist;
    //Copy to user's book folder
    for (int i=0; i < ui->listWidget->count(); i++)
    {
        QFileInfo f( ui->listWidget->item(i)->text() );

        if ( f.exists() )
        {
            fileslist << QPair<QString, QString>(f.absoluteFilePath(), booksUserPath + f.fileName());
            //Copy whole directory
            if( f.isDir() )
            {
                fileslist << listAllFiles(f.absoluteFilePath(), booksUserPath + f.fileName(), filters);
            }
        }
    }

    int nFiles = fileslist.size();
    int cmt = 0;

    if (nFiles > 0)
    {
        ui->progressBar->show();
        ui->fileCopied->show();

        QPair <QString, QString> it;
        foreach(it, fileslist)
        {
            if (QFileInfo(it.first).isDir())
            {
                //qDebug() << "make dir " << it.second;
                d.mkdir(it.second);
            }
            else
            {
                QString label = it.first.mid(it.first.lastIndexOf("/"));
                ui->fileCopied->setText(label);
                ui->progressBar->setValue( int(++cmt * 100 / nFiles) );

                if ( !QFile::copy(it.first, it.second) )
                    qDebug() << "Can't copy file" << it.first << " to " << it.second;
            }
        }
    }

    close();

    emit updateTreeSignal();

    QApplication::restoreOverrideCursor();
}
