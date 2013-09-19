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

#ifndef FILEDOWNLOADER_H
#define FILEDOWNLOADER_H

#include <QObject>
#include <QFile>
#include <QtNetwork/QHttp>
/*
  This class provides a simple method to download requested files;
  The download is saved into $path+".part" untill download is succesfull,
  and only then it is moved to the given path.

  ( If a file exists in the given position, it is asuumed to be a successfull previous download,
     and the download is instantly done. )

  It emits these signalls:
  -> downloadProgress(int) - gives a integer between 1 to 100 representing the current dowload progress.
                              (or zero as long as it didn't start yet)
  -> done() - emitted when the download is finished succesfully
  -> Error() - emitted when the download failed due to an error, but not if it was aborted

  And accepts these slots:
  -> abort() - stops the download, removes the ".part" file, but does not emmit a "error" signal
*/

class FileDownloader : public QObject
{
    Q_OBJECT

public:
    FileDownloader();

    //Downloads the file at the given url to the given target.
    // Returns nothing, but emits signalls representing what happened
    void Download(QString url, QString target, bool overwrite, QString hash = "");

    //Returns the filename of the last download (NOTE: also if it was unsuccessful)
    QString getFileName();

    //Returns true if a download is running (at any state), and false if not
    bool isInProgress();

private:
    //The class's Http object
    QHttp mHttpObject;
    //The file handling the download proccess
    QFile mTargetFile;
    //File name for the finished download
        // (Until it's done it's saved to the same name with ".part" after it)
    QString mFileName;
    //The downloaded file should have this hash when done
    QString mHash;
    //Tests if the downloaded file is valid, using a hash if given.
    bool isValid();

public slots:
    //Aborts the download, and emits no error
    void abort();

private slots:
    //Connects to the Http object's done() signall
    void downloadDone(bool error);
    //Connects to the Http object's download progress() signall
    void downloadProgress(int,int);

signals:
    //Emitted when download WAS successfuly done
    void done();
    //Emitted when download WASN'T successfuly done
    void downloadError();
    //Emitted as the download continues, giving it's progress in percents.
    void downloadProgress(int);
};

#endif // FILEDOWNLOADER_H
