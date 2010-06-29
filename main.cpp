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

QStringList langs;

//Define location for program dirs
void initPaths()
{
    QDir dir("Books/");
    if (dir.exists()) BOOKPATH = dir.absolutePath() + "/" ;
    else BOOKPATH = "/usr/share/Orayta/Books/";

    dir.setPath("UserData/");
    if (dir.exists()) USERPATH =  dir.absolutePath() + "/";
    else
    {
        dir.mkdir(QDir::homePath() + "/.Orayta/");
        USERPATH = QDir::homePath() + "/.Orayta/";
    }

    dir.setPath("Htmltmp/");
    if (dir.exists()) TMPPATH = dir.absolutePath() + "/";
    else
    {
        //Improved by Yoch. Thanks again.
        dir.mkdir(QDir::tempPath() + "/Orayta/");
        dir.mkdir(QDir::tempPath() + "/Orayta/Htmltmp/");

        TMPPATH = QDir::tempPath() + "/Orayta/Htmltmp/";
    }

    dir.setPath("Pics/");
    if ( !dir.exists() )
    {
        //UNIX ONLY:
        #ifndef Q_WS_WIN    //Yoch's idea. Thank you very much :-)
        //Create symbolic link to the folder
            system("ln -s /usr/share/Orayta/Books/Pics /tmp/Orayta/Pics");
        #endif
    }
}



int main(int argc, char *argv[])
{
    //Start the App
    QApplication app(argc, argv);

    //Define location for program dirs
    initPaths();

    //Read lang conf
    QSettings settings("Orayta", "SingleUser");
    settings.beginGroup("Confs");
    LANG = settings.value("lang","Hebrew").toString();
    settings.endGroup();


    QTranslator translator;

    //Update path for installation
    if (!translator.load(LANG + ".qm", ".")) translator.load(LANG + ".qm", "/usr/share/Orayta/");
    app.installTranslator( &translator );

    system("pwd");

    //Show splash screen:
    QPixmap pixmap(":/Images/Orayta.png");
    QSplashScreen *splash = new QSplashScreen(pixmap);
    splash->show();
    QApplication::setOverrideCursor(Qt::WaitCursor);

    app.processEvents();

    MainWindow w;   

    //Hide slpash screen
    splash->finish(&w);
    QApplication::restoreOverrideCursor();

    w.show();
    return app.exec();
}
