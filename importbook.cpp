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


    ui->cancelBTN->hide();
    ui->importBTN->hide();
}

importBook::~importBook()
{
    delete ui;
}


void importBook::on_addBTN_clicked()
{
    QFileDialog dialog(this);
    dialog.setDirectory(QDir::homePath());
    dialog.setFileMode(QFileDialog::ExistingFiles);
    dialog.setNameFilter(tr("Html files(*.htm *.html);;Text files(*.txt);;Pdf files(*.pdf)"));
    QStringList fileNames;

    if (dialog.exec())
    {
        fileNames = dialog.selectedFiles();

        for (int i=0; i<fileNames.size(); i++)
        {
            ui->listWidget->addItem(fileNames[i]);
        }
    }

    if (!fileNames.empty())
    {
        ui->cancelBTN->show();
        ui->importBTN->show();

        ui->addBTN->hide();
    }
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
    for (int i=0; i<ui->listWidget->count(); i++)
    {
        //Make sure the dir exists
        QDir d(USERPATH + "/Books/ספרי_המשתמש") ;
        if (!d.exists()) d.mkpath(USERPATH + "/Books/ספרי_המשתמש");

        QFileInfo f(ui->listWidget->item(i)->text());
        QFile cf;

        //Copy to user's book folder
        if (f.exists())
        {
            //Copy the file to the user's dir
            if (! cf.copy(f.absoluteFilePath(),USERPATH + "/Books/ספרי_המשתמש/" + f.fileName())) qDebug() << "Can't copy file" << f.absoluteFilePath();
        }
    }

    emit updateTree();

    close();
}
