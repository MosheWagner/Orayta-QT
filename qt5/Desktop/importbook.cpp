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
#include "../OraytaBase/functions.h"
#include "ui_importbook.h"

#include <QDesktopServices>
#include <QDebug>
#include <QUrl>
#include <QFileDialog>


importBook::importBook(QWidget *parent) : QDialog(parent), ui(new Ui::importBook)
{
    ui->setupUi(this);

    ui->listWidget->setSelectionMode(QAbstractItemView::ExtendedSelection);
    ui->cancelBTN->hide();
    ui->importBTN->hide();
    ui->deleteButton->hide();
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
    for (int i=0; i < selected_items.size(); i++)
    {
        delete selected_items[i];
    }

    if ( ui->listWidget->count() == 0 )
        ui->deleteButton->hide();
}

void importBook::on_label_2_linkActivated(QString link)
{
    //Open the project's site in a browser
    QDesktopServices::openUrl(QUrl(link));
}

void importBook::on_cancelBTN_clicked()
{
    close();
}

void importBook::on_importBTN_clicked()
{
    QString booksUserPath = USERPATH + "Books/ספרי_המשתמש";

    //Make sure the dir exists
    QDir d(booksUserPath) ;
    if ( !d.exists() )
        d.mkpath(booksUserPath);

    booksUserPath = booksUserPath + "/";

    QStringList filters;
    filters << "*.html" << "*.htm" << "*.pdf" << "*.txt";



    //Copy to user's book folder
    for (int i=0; i<ui->listWidget->count(); i++)
    {
        QFileInfo f( ui->listWidget->item(i)->text() );

        if ( f.exists() )
        {
            //Copy whole directory
            if( f.isDir() )
            {
                copyFolder( f.absoluteFilePath(), booksUserPath + f.fileName(), filters);
            }
            else //Copy file
            {
                QFile cf;

                if ( !cf.copy(f.absoluteFilePath(), booksUserPath + f.fileName()) )
                    qDebug() << "Can't copy file" << f.absoluteFilePath();
            }
        }
    }

    emit updateTree();

    close();
}
