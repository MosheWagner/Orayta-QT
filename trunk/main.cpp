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

#include <QApplication>
#ifdef MOBILE
    #include "Mobile/mobileapp.h"
#else
    #include "Desktop/desktopapp.h"
#endif
#include <QDir>
#include <QSettings>
#include "OraytaBase/functions.h"
#include "Desktop/about.h"
#include <QDebug>
#include <QTranslator>
#include <QSplashScreen>
#include <iostream>

#include <QFontDatabase>

#include <QDesktopWidget>
#include <QPainter>
#include <QRect>

#include <QFileInfo>
#include <QFileInfoList>


//For test only
//#define MOBILE


//Define location for program dirs
void initPaths()
{

    QString defPath;

#ifdef Q_OS_ANDROID
    defPath = "/sdcard/Orayta/Books/";
    //set terget to assets folder
    MAINPATH = "assets:/Orayta/";
#elif defined Q_OS_LINUX
    defPath = "/usr/share/Orayta/Books/";
    MAINPATH = "/usr/share/Orayta/";
#elif defined Q_OS_WIN
    defPath = QDir::rootPath() + "\\program files\\orayta\\books\\"; //TODO: ask yoch to currect this.
    MAINPATH = QDir::rootPath() + "\\program files\\orayta\\";
#endif

    QDir dir("Books/");
    if (dir.exists()) BOOKPATH = dir.absolutePath() + "/" ;
    else BOOKPATH = defPath;
    qDebug() << "bookpath:" << BOOKPATH ;

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

            //TODO: this should be solved in a different way. the html files should be corrected so the link is to the right place

            system("ln -s -T /tmp/Orayta/Pics /usr/share/Orayta/Books/Pics 2> /dev/null");
        #endif
    }
}


//IZAR: adds a font to the application
void addFont(const QString &font)
{
    int fontId = QFontDatabase::addApplicationFont(font);
    if (fontId >= 0) {
        qDebug()<< "installed font successfuly: " << font;
        return;
    }
    else
        qDebug()<< "cant add font "<< font;
}

//IZAR: define location for fonts
void initFonts()
{
    QList<QString> fonts;
//    QString fontpath (":/fonts/");
    QString fontpath (MAINPATH + "fonts/");
#ifdef Q_OS_ANDROID
    fontpath = "/sdcard/Orayta/fonts/"; //assets not working well yet on alpha 3.4
#elif defined MOBILE_TEST
    fontpath = "fonts/";
#endif

    QDir fontDir(fontpath);
    qDebug() << "fontdir: " << fontDir;

    QStringList filter("*.ttf");
    QFileInfoList list = fontDir.entryInfoList(filter, QDir::AllEntries | QDir::NoDotAndDotDot, QDir::Name);
    foreach(QFileInfo child, list){
        addFont(child.filePath());
    }
}

void initLang(QApplication *app)
{
    //Read lang conf
    QSettings settings("Orayta", "SingleUser");
    settings.beginGroup("Confs");

    bool systemlang = false;
#ifndef MOBILE
    //system language disabled in mobile because it doesn't work.
    systemlang = settings.value("systemLang",true).toBool();

#endif

    if (systemlang) LANG = QLocale::languageToString(QLocale::system().language());
    else (LANG = settings.value("lang","Hebrew").toString());
//    qDebug()<<"lang: " + LANG;

    settings.endGroup();


    extern QTranslator *translator;
    translator = new QTranslator();

    //Update path for translator installation
    QString transPath (MAINPATH);
    if (!translator->load(LANG + ".qm", ".")) translator->load(LANG + ".qm", transPath);
    QFile file(transPath+LANG + ".qm");
    qDebug()<<"trans: " << file.fileName();
    //Install as the app's translator
    app->installTranslator(translator);
}


#include <QPlastiqueStyle>

int main(int argc, char *argv[])
{
    //Start the App
    QApplication app(argc, argv);

    //Deal with command line options
    //TODO: add more if it's needed
    QStringList args = app.arguments();
    if (args.contains("-v") || args.contains("--version"))
    {
        cout << "Orayta, Hebrew books program, ";
        cout << "Version " << VERSION << endl;

        exit(0);
    }

    if (args.contains("-i") || args.contains("--import"))
    {

    }

    //Build DB option chosen
    if (args.contains("-D") || args.contains("--buildDB"))
    {
        int p = args.indexOf("-D");
        if (p == -1) p = args.indexOf("--buildDB");

        QString path = "";
        if (args.size() > p + 1) path = args[p+1];
        else
        {
            qDebug() << "Error! no path for DB buldng given.";
            return 2;
        }

        QString lvlmapoutpath;
        QString dboutpath;
        QString pt = path;
        if (path.endsWith(".txt"))
        {
            lvlmapoutpath = pt.replace(".txt", ".LMP");
            dboutpath = pt.replace(".LMP", ".TDB");
        }
        if (path.endsWith(".TXT"))
        {
            lvlmapoutpath = pt.replace(".TXT", ".LMP");
            dboutpath = pt.replace(".LMP", ".TDB");
        }
        GenerateSearchTextDB(path, dboutpath, lvlmapoutpath);

        return 0;
    }


    //Define location for program dirs
    initPaths();


    //IZAR: add fonts to our app.
    initFonts();


    //Define the program's language
    initLang(&app);

#ifndef MOBILE
    //Show splash screen:
    QPixmap pixmap(":/Images/Orayta.png");
    QSplashScreen *splash = new QSplashScreen(pixmap);
    splash->show();
    QApplication::setOverrideCursor(Qt::WaitCursor);

    app.processEvents();

    QApplication::restoreOverrideCursor();


    DesktopApp d;
    d.show();
    app.processEvents();

    //Hide slpash screen
    splash->finish(&d);
#endif

#ifdef MOBILE
    //Show splash screen:
//    QPixmap basePixmap(":/Images/Orayta.png");
    QPixmap basePixmap(":/Images/spalsh-portrait.png");

    //get desktop size
    QDesktopWidget* desktop = QApplication::desktop();
    const QRect desktopRect = desktop->availableGeometry();

    QSplashScreen *splash;

    //Scale splash image to fit screen size:

    //create a resized version of image
    QPixmap resizedPixmap (basePixmap.scaledToWidth(desktopRect.width()));

    //create a background empty rectangle
    QPixmap splashPixmap(desktopRect.width(), desktopRect.height());
    splashPixmap.fill(QColor("black"));

    //put our image in the middle of background
    QPainter p;
    p.begin(&splashPixmap);
    QRect targetRect((splashPixmap.width() - resizedPixmap.width())/2,
                     (splashPixmap.height() - resizedPixmap.height())/2,
                     resizedPixmap.width(), resizedPixmap.height());
    p.drawPixmap(targetRect, resizedPixmap);
    p.end();

    splash = new QSplashScreen(splashPixmap);


    splash->show();
    //splash->showMessage("Loading Orayta...", Qt::AlignLeft, Qt::white);

    // fix for certain devices which don't support hebrew chars well.
    app.setFont(QFont("DejaVu Sans"));

    MobileApp m;

    QApplication::setStyle(new QPlastiqueStyle);

    m.show();

    app.processEvents();

    //Hide slpash screen
    splash->finish(&m);
#endif

    return app.exec();
}
