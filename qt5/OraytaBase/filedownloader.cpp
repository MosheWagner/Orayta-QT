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

#include "filedownloader.h"
#include <QUrl>
#include <QDebug>
#include <QNetworkRequest>
#include "functions.h"

FileDownloader::FileDownloader()
{
    mFileName = "";
    mTargetFile.setFileName(mFileName);
    cdownload = NULL;
}


//Downloads the file at the given url to the given target.
// Returns nothing, but emits signalls representing what happened
void FileDownloader::Download(QString rUrl, QString rTarget, bool overwrite, QString hash)
{
    mFileName = rTarget;

    //If last download wasn't shut down properly - close it now
    if ( mTargetFile.isOpen())
    {
        mTargetFile.close();
    }

    mTargetFile.setFileName(rTarget);

    mHash = hash;

    //Check if a file exists in the given target:
    if (mTargetFile.exists() && overwrite == false)
    {
        //Use the existing file, so the download is done
        emit done();
    }
    else
    {
        //Set the download to a tepmorary name
        mTargetFile.setFileName(rTarget + ".part");
        //Open the file for writing:
        mTargetFile.open(QIODevice::ReadWrite);

        QNetworkRequest request(rUrl);

        cdownload = mNetworkObject.get(request);
        connect(cdownload, SIGNAL(downloadProgress(qint64,qint64)), SLOT(downloadProgress(qint64,qint64)));
        connect(cdownload, SIGNAL(finished()), SLOT(downloadDone()));
        connect(cdownload, SIGNAL(readyRead()),SLOT(downloadReadyRead()));
    }
}

void FileDownloader::downloadReadyRead()
{
    mTargetFile.write(cdownload->readAll());
}

// Called when the download is done.
// Emitts the error signal if it failed, or the done signall if it was succesful
void FileDownloader::downloadDone()
{
    if (cdownload->error())
    {
        emit downloadError();
    }
    else
    {
        mTargetFile.close();
        if (isValid())
        {
            //Rename the filename to the real name (without the ".part");
            //first remove the old version if it exists:
            QFile old(mFileName);
            if (old.exists()) old.remove();
            //Now we can rename the new file:
            bool ok = mTargetFile.rename(mFileName);

            if (!ok)
            {
                qDebug()<< "Can't rename: " << mTargetFile.fileName() << " as: " << mFileName;
            }

            //Force deletion of .part file
            QFile deleter(mFileName + ".part");
            if (deleter.exists()) deleter.remove();

            emit done();
        }
        else emit downloadError();
    }
}

//Called as the HttpRequest procceds.
void FileDownloader::downloadProgress(qint64 done, qint64 total)
{
    if (total == 0)
    {
        emit downloadProgress(0);
    }
    else
    {
        int percent = ( 100 * done ) / total;
        emit downloadProgress(percent);
    }
}

bool FileDownloader::isValid()
{
    QFile test(mTargetFile.fileName());

    if (!test.open(QIODevice::ReadOnly))
    {
        return false;
    }
    else
    {
        //Make sure the file isn't empty (Read only first 10 KB)
        QString s = QString(test.read(1024 * 10).simplified());
        if (s == "" || s.indexOf("not found on this server") != -1 )
        {
            qDebug()<< "Empty file: " << mFileName << " You may want to download it again or report a bug.";
            return false;
        }

        //Compare to hash
        if (mHash != "")
        {
            if ( fileHash(mTargetFile.fileName()).simplified() != mHash.simplified() )
            {
                qDebug() << "Downloaded file: " << mFileName << " Does not match the hash it should have. Try to download it again or report a bug.";
                return false;
            }
        }
    }

    test.close();

    return true;
}

//Returns the filename of the last download
QString FileDownloader::getFileName()
{
    return mFileName;
}
