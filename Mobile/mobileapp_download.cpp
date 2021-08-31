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

#include "mobileapp.h"
#include "ui_mobileapp.h"
#include <QDebug>
#include <QDir>

/*
  This file includes all functions of the mobileapp class that deal with downloading books -
  The booklist file abd the books themselves.
 */


// start download of the selected books.
void MobileApp::on_downloadBTN_clicked()
{
    downloadStart();
}

void MobileApp::on_downloadListWidget_itemClicked(QListWidgetItem *item)
{
    //This is a little hack to prevent double events on some android machines and emulators.
    //If the function is called again in less than 2 ms, the second time is ignored.
    qint64 miliSec = timer.restart();
    if (miliSec < 200) return;

    //Invert the selection of the item only if it was not chnaged by the click itself already.
    // (In other words, if the user clicked the checkbox, it will work without us. if he clicked somewhere else - we should invert the value)
    if ((item->checkState() == Qt::Checked && item->toolTip() == "True") ||
        (item->checkState() == Qt::Unchecked && item->toolTip() == "False") )
    {
        if (item->checkState() == Qt::Checked)
        {
            item->setCheckState(Qt::Unchecked);
            item->setToolTip("False");
        }
        else
        {
            item->setCheckState(Qt::Checked);
            item->setToolTip("True");
        }
    }
    else
    {
        if (item->checkState() == Qt::Checked) item->setToolTip("True");
        else item->setToolTip("False");
    }
}

void MobileApp::downloadBookList()
{
    //show aprorpriate widgets
    ui->downloadSTKWidget->setCurrentIndex(0);

    //If we already created a download list widget, we are done.
    // In any other case, we should download the list.
    if (ui->downloadListWidget->count() < 1)
    {
        listdownload = new FileDownloader();
        connect(listdownload, SIGNAL(done()), this, SLOT(listDownloadDone()));
        listdownload->Download(BOOKLISTURL, SAVEDBOOKLIST, true);
    }
    else ui->downloadSTKWidget->setCurrentIndex(1);
}

void MobileApp::downloadDailyLimudFiles()
{
    FileDownloader * limudDownloader = new FileDownloader();
    limudDownloader->Download(LIMUD_YOMI_URL, LIMUD_YOMI_FILE, true);
}

void MobileApp::updateDownloadableList()
{
    QList <QString> dl;
    groups.clear();

    ReadFileToList(SAVEDBOOKLIST, dl, "UTF-8");
    parseDLFile(dl);

    //show aprorpriate widgets
    ui->downloadSTKWidget->setCurrentIndex(1);


    //Clear the old list
    ui->downloadListWidget->clear();

    //Build the new list
    for (int i=0; i<groups.size(); i++)
    {
        if (groups[i].downloadState != 0)
        {
            QListWidgetItem *lwi;
            lwi= new QListWidgetItem(groups[i].name + " (" + QString::number(groups[i].downloadSize)
                                     + /* "/" + QString::number(groups[i].fullSize) + */ " MB)");
            if (autoInstallKukBooksFlag && groups[i].name.contains("הרחבה"))
                lwi->setCheckState(Qt::Checked);
            else
                lwi->setCheckState(Qt::Unchecked);
            lwi->setWhatsThis(stringify(i));
            lwi->setToolTip("False");
            if(groups[i].hidden)
                lwi->setTextColor(QColor("gray"));

            ui->downloadListWidget->addItem(lwi);
            ui->downloadListWidget->setEnabled(true);
        }
    }

    if(autoInstallKukBooksFlag)
        downloadStart();

}

// Parse the booklist file
void MobileApp::parseDLFile(QList <QString> dl)
{
    for (int i=0; i<dl.size(); i++)
    {
        //Comment, ignore
        if (dl[i].startsWith("#")) {}
        //Group name
        else if (dl[i].startsWith("@"))
        {
            //Create new group
            DownloadbleBookGroup g;
            g.name = dl[i].mid(2);
            g.hidden=false;

            groups.append(g);
        }

        else if (dl[i].startsWith("./"))
        {
            if (groups.size() < 1)
            {
                qDebug() << "Error! Book with no group... Aborting download list";
                return ;
            }

            DownloadbleBookObject bo;
            bo.hash = "";
            QStringList sl = dl[i].split(", ");
            if (sl.size() < 3)
            {
                qDebug() << "Error! Invalid book entry! Skipping.";
                break ;
            }

            QString t = sl[0];
            bo.URL = t.replace("./", "https://raw.githubusercontent.com/MosheWagner/Orayta-Books/master/books/");
            bo.UnpackPath = sl[0].replace("./", BOOKPATH);
            int n; if (ToNum(sl[1], &n)) bo.fileSize = n / 1000000.0;

            bo.dateModified = QDate::fromString(sl[2].simplified(), "dd/MM/yy");
            //Because QT thinks '12' is 1912 and not 2012...
            bo.dateModified.setDate(100 + bo.dateModified.year(), bo.dateModified.month(), bo.dateModified.day());

            //If a hash is present for this line
            if(sl.size() > 3)
            {
                bo.hash = sl[3];
            }

            groups.last().books.append(bo);
        }
    }


    for (int i=0; i<groups.size(); i++)
    {
        //qDebug() << "#" << groups[i].name << groups[i].groupSize;

        bool hasAll = true;
        bool hasNone = true;

        for (int j=0; j<groups[i].books.size(); j++)
        {
            Book * b = bookList.FindBookByPath(groups[i].books[j].UnpackPath);

            bool needToDownload;

            if (b)
            {
                if (b->modificationDate() >= QDateTime(groups[i].books[j].dateModified))
                {
                    needToDownload = false;
                }
                else needToDownload = true;
            }
            else needToDownload = true;

            if (!needToDownload)
            {
                    groups[i].books[j].needToDownload = false;
                    hasNone = false;
            }
            else
            {
                groups[i].books[j].needToDownload = true;
                hasAll = false;
            }
        }

        //Calculate size of download
        double fs = 0, ds = 0;
        for (int k=0; k < groups[i].books.size(); k++)
        {
            if (groups[i].books[k].needToDownload) ds += groups[i].books[k].fileSize;
            fs += groups[i].books[k].fileSize;
        }
        groups[i].fullSize = int (fs * 10) / 10.0;
        groups[i].downloadSize = int (ds * 10) / 10.0;

        if (hasAll) groups[i].downloadState = 0; //All installed
        else if (hasNone) groups[i].downloadState = 2; //None installed
        else groups[i].downloadState = 1; //Needs update

        //qDebug() << groups[i].name <<  groups[i].downloadState << groups[i].fullSize << groups[i].downloadSize;
   }
}


void MobileApp::listDownloadDone()
{
    qDebug() << "#";
    //If all is ok
    if (listdownload)
    {
        if (listdownload->getFileName().contains("Orayta"))
        {
            updateDownloadableList();
        }
        else
        {
            listdownload = NULL;
            //Retry the download
            downloadBookList();
        }

    }
    else
        // this shouldn't happen, but if so we should recreate everything.
        downloadBookList();
}

void MobileApp::downloadStart()
{
    downloadsList.clear();
    hashs.clear();

    for (int i=0; i<ui->downloadListWidget->count(); i++)
    {
        QListWidgetItem *item = ui->downloadListWidget->item(i);
        if (item->checkState() == Qt::Checked)
        {
            //Generate download urls
            int n;
            if (ToNum(item->whatsThis(), &n))
            {
                if (groups.size() > n)
                {
                    for (int j=0; j<groups[n].books.size(); j++)
                    {
                        if (groups[n].books[j].needToDownload)
                        {
                            QString url = groups[n].books[j].URL;
                            downloadsList << url;
                            hashs << groups[n].books[j].hash;
                        }
                    }
                }
            }
        }
    }
    downloadNum = downloadsList.size();

    ui->downloadInfo->toolTip() = "";

    ui->downloadListWidget->setEnabled(false);
    ui->downloadBTN->setEnabled(false);


    // download the next file in downloadsList.
    downloadNext();
}

// download the next file in downloadsList.
void MobileApp::downloadNext()
{
    if (!downloadsList.isEmpty())
    {
        //Reset small progress bar
        ui->downloadPrgBar->show();
        ui->downloadPrgBar->setValue(0);

        QString url = downloadsList.first();
        QString name = url.mid(url.lastIndexOf("/") + 1);
        //Generate download target
        QString target = QString(url).replace("https://raw.githubusercontent.com/MosheWagner/Orayta-Books/master/books/", BOOKPATH);
        QString p = target.left(target.length() - name.length());
        QDir().mkpath(p);

        //qDebug() <<"download file to: "<< target;
        downloader->Download(url, target, true, hashs.first());

        //display download information:
        ui->downloadInfo->setText(tr("Downloading file ") + QString::number(downloadNum - downloadsList.size() + 1) + tr(" of ") + QString::number(downloadNum));

        downloadsList.removeFirst();
        hashs.removeFirst();
    }
    //No more books to download
    else
    {
        //qDebug() << "done downloading";
        //display download information:
        ui->downloadInfo->setText(tr("Download complete!"));

        if (ui->downloadInfo->toolTip() == "Error")
        {
            ui->downloadInfo->setText(tr("Download complete with errors. Try downloading the files again."));
        }

        //reload the book tree
        reloadBooklist();

        //Refresh the download list window
        ui->downloadListWidget->clear();

        //reset the download page
        ui->downloadPrgBar->hide();
        ui->downloadListWidget->setEnabled(true);
        ui->downloadBTN->setEnabled(true);


        if (ui->downloadInfo->toolTip() == "")
        {
            //Switch view to book tree to see the new books
            if (ui->stackedWidget->currentIndex() == GET_BOOKS_PAGE)
                ui->stackedWidget->setCurrentIndex(LIST_PAGE);
        }

        autoInstallKukBooksFlag= false;
    }
}


void MobileApp::downloadProgress(int val) { ui->downloadPrgBar->setValue(val); }

#include <QMessageBox>

void MobileApp::downloadError()
{
    qDebug() << "Error downloading: " + downloader->getFileName();

    ui->downloadInfo->setToolTip("Error");

    //QMessageBox msgBox;
    //msgBox.setText(tr("Error downloading: ")  + downloader->getFileName() + tr("\nPlease try to re download it."));
    //msgBox.exec();

    //remove the tmpfile created by the downloader.
    //downloader->getFile()->remove();

    downloadNext();
}

//called when book package has done downloading.
void MobileApp::downloadDone()
{
    //Book downloaded
    if (downloader)
    {
        ui->downloadPrgBar->hide();

        //this file has finished downloading, get the next file.
        downloadNext();
    }
}

