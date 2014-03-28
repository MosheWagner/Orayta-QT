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
* Authors:
* Izar <izar00@gmail.com>
* Moshe Wagner. <moshe.wagner@gmail.com>
*/





/*TODO:
 *
  - Make font sizes consistant...
  - Crashes on exit (sometimes)
  - Exit always takes a long time
  - Can't navigate from links from gmara (swipes dont work)

*/


// testing qml !!@@%%
//#include <QtDeclarative/QDeclarativeView>
//#include <QVBoxLayout>
//#include <QtGui/QGuiApplication>
//#include <QQuickView>









#include "mobileapp.h"
#include "ui_mobileapp.h"
#include "../OraytaBase/functions.h"
#include "../OraytaBase/booklist.h"
#include "../OraytaBase/search.h"
#include <QMessageBox>
#include <QDebug>
#include <QFile>
#include <QDir>
#include <QCloseEvent>
#include <QSettings>
#include <QListWidget>
#include <QListWidgetItem>
#include <QTimer>
#include <QDesktopWidget>
#include <QEasingCurve>
#include <QMenu>
#include <QScroller>
#include <QClipboard>

#ifdef Q_OS_ANDROID
    #include <jni.h>
    #include "jnifunc.h"
#endif

#include "../OraytaBase/quazip/quazipfile.h"

// Global
#define DEF_FONT "Droid Sans Hebrew Orayta"

QString gFontFamily = DEF_FONT;
int gFontSize = 0;

MobileApp::MobileApp(QWidget *parent) :QDialog(parent), ui(new Ui::MobileApp)
{
#ifdef TIMEDBG
    qDebug() << "Mobile start" << QTime::currentTime();
#endif

    //Set all QString to work with unicode
    QTextCodec::setCodecForLocale(QTextCodec::codecForName("utf8"));

    //set the display to native android look.
    //QApplication::setStyle(new QAndroidStyle()); //dosnt work

    ui->setupUi(this);

#ifdef TIMEDBG
    qDebug() << "setupUI"<<QTime::currentTime();
#endif

    //show the about page while app loads
    ui->gtoHelp->hide();

    ui->stackedWidget->setCurrentIndex(ABOUT_PAGE);

    QApplication::processEvents();

    timer.start();

    //set stuff as null only for pertection
    viewHistory = NULL;
    listdownload = NULL;
    downloader = NULL;
    menu = NULL;
    bm = NULL;
    action = NULL;
    autoInstallKukBooksFlag=false;

    //Initialize a new FileDownloader to download the list
    listdownload = new FileDownloader();
    connect(listdownload, SIGNAL(done()), this, SLOT(listDownloadDone()));
    //Initialize a new FileDownloader object for books downloading
    downloader = new FileDownloader();


    autoInstallKukBooksFlag=false;

    //Initialize the bookdisplayer object
    displayer = new textDisplayer(this, &bookList);
    ui->displayArea->layout()->addWidget(displayer);
    ui->displayArea->layout()->addWidget(ui->loadBar);

    //Initialize bookFind page
    bookFindDialog = new bookfind(this, bookList);
    ui->findBook->layout()->addWidget(bookFindDialog);
    connect(bookFindDialog, SIGNAL(openBook(int)), this, SLOT(showBook(int)));


    connect(displayer, SIGNAL(sourceChanged(QUrl)), this, SLOT(titleUpdate(QUrl)));

    connect(displayer, SIGNAL(loadStart()), this, SLOT(tdloadStarted()));
    connect(displayer, SIGNAL(loadEnd(QUrl, Book*, BookIter)), this, SLOT(tdloadFinished(QUrl, Book*, BookIter)));


    exclude << SETTINGS_PAGE << MIXED_SELECTION_PAGE ;//<< HISTORY_PAGE;
    viewHistory = new QList<int>;
    //The base of history should always point to the main page
    viewHistory->append(MAIN_PAGE);
    connect(ui->stackedWidget, SIGNAL(currentChanged(int)), this, SLOT(viewChanged(int)));

    // setup the search page
    showHideSearch(false);

    QList<QWidget *> addscroll;
    addscroll << displayer << ui->treeWidget << ui->SearchTreeWidget << ui->staticBookMarkList << ui->dailyLearningList << ui->historyBookmarkList << ui->scrollArea;

#ifdef TIMEDBG
    qDebug() << "UI stuff 1"<<QTime::currentTime();
#endif


    foreach (QWidget * w, addscroll)
    {
        QScroller::grabGesture(w, QScroller::LeftMouseButtonGesture);

        QScroller* s = QScroller::scroller(w);
        QScrollerProperties p = s->scrollerProperties();
        p.setScrollMetric(QScrollerProperties::HorizontalOvershootPolicy, QScrollerProperties::OvershootAlwaysOff);
        p.setScrollMetric(QScrollerProperties::VerticalOvershootPolicy, QScrollerProperties::OvershootAlwaysOff);
        p.setScrollMetric(QScrollerProperties::OvershootDragResistanceFactor,  0.9);
        p.setScrollMetric(QScrollerProperties::OvershootDragDistanceFactor,  0);
        p.setScrollMetric(QScrollerProperties::OvershootScrollDistanceFactor,  0);

        p.setScrollMetric(QScrollerProperties::DragStartDistance,   0.001 );
        p.setScrollMetric(QScrollerProperties::ScrollingCurve, QEasingCurve::Linear );
        p.setScrollMetric(QScrollerProperties::AxisLockThreshold, 0.9);

        //Does this help?
        //p.setScrollMetric(QScrollerProperties::SnapTime, 1);
        //p.setScrollMetric(QScrollerProperties::SnapPositionRatio, 1);

        p.setScrollMetric(QScrollerProperties::AcceleratingFlickSpeedupFactor, 2.5);
        p.setScrollMetric(QScrollerProperties::AcceleratingFlickMaximumTime, 1.25);

        s->setScrollerProperties(p);
    }

#ifdef TIMEDBG
    qDebug() << "Qscroller"<<QTime::currentTime();
#endif


    //Build the book list
    reloadBooklist();

#ifdef TIMEDBG
    qDebug() << "Booklist" <<QTime::currentTime();
#endif


    //Connect slots to the signalls of the book downloader
    connect(downloader, SIGNAL(done()), this, SLOT(downloadDone()));
    connect(downloader, SIGNAL(downloadProgress(int)), this, SLOT(downloadProgress(int)));
    connect(downloader, SIGNAL(downloadError()), this, SLOT(downloadError()));


    ui->downloadPrgBar->hide();

    //IZAR
    // hack to enable me to test downloads without internet
    // listDownloadDoneOverride();

    // the default for the menu in display page is hidden.
    ui->dispalyMenu->hide();

#ifdef TIMEDBG
    qDebug() << "UI2"<<QTime::currentTime();
#endif



    //load saved settings
    setupSettings();

#ifdef TIMEDBG
    qDebug() << "Settings" <<QTime::currentTime();
#endif


    adjustToScreenSize();

#ifdef TIMEDBG
    qDebug() << "Screensize" <<QTime::currentTime();
#endif

    setupBookmarkList();

#ifdef TIMEDBG
    qDebug() << "Bm list" <<QTime::currentTime();
#endif

    connect (ui->staticBookMarkList, SIGNAL(shortPress(QListWidgetItem*)), this, SLOT(BMShortClicked(QListWidgetItem*)));
    connect (ui->dailyLearningList, SIGNAL(shortPress(QListWidgetItem*)), this, SLOT(BMShortClicked(QListWidgetItem*)));
    connect (ui->historyBookmarkList, SIGNAL(shortPress(QListWidgetItem*)), this, SLOT(BMShortClicked(QListWidgetItem*)));

    QApplication::processEvents();

//    initCrypterRequest();

    //Reopen last book, if relevant
    //get last open book
    QSettings settings("Orayta", "SingleUser");
    settings.beginGroup("History");
        int page = settings.value("lastPage").toInt();
        int lastBookId = settings.value("lastBook").toInt();
        Book *b = bookList.findBookById(lastBookId);
        BookIter itr = BookIter::fromEncodedString(settings.value("position", "").toString());
        lastViewPosition =  settings.value("viewposition").toInt();
    settings.endGroup();

    // restore the bookmark list
    ui->historyBookmarkList->loadHistory(bookList);
    ui->staticBookMarkList->loadHistory(bookList);

#ifdef TIMEDBG
    qDebug() << "last book" <<QTime::currentTime();
#endif


    if (page != DISPLAY_PAGE || !b) ui->stackedWidget->setCurrentIndex(MAIN_PAGE);
    else
    {
        showBook(b, itr);

        //Yuchy hack. but I culdn't get it to work otherwise...
        QTimer::singleShot(100, this, SLOT(jumpToLastPos()));
    }



    ui->gtoHelp->show();
    ui->bmLBL->hide();

    ui->stackedWidget->currentWidget()->setFocus();


#ifdef TIMEDBG
    qDebug() << "Jump" <<QTime::currentTime();
#endif


    adjustToScreenSize();

#ifdef TIMEDBG
    qDebug() << "Adjust 2" <<QTime::currentTime();
#endif


    //displayKukaytaMessage();
}

void MobileApp::resizeEvent(QResizeEvent *)
{
    adjustToScreenSize();
}


//Adjust UI properties depending on device screen size
void MobileApp::adjustToScreenSize()
{
    QDesktopWidget* desktop = QApplication::desktop();
    QSize size = desktop->availableGeometry().size();
// hack to enable desktop usage as well as
#ifndef Q_OS_ANDROID
    size = this->size();
#endif

    //Mazimize the app
    resize(size);

    //Adjust main page icons:
    int w = (size.width() / 2);
    int max = 1000;

    //Round to the closest 10
    w = int(w / 10) * 10;

    ui->gridLayout->setVerticalSpacing(w / 80);
    ui->gridLayout->setHorizontalSpacing(w / 80);

    QSize a(w -20, w - 20);
    //qDebug() << "Icon size:" << a;

    int h = (size.height() / 15);
    //ui->aboutBTN->setIconSize(a);
    ui->aboutBTN->setMaximumSize(w,h);
    //ui->helpBTN->setIconSize(a);
    ui->helpBTN->setMaximumSize(w,h);

    ui->openBTN->setIconSize(a);
    ui->openBTN->setMaximumSize(w,max);
    ui->searchBTN->setIconSize(a);
    ui->searchBTN->setMaximumSize(w,max);
    ui->getbooksBTN->setIconSize(a);
    ui->getbooksBTN->setMaximumSize(w,max);
    ui->bookMarksBTN->setIconSize(a);
    ui->bookMarksBTN->setMaximumSize(w,max);

    int wth = size.width();
    ui->treeWidget->setColumnWidth(0, wth);
    ui->SearchTreeWidget->setColumnWidth(0, wth);

    // if the font size wasn't set manually by the user, we will geuss the best values
    // depending on target device dpi

    if (gFontSize < 1)
    {

        int dpix = desktop->physicalDpiX();
        int dpiy = desktop->physicalDpiY();
        int dpi = (dpix+dpiy)/2;

//        qDebug() << "x: " << dpix << " y: " << dpiy << " avrage: " << dpi;


        //IZAR: this is a guess that must be tested deeper.
        int fontSize = 16;
        if (dpi >= 150) fontSize = 24;
        if (dpi >= 200) fontSize = 28;
        if (dpi >= 250) fontSize = 36;
        if (dpi >= 300) fontSize = 48;
        if (dpi >= 400) fontSize = 58;

        gFontSize = fontSize;

        QSettings settings("Orayta", "SingleUser");
        settings.beginGroup("Confs");
        //save current font settings
        settings.setValue("fontsize", gFontSize);
        settings.endGroup();
    }

    adjustFontSize();

}

int MobileApp::getAutoFontSize()
{
    QDesktopWidget* desktop = QApplication::desktop();
    int dpix = desktop->physicalDpiX();
    int dpiy = desktop->physicalDpiY();
    int dpi = (dpix+dpiy)/2;

    //IZAR: this is a guess that must be tested deeper.
    int fontSize = gFontSize /1.4;
    if (dpi >= 150) {
        fontSize = gFontSize/1.8;
    }
    if (dpi >= 200) {
        fontSize = gFontSize/2;
    }
    if (dpi >= 250) {
        fontSize = gFontSize/2.4;
    }
    if (dpi >= 300) {
        fontSize = gFontSize/2.8;
    }

    return fontSize;
}

//set global font size to ui.
void MobileApp::adjustFontSize()
{
    int fontSize = ui->interfaceSizeSpinBox->value();

   if (fontSize < MIN_INTERFACE_SIZE) // font size is 0 (auto) or too small
        fontSize = getAutoFontSize();

    int smallFont = fontSize*0.8;

    // Enable/Disable night mode:
    QString nStyleSheet("");
    if (nightMode) nStyleSheet = "color: #7faf70; background-color:black;";

    QString styleSheet("*{font-size: " +QString::number(fontSize) +"pt;" + nStyleSheet +"}");
    styleSheet += "QLabel#intro_label{font-size: " + QString::number(smallFont) + "pt;}";

     ui->stackedWidget->setStyleSheet(styleSheet);
//     qDebug() <<" styleSheet: " << styleSheet;

}

//Yuchy hack. but I culdn't get it to work otherwise...
void MobileApp::jumpToLastPos()
{
    ui->stackedWidget->currentWidget()->setFocus();
    displayer->setFocus();

    displayer->verticalScrollBar()->setValue(lastViewPosition);
    lastViewPosition = -1;
}

MobileApp::~MobileApp()
{
    delete downloader;
    delete listdownload;

    //delete action;
    if (action) action->deleteLater();
    //delete menu;
    if (menu) menu->deleteLater();

    delete ui;
}

//IZAR
// reload the whole book list and tree
void MobileApp::reloadBooklist(){

    //**TIMER**//
//    qDebug()<< "main timer, elapsed: " << timer_n1.elapsed() << "begin loading books";

    //create a new empty booklist
    bookList = BookList();

    //Refresh book list
    ui->treeWidget->clear();

    //**TIMER**//
//    qDebug()<< "main timer, elapsed: " << timer_n1.elapsed() << "begin build booklist from folder";

    bookList.BuildFromFolder(BOOKPATH);

    //**TIMER**//
//    qDebug()<< "main timer, elapsed: " << timer_n1.elapsed() << "done build from folder";

    if (bookList.empty())
        qDebug()<<"can't find books in: " << BOOKPATH;

    //**TIMER**//
//    qDebug()<< "main timer, elapsed: " << timer_n1.elapsed() << "begin check uids";

    // Check all uids
    bookList.CheckUid();

    //**TIMER**//
//    qDebug()<< "main timer, elapsed: " << timer_n1.elapsed() << "end check uids";

    QSettings settings("Orayta", "SingleUser");

    //Load books settings
    foreach (Book *book, bookList)
    {
        if (!book || book->getUniqueId() == -1)
            continue;

        settings.beginGroup("Book" + stringify(book->getUniqueId()));
        //default is to show no commentaries
        book->showAlone = settings.value("ShowAlone", true).toBool();
        //int n = settings.value("MixedDisplayes", 0).toInt();
        int n = book->mWeavedSources.size();
        //start from 1, ignore first source which shold always be shown.
        for (int j=1; j<n; j++)
        {
            book->mWeavedSources[j].show = settings.value("Shown" + stringify(j), false).toBool();
        }

        settings.endGroup();
    }

    //**TIMER**//
//    qDebug()<< "main timer, elapsed: " << timer_n1.elapsed() << "4";

    bookList.displayInTree(ui->treeWidget, false);

    //if booklist has changed, reset also the books in search tree
    resetSearchBookTree();

    //**TIMER**//
//    qDebug()<< "main timer, elapsed: " << timer_n1.elapsed() << "done loading books";

}

//IZAR
//when going to 'books in search' page, reset the page
void MobileApp::resetSearchBookTree(){

//    QTime time;
//    time.start();
    ui->SearchTreeWidget->clear();
//    booksInSearch.BuildFromFolder(BOOKPATH);
    booksInSearch = BookList(bookList);
    booksInSearch.displayInTree(ui->SearchTreeWidget, true, true);
//    qDebug() << "load time: "<< time.elapsed();

}


void MobileApp::on_openBTN_clicked()
{
    ui->stackedWidget->setCurrentIndex(LIST_PAGE);
}

void MobileApp::on_searchBTN_clicked()
{
    ui->stackedWidget->setCurrentIndex(SEARCH_PAGE);
}



void MobileApp::on_getbooksBTN_clicked()
{
    ui->stackedWidget->setCurrentIndex(GET_BOOKS_PAGE);
}

void MobileApp::on_aboutBTN_clicked()
{
    ui->stackedWidget->setCurrentIndex(ABOUT_PAGE);
}

void MobileApp::on_treeWidget_clicked(const QModelIndex &index)
{
    //This is a little hack to prevent double events on some android machines and emulators.
    //If the function is called again in less than 2 ms, the second time is ignored.
    qint64 miliSec = timer.restart();
    if (miliSec < 200) return;

    if (ui->treeWidget->isExpanded(index)) ui->treeWidget->collapse(index);
    else ui->treeWidget->expand(index);
}


void MobileApp::on_openBook_clicked()
{
    if ( ui->treeWidget->currentItem() == 0)
        return;
    Book *b = bookList.findBookByTWI(ui->treeWidget->currentItem());
    if (!b->IsDir()) showBook(b);
}


void MobileApp::on_treeWidget_itemDoubleClicked(QTreeWidgetItem *item, int column)
{
    Book *b = bookList.findBookByTWI(item);

    if (!b) return;

    if (!b->IsDir()) showBook(b);
}

void MobileApp::on_treeWidget_itemClicked(QTreeWidgetItem *item, int column)
{
    //do the same thing as when item double clicked
    on_treeWidget_itemDoubleClicked(item, column);
}

void MobileApp::showBook(Book *book, BookIter itr)
{

    //IZAR: temporary work-around. the problem is that orayta reads the global font settings ONLY on startup, and is careless if it is changed latter.
    //TODO: fix this.
    QString fontname= useCustomFontForAll? gFontFamily: DEF_FONT;
    fontname = (book->hasNikud || book->hasTeamim)? gFontFamily: fontname;
    QFont font( fontname, gFontSize );
    book->setFont(font);


    // display mixed selection button only if the book has commentaries
    ui->mixedSelectBTN->setEnabled(book->IsMixed());

    // qml test !!!@@@###%%%



//    QQuickView *viewer = new QQuickView();
//    if (viewer) {
//    viewer->setSource(QUrl("qrc:/testqml.qml"));
//   this->createWindowContainer(viewer);
//        viewer->show();
//    QWidget *container = QWidget::createWindowContainer(viewer, this);
//        container->setMinimumSize(200, 200);
//        container->setMaximumSize(200, 200);
//        container->setFocusPolicy(Qt::TabFocus);
    //QWidget * empty = new QWidget;
//        QLayout* ly = ui->displayArea->layout();
//        ly->removeWidget(displayer);
//        ly->addWidget(container);
//    this->layout()->addWidget(viewer);
//    }


// testing qml !!@@%%
//    QDeclarativeView *qmlView = new QDeclarativeView;
//     qmlView->setSource(QUrl::fromLocalFile("./testqml.qml"));
//    qmlView->setSource(QUrl("qrc:/testqml.qml"));

//     QWidget *widget = myExistingWidget();
  //QVBoxLayout *layout = new QVBoxLayout(this);

          //   layout->addWidget(qmlView);
//     this->layout()->addWidget(qmlView);


    //IZAR TODO: find a way

    switch ( book->fileType() )
    {
        case ( Book::Normal ):
        {
    #ifdef KOOKITA
            initRequest();
            //book is kukayta book but kukayta isn't installed
            if ((! isKukaytaInstalled()) && book->isEncrypted){
                //display install kukayta page
                ui->stackedWidget->setCurrentIndex(KUKAYTA_PAGE);
                ui->kukaytaArea->setCurrentIndex(0);
                return;
            }
    #endif

            ui->stackedWidget->setCurrentIndex(DISPLAY_PAGE);
            qApp->processEvents();

            //book->readBook(1);
            ui->bookNameLBL->setText(book->getNormallDisplayName());
            displayer->display(book, itr);


            break;
        }
        case ( Book::Html ):
        {
            ui->stackedWidget->setCurrentIndex(DISPLAY_PAGE);
            qApp->processEvents();
            //QFile f(book->getPath());
            //ui->textBrowser->setHtml( f.readAll() );
            displayer->setSource(QUrl::fromLocalFile(book->getPath()));
            ui->bookNameLBL->setText(book->getName());

            break;
        }
    /*
      //@@@@@@@@
        case ( Book::Pdf ):
        {
            //TODO: Add poppler support?
            wview->page()->settings()->setAttribute(QWebSettings::PluginsEnabled, true);
            wview->setHtml(pluginPage(book->getNormallDisplayName()));
            ui->stackedWidget->setCurrentIndex(DISPLAY_PAGE);
            if ( wview->page()->mainFrame()->evaluateJavaScript("testPdfPlugin()").toString() == "yes" )
            {
                wview->load( QUrl::fromLocalFile( "file:///" + book->getPath() ) );
                //TODO: title
                //ui->viewTab->setTabText(CURRENT_TAB, book->getNormallDisplayName());
            }
            break;

        }*/
        case ( Book::Link ):
        {
            ui->stackedWidget->setCurrentIndex(DISPLAY_PAGE);
            qApp->processEvents();

            //Process link:

            //Read link file
            QList <QString> t;
            ReadFileToList(book->getPath(), t, "UTF-8");

            //Find the id of the book the link points to
            int lId = -1;
            for (int i=0; i<t.size(); i++)
            {
                int p = t[i].indexOf("Link=");
                if (p != -1) ToNum(t[i].mid(p + 5), &lId);
            }

            if (lId != -1) showBook( bookList.findBookById(lId) );
            else qDebug("Invalid link!");

            break;
        }
    }


}

void MobileApp::showBook(Book *book)
{
    if (!book)
    {
        qDebug() << "Error! Can't open book";
        return;
    }

    showBook(book, BookIter());

}

void MobileApp::showBook(int id)
{
    qDebug() << "open book " << id;
    Book *b= bookList.findBookById(id);
    showBook(b);
}


void MobileApp::tdloadFinished(QUrl u, Book* book, BookIter iter)
{
   QApplication::processEvents();

   titleUpdate(u);
   displayer->setEnabled(true);
   ui->loadBar->hide();

    //IZAR- add a bookmark in new location
   // this seems now to be unnecesary, beacuase we save bookmark when we leave the displayer page.
//   if (book)
//   {
//       addBookMark(book, iter);
//   }
}

void MobileApp::tdloadStarted()
{
    ui->loadBar->show();
    displayer->setEnabled(false);
    QApplication::processEvents();
}

void MobileApp::titleUpdate(QUrl u)
{
//    qDebug() << "titleupdate. url: " << u;
    QString titleTail = displayer->getCurrentIter().humanDisplay();
    QString titleHead, title = "";

    if (displayer && displayer->getCurrentBook()){
        titleHead= displayer->getCurrentBook()->getNormallDisplayName();
        // add separator
        titleHead   += " | ";
    }

    //if titleTail is empty we may be at a search results page. TODO: find a way to really check and not only guess.
    if (u.toString().contains("SEARCH")) { title = tr("Search results"); }
    else {title = titleHead + titleTail;}

    ui->bookNameLBL->setText(title);
}


//Overrides the normal "closeEvent", so it can save tha window's state before quiting
void MobileApp::closeEvent(QCloseEvent *event)
{
    qDebug()<<"close event";

    //Cancel any running searches
    stopSearchFlag = true;

    saveSettings();

    //Delete the old downloadable-books list
    QFile f(SAVEDBOOKLIST);
    if (f.exists()) f.remove();

    ClearTmp();

    QDialog::close();
}

// store all settings from the app
void MobileApp::saveSettings(){

    qDebug()<<"saving global settings...";

    QSettings settings("Orayta", "SingleUser");

    //remmeber last open book
    settings.beginGroup("History");
    settings.setValue("lastPage", ui->stackedWidget->currentIndex());

    if (displayer->getCurrentBook()) settings.setValue("lastBook", displayer->getCurrentBook()->getUniqueId());

    settings.setValue("position", displayer->getCurrentIter().toEncodedString());
    settings.setValue("viewposition", displayer->getVpos());
    settings.endGroup();

    /*
    // This takes way too long!

    foreach (Book *book, bookList)
    {
        if (book->getUniqueId() == -1 || book->hasRandomId)
            continue;

        settings.beginGroup("Book" + stringify(book->getUniqueId()));
        settings.setValue("ShowAlone", book->showAlone);
        for (int j=1; j<book->mWeavedSources.size(); j++)
        {
            settings.setValue("Shown" + stringify(j), book->mWeavedSources[j].show);
        }
        settings.setValue("InSearch", book->IsInSearch());
        settings.endGroup();
    }
    */

    addBookMark(displayer->getCurrentBook(), displayer->getCurrentIter(), displayer->getVpos());
        ui->staticBookMarkList->saveSettings();
        ui->historyBookmarkList->saveSettings();


    qDebug()<<"done saving settings.";
}

//Remove all temporary html files the program created
void MobileApp::ClearTmp()
{
    QDir dir;
    QStringList list;
    QFile f;

    //remove all html rended files in temp path
    dir = QDir(TMPPATH);
    list = dir.entryList(QStringList("*.html"));
    for (int i=0; i<list.size(); i++)
    {
        f.remove(dir.absoluteFilePath(list[i]));
    }
}


void MobileApp::keyPressEvent(QKeyEvent *keyEvent)
{
    //We use the press event here, so that auto-repeat works
    switch ( keyEvent->key() )
    {
        //case Qt::Key_U:
        case Qt::Key_VolumeUp:
            displayer->verticalScrollBar()->setValue(displayer->verticalScrollBar()->value() - 60);
            break;
        //case Qt::Key_D:
        case Qt::Key_VolumeDown:
            displayer->verticalScrollBar()->setValue(displayer->verticalScrollBar()->value() + 60);
            break;
    }
}


void MobileApp::keyReleaseEvent(QKeyEvent *keyEvent){

    switch ( keyEvent->key() )
    {

    // onPause event from android
    case Qt::Key_MediaTogglePlayPause:
    case Qt::Key_MediaPlay:
        saveSettings();
        break;

    /*
    //stop event sent from android. exit app
    case Qt::Key_MediaStop:
        qDebug()<< "android stop request";

        // if autoResume selected by user, dont terminate app.
        if (!autoResume)  close();
        break;
    */

    //back button was clicked
    case Qt::Key_Close:
    case Qt::Key_MediaPrevious:
    case Qt::Key_Back:
        goBack();
        break;

    //ctrl + backspace clicked , go back.
    case Qt::Key_Backspace:
        if (keyEvent->modifiers() == Qt::CTRL) goBack();
        break;

        return;


    //Test different screen sizes:
    case Qt::Key_0:
        resize(QSize(240,300));
        adjustToScreenSize();
        break;
    case Qt::Key_1:
        resize(QSize(240,380));
        adjustToScreenSize();
        break;
    case Qt::Key_2:
        resize(QSize(240,412));
        adjustToScreenSize();
        break;
    case Qt::Key_3:
        resize(QSize(320,460));
        adjustToScreenSize();
        break;
    case Qt::Key_4:
        resize(QSize(480,780));
        adjustToScreenSize();
        break;
    case Qt::Key_5:
        resize(QSize(480,854));
        adjustToScreenSize();
        break;

    //menu button was clicked
    case Qt::Key_TopMenu:
    case Qt::Key_Menu:
    case Qt::Key_Explorer:
    case Qt::Key_Meta:
    case Qt::Key_Super_L:
        showMenu();
        break;

    default:
        qDebug() << "unknown key pressed: " << keyEvent->key();
        QDialog::keyReleaseEvent(keyEvent);
    }

    keyEvent->accept();
}

void MobileApp::mouseReleaseEvent(QMouseEvent *ev){
    if (ev->button() == Qt::RightButton) showMenu();

    QDialog::mouseReleaseEvent(ev);
}

// when menu butten clicked, show the menu
void MobileApp::showMenu()
{
//    qDebug() << "show menu";
    switch (ui->stackedWidget->currentIndex())
    {
    case DISPLAY_PAGE:
        //show/hide the toolbar menu
        if (ui->dispalyMenu->isVisible())
            ui->dispalyMenu->hide();
        else
        {
            QToolButton * idxBtn = ui->toIndexMenuBTN;
            if (displayer->isLastSearch())
            {
                idxBtn->setText(tr("Back to results"));
                //shrink font to fit button
                idxBtn->setFont(QFont(idxBtn->font().family(), 7));
            }
            else
            {
                idxBtn->setText(tr("Index"));
                idxBtn->setFont(QFont(idxBtn->font().family(), 9));
            }

            ui->dispalyMenu->show();
        }
        return;

    case MAIN_PAGE:
        //go to the main setting page
        ui->stackedWidget->setCurrentIndex(SETTINGS_PAGE);
        return;

    //TODO: set more options
    default:
        ui->stackedWidget->setCurrentIndex(SETTINGS_PAGE);
        return;
    }
}

// stacked widget currnet view canged.
void MobileApp::viewChanged(int index)
{

    //IZAR
    // this is a list of things to do when we go to a certain page
    switch (index){
    //when going to books in search page, reset the page

    //when going to get books page get the list from server.
    case (GET_BOOKS_PAGE):
        downloadBookList();
        break;

    case (SETTINGS_PAGE):
        resetSettingsPage();
        //don't put this page in history
//        return;
        break;

    //when going to commentary selection page, reset it.
    case (MIXED_SELECTION_PAGE):
        setupMixedSelection();
        //don't put this page in history
//        return;
        break;
    }

    if(!viewHistory)
    {
        qDebug()<< "cant stat view history";
        return;
    }
    

    //add this page to history
    if (viewHistory->size() == 0) viewHistory->append(index);
    else
    {
        int previousPage = viewHistory->at(viewHistory->size()-1);

        // if we came from the displayer page then we want to save the scrolled location
        if (previousPage == DISPLAY_PAGE)
        {
            Book* b= displayer->getCurrentBook();
            BookIter itr = displayer->getCurrentIter();
            if (b)
            {
                int viewPosition = displayer->getVpos();
                addBookMark(b, itr, viewPosition);
            }
        }

        // never add the same page twice
        if (previousPage != index)
            // ignore pages we want to exclude
            if (!exclude.contains(index))
                viewHistory->append(index);
    }
}

//go to previous view of stacked widget.
void MobileApp::goBack()
{
//    qDebug()<< "Back pressed";

    if (ui->stackedWidget->currentIndex() == MAIN_PAGE)
    {
        qDebug()<< "Exiting!";
        close();
    }

    // if we have only one object it probably is the current view and we can only go back to the main page
    else if(viewHistory->size() <= 1)
    {
        ui->stackedWidget->setCurrentIndex(MAIN_PAGE);
        return;
    }

    int currentId = ui->stackedWidget->currentIndex();

    if (currentId == DISPLAY_PAGE)
    {
        // if the menu is displayed then hide it
        if (ui->dispalyMenu->isVisible())
        {
            ui->dispalyMenu->hide();
            return;
        }

        //If we are at the index or the search page
        if (displayer->source().path().indexOf("Index") != -1 || displayer->source().path().indexOf("SEARCH") != -1)
        {
            //do nothing here. we'll get to the next statement and execute there.
//            int id = viewHistory->at(viewHistory->size()-2);
//            viewHistory->removeLast();
//            ui->stackedWidget->setCurrentIndex(id);
        }
        else
        {
            if (displayer->isLastSearch())
            {
                displayer->backward();
            }
            else
            {
                //save bookmark at previous location
                if(displayer->getCurrentBook())
                {
                    addBookMark(displayer->getCurrentBook(), displayer->getCurrentIter(), displayer->getVpos());
                }

                displayer->goToIndex();
            }
            return ;
        }
    }
//    else
    {

            int id = viewHistory->at(viewHistory->size()-1);
            // if we are at a excluded page, this means that it isn't in wiewHistory, in which case we should go to the last item in the list
            if (id == currentId)
            {
                if (viewHistory->size() > 0) viewHistory->removeLast();

                // no where to go back to.
                if (viewHistory->size() <= 0) { qDebug()<< "Nowhere to go. exiting."; close(); return;}

                id = viewHistory->at(viewHistory->size()-1);
            }
            if (viewHistory->size() > 0) viewHistory->removeLast();
            ui->stackedWidget->setCurrentIndex(id);

            return;
    }

    //If we got 'till here then:
    qDebug()<< "Nowhere to go. exiting.";
    close();
}


void MobileApp::on_toIndexMenuBTN_clicked()
{
    if (displayer->isLastSearch())
    {
        displayer->backward();
    }
    else
    {
        displayer->goToIndex();
    }

    //ui->textBrowser->scrollToAnchor("Top");
    //wview->page()->mainFrame()->scrollToAnchor("Top");
}


void MobileApp::on_saveConf_clicked()
{
    //Save font
    gFontFamily = ui->fontComboBox->currentFont().family();
    gFontSize = ui->fonSizeSpinBox->value();

    useCustomFontForAll = ui->customFontRDBTN->isChecked();
    nightMode = ui->NightModeCKBX->isChecked();

    ui->saveConf->setEnabled(false);

    //Change language if needed
    QSettings settings("Orayta", "SingleUser");
    settings.beginGroup("Confs");

    //save current font settings
    settings.setValue("fontfamily", gFontFamily);
    settings.setValue("fontsize", gFontSize);

    settings.setValue("useCustomFontForAll", useCustomFontForAll);

    settings.setValue("nightMode", nightMode);

    settings.setValue("inetrfaceSize", ui->interfaceSizeSpinBox->value());

    /* disabled
    //Change language if needed
    settings.setValue("systemLang",ui->systemLangCbox->isChecked());
    if (ui->systemLangCbox->isChecked())
    {
        LANG = QLocale::languageToString(QLocale::system().language());
    }
    //Use custom language only if "useSystemLang" is not checked
    else
    */

    {
        int i = langsDisplay.indexOf(ui->langComboBox->currentText());
        if (i != -1)
        {
            settings.setValue("lang", langs[i]);
//            settings.endGroup();
            LANG = langs[i];
        }
    }

    settings.endGroup();
//    qDebug() << "Settings: "<<settings.fileName();

//    emit ChangeLang(LANG);
    translate(LANG);

    adjustFontSize();

    //also, clear currently displayed book.

    // test if the previous view was the book itself. if so we want to reload the book.
    if (viewHistory->length() > 1 &&
                      viewHistory->at(viewHistory->length()-1) == DISPLAY_PAGE)
    {
        if (displayer->getCurrentBook()) {
            // remove two last itmes from history. (settings page and dispaly page).
            //viewHistory->removeLast(); viewHistory->removeLast();

            //reload previously shown book
            displayer->getCurrentBook()->setFont(QFont(gFontFamily,gFontSize));
            showBook(displayer->getCurrentBook(), displayer->getCurrentIter());

        }

    }
    else goBack();
}

void MobileApp::on_fontComboBox_currentIndexChanged(const QString &font)
{
    //Settings have changed, so the save button should be enabled
    ui->saveConf->setEnabled(true);

    //Show the new font in the preview box
    ui->fontPreview->setFont(QFont(font, ui->fonSizeSpinBox->value()));
}

void MobileApp::on_horizontalSlider_sliderMoved(int position)
{
    //Show the new font in the preview box
    ui->fontPreview->setFont(QFont(ui->fontComboBox->currentText(), ui->fonSizeSpinBox->value()));
}


void MobileApp::on_fonSizeSpinBox_valueChanged(int size)
{
    //Settings have changed, so the save button should be enabled
    ui->saveConf->setEnabled(true);

    //set the slider to the same value
    ui->horizontalSlider->setValue(size);

    //Show the new font in the preview box
    ui->fontPreview->setFont(QFont(ui->fontComboBox->currentFont().family(), size));
}

void MobileApp::on_interfaceSizeSpinBox_valueChanged(int size)
{
    //Settings have changed, so the save button should be enabled
    ui->saveConf->setEnabled(true);

    if(size>0 && size< MIN_INTERFACE_SIZE)
        ui->interfaceSizeSpinBox->setValue(getAutoFontSize());

    //preffered but might cause trouble
    //adjustFontSize();
}

void MobileApp::on_horizontalSlider_valueChanged(int value)
{
    //set this value to the SpinBox
    ui->fonSizeSpinBox->setValue(value);
    MobileApp::on_fonSizeSpinBox_valueChanged(value);
}



void MobileApp::on_cancelBTN_clicked()
{
//    ui->stackedWidget->setCurrentIndex(MAIN_PAGE);
    goBack();
    resetSettingsPage();
}

//perform search in books
void MobileApp::on_SearchInBooksBTN_released()
{

        //Do search
        QString otxt = ui->searchInBooksLine->text();

        //do nothing if no search text.
        if (otxt.isEmpty()) return;

        QString stxt = otxt;
        QRegExp regexp;

        /*
        if (ui->fuzzyCheckBox->isChecked())
            stxt = AllowKtivHasser(stxt);
        */

        regexp = QRegExp( createSearchPattern (stxt) );
        regexp.setMinimal(true);

        //show the stop button and search bar
        showHideSearch(true);

        QApplication::processEvents();

        QUrl u = SearchInBooks (regexp, otxt, booksInSearch.BooksInSearch(), ui->progressBar);
        displayer->setSource(u);

        //wview->page()->setLinkDelegationPolicy(QWebPage::DelegateAllLinks);
        ui->stackedWidget->setCurrentIndex(DISPLAY_PAGE);

        //done search, reset the ui
        showHideSearch(false);

}

void MobileApp::on_searchInBooksLine_returnPressed() { on_SearchInBooksBTN_released(); }



//Cancel search
void MobileApp::on_stopSearchBTN_clicked()
{
    stopSearchFlag = true;
    showHideSearch(false);
}

// switch the view from normal to in search mode
void MobileApp::showHideSearch(bool inSearch){

    ui->inSearchGroup->setVisible(inSearch);
    ui->SearchInBooksBTN->setVisible(!inSearch);
}

void MobileApp::on_settingsMenuBTN_clicked()
{
    //Copy all
}

void MobileApp::on_SearchTreeWidget_itemClicked(QTreeWidgetItem *item, int column)
{
    //This is a little hack to prevent double events on some android machines and emulators.
    //If the function is called again in less than 2 ms, the second time is ignored.
    qint64 miliSec = timer.restart();
    if (miliSec < 200) return;


    Book* book = booksInSearch.findBookByTWI(item);

    if (!book) {return;}

    if (book->IsInSearch()) book->unselect();
    else book->select();

    // select/unselect childrean
    //if (item->checkState(column) == Qt::Checked) book->select();
    //else book->unselect();

}


//---------------------------------------
//the folowing methods where copied from Settings.cpp, but modified.
//TODO: they shold be implemented in a separate class.

/* disabled because system lang doesnt work on android.
  //maybe we should report a bug and fix this when it is fixed.
void MobileApp::on_systemLangCbox_clicked(bool checked)
{
    // Disable language choosing if the "use system language" is selected
    ui->groupBox->setEnabled(!checked);

    //Settings have changed, so the save button should be enabled
    ui->saveConf->setEnabled(true);
}
*/

void MobileApp::setupSettings(){

    QSettings settings("Orayta", "SingleUser");
    settings.beginGroup("Confs");
        useCustomFontForAll = settings.value("useCustomFontForAll", false).toBool();
        nightMode = settings.value("nightMode", false).toBool();
    settings.endGroup();

    //Set available languages
    //TODO - NOTICE :
    // french disabled because we have no french translation available
    langs << "Hebrew" << "English" /* << "French"*/;
    langsDisplay << tr("עברית") << "English" /*<< tr("Français")*/;

    //Show available languages in the language combobox
    for (int i=0; i<langs.size(); i++)
    {
        ui->langComboBox->addItem(langsDisplay[i]);
    }

//    QSettings settings("Orayta", "SingleUser");

    /* this feature is disabled
    //Check if "use system lang" is set
    settings.beginGroup("Confs");
    bool useSystemLang = settings.value("systemLang",true).toBool();
    settings.endGroup();


    ui->systemLangCbox->setChecked(useSystemLang);
    ui->groupBox->setEnabled(!useSystemLang);
    */

    //Show current language
    int is = -1;
    for (int i=0; i<langs.size(); i++) if (LANG == langs[i]) is = i;
    ui->langComboBox->setCurrentIndex(is);

    //get stored settings for display font
    settings.beginGroup("Confs");
        QString defaultFont = "Droid Sans Hebrew Orayta";
        gFontFamily = settings.value("fontfamily", defaultFont).toString();

        //default set to 0. if it is so, adjustToScreenSize() will guess a better value depending on target screen dpi.
        gFontSize = settings.value("fontsize",0).toInt();

        int interfaceSize = settings.value("inetrfaceSize", 0).toInt();
        ui->interfaceSizeSpinBox->setValue(interfaceSize);
    settings.endGroup();

    resetSettingsPage();

    //as default display the font selection page int the settings page
    ui->tabWidget->setCurrentIndex(0);
}

void MobileApp::resetSettingsPage()
{
    //Show current font values in the
    QFont f (gFontFamily, gFontSize);
    ui->fontPreview->setFont(f);
    ui->fontComboBox->setCurrentFont(f);
    ui->fonSizeSpinBox->setValue(gFontSize);
    ui->horizontalSlider->setValue(gFontSize);
    ui->NightModeCKBX->setChecked(nightMode);
    ui->customFontRDBTN->setChecked(useCustomFontForAll);
    ui->defaultFontRDBTN->setChecked(!useCustomFontForAll);
    ui->saveConf->setEnabled(false);

}


//------------------------------------

//copied from desktopapp
void MobileApp::translate(QString newlang)
{
    LANG = newlang;

    extern QTranslator *translator;

    //Remove old translator
    QApplication::removeTranslator(translator);

    //English needs no translator, it's the default
    if (LANG != "English")
    {
        if (!translator->load(LANG + ".qm", ".")) translator->load(LANG + ".qm", MAINPATH);
        QApplication::installTranslator(translator);
    }

    ui->retranslateUi(this);

//    if (LANG == "Hebrew") setDirection(true);
//    else setDirection(false);
}

void MobileApp::on_langComboBox_currentIndexChanged(const QString &arg1)
{
    //Settings have changed, so the save button should be enabled
    ui->saveConf->setEnabled(true);
}

void MobileApp::on_mixedSelectBTN_clicked()
{
    ui->stackedWidget->setCurrentIndex(MIXED_SELECTION_PAGE);
}

void MobileApp::setupMixedSelection(){
    Book* book = displayer->getCurrentBook();

    // no current book or no commentaries available.
    if (!book || !book->IsMixed()){
        ui->noCommemtaries->show();
        ui->mixedGroup->hide();

        return;
    }
    else if (book->IsMixed())
    //Show / Hide mixed display stuff
    {
        ui->mixedGroup->show();
        ui->noCommemtaries->hide();

        //Clear old entries
        ui->selectionArea->clear();

        for(int i=1; i<book->mWeavedSources.size(); i++)
        {
            QListWidgetItem * item = new QListWidgetItem(book->mWeavedSources[i].Title);
            item->setCheckState(book->mWeavedSources[i].show? Qt::Checked : Qt::Unchecked);
            QString id = stringify(book->mWeavedSources[i].id);
            item->setWhatsThis(id);
            item->setToolTip(item->checkState() == Qt::Checked? "True" : "False");
            ui->selectionArea->addItem(item);

            // if the book isn't installed
            QFile f (book->mWeavedSources[i].FilePath);
            if (!f.exists())
            {
                item->setText(item->text() + tr(" (Not installed. please install it from 'Get books' page)"));
                item->setTextColor(QColor("gray"));
                item->setCheckState(Qt::PartiallyChecked);
            }
        }
    }
}

void MobileApp::on_openMixed_clicked()
{
    if (!displayer->getCurrentBook()) return;
    Book* b = displayer->getCurrentBook();

    bool showalone =true;

    for(int j=1; j<b->mWeavedSources.size(); j++)
    {
        QString srcId = stringify(b->mWeavedSources[j].id);

        for (int i =0; i< ui->selectionArea->count(); i++)
        {
            QListWidgetItem *item = ui->selectionArea->item(i);

              QString widgetId = item->whatsThis();

              if (srcId == widgetId) //we found the currect item
              {
                  bool checked = item->checkState() == Qt::Checked? true : false;

                  //set the showability of this item to what the user chose.
                  b->mWeavedSources[j].show = checked;
                  if (checked) showalone = false;
                  break;
              }
        }
    }

    b->showAlone = showalone;

    // save the settings for this book
    if (b->getUniqueId() != -1 && !(b->hasRandomId)) //book has normal uid
    {
        QSettings settings("Orayta", "SingleUser");
        settings.beginGroup("Book" + stringify(b->getUniqueId()));
        settings.setValue("ShowAlone", b->showAlone);
        for (int j=1; j<b->mWeavedSources.size(); j++)
        {
            settings.setValue("Shown" + stringify(j), b->mWeavedSources[j].show);
        }
        settings.setValue("InSearch", b->IsInSearch());
        settings.endGroup();
    }

    //show the book
    qApp->processEvents();
    showBook(b, displayer->getCurrentIter());

}

void MobileApp::on_markAllBTN_clicked()
{
    //mark all items as checked
    for (int i =0; i< ui->selectionArea->count(); i++)
        ui->selectionArea->item(i)->setCheckState(Qt::Checked);
}

void MobileApp::on_unmarkAllBTN_clicked()
{
    //uncheck all items
    for (int i =0; i< ui->selectionArea->count(); i++)
        ui->selectionArea->item(i)->setCheckState(Qt::Unchecked);
}

void MobileApp::on_markAllBTN_2_clicked()
{
    //mark all items as checked
    for (int i =0; i< ui->downloadListWidget->count(); i++)
        ui->downloadListWidget->item(i)->setCheckState(Qt::Checked);
}

void MobileApp::on_unmarkAllBTN_2_clicked()
{
    //uncheck all items
    for (int i =0; i< ui->downloadListWidget->count(); i++)
    {
        ui->downloadListWidget->item(i)->setCheckState(Qt::Unchecked);
    }
}

void MobileApp::on_markAllBTN_3_clicked()
{
    foreach (Book * book, booksInSearch)
    {
        book->select();
    }
}

void MobileApp::on_unmarkAllBTN_3_clicked()
{
    foreach (Book * book, booksInSearch)
    {
        book->unselect();
    }
}


void MobileApp::on_copyTextBTN_clicked()
{
    //Copy the text of the whole chapter to the clipboard
    QApplication::clipboard()->setText(displayer->toPlainText());
}


void MobileApp::on_selectionArea_itemClicked(QListWidgetItem *item)
{
    //This is a little hack to prevent double events on some android machines and emulators.
    //If the function is called again in less than 2 ms, the second time is ignored.
    qint64 miliSec = timer.restart();
    if (miliSec < 200) return;

    // ignore books that aren't installed:
    if (item->checkState() == Qt::PartiallyChecked) return;

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

/* derecated
void MobileApp::on_moreInfoBTN_clicked()
{
    //show wellcome page
    Book *wellcome = bookList.findBookById(1);

    showBook(wellcome);
}

void MobileApp::on_helpBTN_clicked()
{
    if (LANG.contains( "Hebrew"))
        //show hebrew help
        showBook(bookList.findBookById(2));
    else
       showBook( bookList.findBookById(3));
}
*/

void MobileApp::on_resetBookListBTN_clicked()
{
    ui->treeWidget->collapseAll();
}

void MobileApp::on_lastBookBTN_clicked()
{
    //probably we just loaded the app
    if (!displayer->getCurrentBook())
    {
        //get last open book
        QSettings settings("Orayta", "SingleUser");
        settings.beginGroup("History");
            int lastBookId = settings.value("lastBook").toInt();
            Book *b = bookList.findBookById(lastBookId);
            BookIter itr = BookIter::fromEncodedString(settings.value("position", "").toString());
            int vp =  settings.value("viewposition").toInt();
        settings.endGroup();

        if (!b) return;
        showBook(b, itr);
        displayer->verticalScrollBar()->setValue(vp);
    }
    else
        ui->stackedWidget->setCurrentIndex(DISPLAY_PAGE);
}


void MobileApp::on_gtoHelp_clicked()
{
    //try to show the help page
    Book *helpBook = NULL;
    if (LANG.contains( "Hebrew"))
        //show hebrew help
        helpBook = bookList.findBookById(2);
    else
        helpBook = bookList.findBookById(3);

    if (helpBook)
        showBook(helpBook);
}

void MobileApp::on_backBTN_clicked()
{
    // if there is a location to go backwords to, go there. else, go to previous chapter.
    if (displayer->isBackwardAvailable())
        displayer->backward();
    else
    {
    Book *b = displayer->getCurrentBook();
    BookIter it = displayer->getCurrentIter();
    it = b->prevChap(it);

    if (it != BookIter()) showBook(b, it);
    }
 }

void MobileApp::on_forwardBTN_clicked()
{
   // if there is a location to go forword to, go there. else, go to next chapter.
    if (displayer->isForwardAvailable())
        displayer->forward();
    else
    {
        Book *b = displayer->getCurrentBook();
        BookIter it = displayer->getCurrentIter();
        it = b->nextChap(it);

        //@@@
//        qDebug() << it.toString();

        if (it != BookIter()) showBook(b, it);
    }
}

void MobileApp::on_ZoomInBTN_clicked()
{
    float p = (float) displayer->verticalScrollBar()->value() / displayer->verticalScrollBar()->maximum();

    displayer->increaseSize();

    displayer->verticalScrollBar()->setValue(displayer->verticalScrollBar()->maximum() * p);
}

void MobileApp::on_ZoomOutBTN_clicked()
{
    float p = (float) displayer->verticalScrollBar()->value() / displayer->verticalScrollBar()->maximum();

    displayer->decreaseSize();

    displayer->verticalScrollBar()->setValue(displayer->verticalScrollBar()->maximum() * p);
}

void MobileApp::on_toMainMenuBTN_clicked()
{
    ui->stackedWidget->setCurrentIndex(MAIN_PAGE);
}


void MobileApp::displayKukaytaMessage()
{
    qDebug()<<"is kukayta installed ?";
    qDebug()<<"?: " <<isKukaytaInstalled();
    if (! isKukaytaInstalled())
        return;

    bool firstTimeAfterInstalledKukayta = false;

    QSettings settings("Orayta", "SingleUser");
    settings.beginGroup("Confs");
    firstTimeAfterInstalledKukayta = !settings.contains("kukaytaInstalled");


    if (firstTimeAfterInstalledKukayta)
    {
        ui->stackedWidget->setCurrentIndex(KUKAYTA_PAGE);
        ui->kukaytaArea->setCurrentIndex(1);
    }

    settings.setValue("kukaytaInstalled", true);

    settings.endGroup();
}


void MobileApp::on_helpBTN_clicked()
{
    on_gtoHelp_clicked();
}

void MobileApp::on_findBookBTN_clicked()
{
//    ui->stackedWidget->setCurrentWidget(bookFindDialog);
    ui->stackedWidget->setCurrentIndex(BOOKFIND_PAGE);
    bookFindDialog->Reset();
}

void MobileApp::on_customFontRDBTN_toggled(bool checked)
{ ui->saveConf->setEnabled(true);}

void MobileApp::on_autoResumeCKBX_stateChanged(int arg1)
{ ui->saveConf->setEnabled(true);}

void MobileApp::on_NightModeCKBX_clicked(bool checked)
{ ui->saveConf->setEnabled(true); }

void MobileApp::on_dlKukaytaBooksBTN_clicked()
{
    autoInstallKukBooksFlag=true;
    ui->stackedWidget->setCurrentIndex(GET_BOOKS_PAGE);
    downloadBookList();
}


void MobileApp::on_installKukaytaBTN_clicked()
{
#ifdef KOOKITA
    installKukayta();
#endif
}


void MobileApp::on_dlKukaytaBooksBTN__clicked()
{
    goBack();
}

void MobileApp::on_settingsBTN_clicked()
{
    ui->stackedWidget->setCurrentIndex(SETTINGS_PAGE);
}


