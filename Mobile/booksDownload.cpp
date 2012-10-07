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
            lwi->setCheckState(Qt::Unchecked);
            lwi->setWhatsThis(stringify(i));
            lwi->setToolTip("False");

            ui->downloadListWidget->addItem(lwi);
            ui->downloadListWidget->setEnabled(true);
        }
    }

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
            QStringList sl = dl[i].split(", ");
            if (sl.size() < 3)
            {
                qDebug() << "Error! Invalid book entry! Skipping.";
                break ;
            }

            QString t = sl[0];
            bo.URL = t.replace("./", "https://orayta.googlecode.com/svn/books/");
            bo.UnpackPath = sl[0].replace("./", BOOKPATH);
            int n; if (ToNum(sl[1], &n)) bo.fileSize = n / 1000000.0;

            bo.dateModified = QDate::fromString(sl[2].simplified(), "dd/MM/yy");
            //Because QT thinks '12' is 1912 and not 2012...
            bo.dateModified.setYMD(100 + bo.dateModified.year(), bo.dateModified.month(), bo.dateModified.day());
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

        qDebug() << groups[i].name <<  groups[i].downloadState << groups[i].fullSize << groups[i].downloadSize;
   }
}


void MobileApp::listDownloadDone()
{
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
    downloader->abort();
    downloadsList.clear();

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
                        }
                    }
                }
            }
        }
    }
    ui->downloadListWidget->setEnabled(false);
    ui->downloadBTN->setEnabled(false);

    //qDebug() << downloadsList;

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
        QString target = QString(url).replace("https://orayta.googlecode.com/svn/books/", BOOKPATH);
        QString p = target.left(target.length() - name.length());
        QDir().mkpath(p);

        //qDebug() <<"download file to: "<< target;
        downloader->Download(url, target, true);

        //display download information:
        ui->downloadInfo->setText(name);

        downloadedBooks << downloadsList.first();

        downloadsList.removeFirst();
    }
    //No more books to download
    else
    {
        //qDebug() << "done downloading";
        //display download information:
        ui->downloadInfo->setText(tr("Download complete!"));

        //reload the book tree
        reloadBooklist();

        //Refresh the download list window
        ui->downloadListWidget->clear();

        //reset the download page
        ui->downloadPrgBar->hide();
        ui->downloadListWidget->setEnabled(true);
        ui->downloadBTN->setEnabled(true);


        //Switch view to book tree to see the new books
        if (ui->stackedWidget->currentIndex() == GET_BOOKS_PAGE)
            ui->stackedWidget->setCurrentIndex(LIST_PAGE);
    }
}


void MobileApp::downloadProgress(int val) { ui->downloadPrgBar->setValue(val); }

void MobileApp::downloadError()
{
    qDebug() << "Error downloading:" + downloader->getFileName();

    //remove the tmpfile created by the downloader.
//    downloader->getFile()->remove();
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
