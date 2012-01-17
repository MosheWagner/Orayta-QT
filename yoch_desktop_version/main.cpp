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
#include "settings.h"

#include <windows.h>

//Define location for program dirs
void initPaths()
{
    QDir dir("Books/");
    QString defPath("");

#ifdef Q_OS_LINUX
    defPath = "/usr/share/Orayta/Books/";
#elif defined Q_OS_WIN
    defPath = QDir::rootPath() + "\\program files\\orayta"; //TODO: ask yoch to currect this.
#endif

    if (dir.exists())
        BOOKPATH = dir.absolutePath() + "/" ;
    else
        BOOKPATH = defPath;

    dir.setPath("UserData/");
    if (dir.exists())
        USERPATH =  dir.absolutePath() + "/";
    else
    {
        dir.mkdir(QDir::homePath() + "/.Orayta/");
        USERPATH = QDir::homePath() + "/.Orayta/";
    }

   // qDebug() << "temp path:\n" << QDir::tempPath();

    dir.setPath("Htmltmp/");
    if (dir.exists())
        TMPPATH = dir.absolutePath() + "/";
    else
    {
        dir.mkdir(QDir::tempPath() + "/Orayta/");
        dir.mkdir(QDir::tempPath() + "/Orayta/Htmltmp/");

        TMPPATH = QDir::tempPath() + "/Orayta/Htmltmp/";
    }

    dir.setPath("Pics/");
    if ( !dir.exists() )
    {
//UNIX ONLY:
#ifndef Q_WS_WIN
//Create symbolic link to the folder

        //TODO: this should be solved in a different way. the html files should be corrected so the link is to the right place
        system("ln -s -T /tmp/Orayta/Pics /usr/share/Orayta/Books/Pics 2> /dev/null");
#endif
    }
}

void initLang(QApplication *app)
{
    //Read lang conf
    QSettings settings("Orayta", "SingleUser");
    settings.beginGroup("Confs");

    bool systemlang = settings.value("systemLang",true).toBool();
    if (systemlang)
        LANG = QLocale::languageToString(QLocale::system().language());
    else
        LANG = settings.value("lang","Hebrew").toString();

    settings.endGroup();


    translator = new QTranslator();

    //Update path for translator installation
    if (!translator->load(LANG + ".qm", "."))
        translator->load(LANG + ".qm", "/usr/share/Orayta/");

    //Install as the app's translator
    app->installTranslator(translator);
}

int main(int argc, char *argv[])
{
    //Sleep(10000);

    //Set all QString to work with utf-8
    QTextCodec::setCodecForCStrings(QTextCodec::codecForName("utf8"));
    QTextCodec::setCodecForTr(QTextCodec::codecForName("utf8"));

    //Start the App
    QApplication app(argc, argv);

    //Define location for program dirs
    initPaths();

    //Define the program's language
    initLang(&app);

    //Show splash screen:
    QPixmap pixmap(":/Images/Orayta.png");
    QSplashScreen *splash = new QSplashScreen(pixmap);
    splash->show();

    MainWindow w;
    w.show();
    app.processEvents();

    //Hide slpash screen
    splash->finish(&w);

    return app.exec();
}
