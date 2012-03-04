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
#include "mobileapp.h"
#include "ui_mobileapp.h"

#include "functions.h"
#include "booklist.h"
#include "search.h"
#include <QMessageBox>
#include <QDebug>
#include <QFile>
#include <QCloseEvent>
#include <QWebFrame>
#include <QWebPage>
#include <QDesktopServices>
#include <QMovie>
#include <QListWidget>
#include <QListWidgetItem>
#include <QTimer>

//#include <QScroller>

#define MAIN_PAGE 0
#define ABOUT_PAGE 1
#define DISPLAY_PAGE 2
#define LIST_PAGE 3
#define SEARCH_PAGE 4
#define BOOK_SELECTION_PAGE 5
#define GET_BOOKS_PAGE 6
#define SETTINGS_PAGE 7
#define MIXED_SELECTION_PAGE 8
#define WAIT_PAGE 9


//TODO: CLEAN & SPLIT UP!!!!
//TODO: Test landscape - portrait switching
//TODO: Improve look & feel
//TODO: Bookmarks
//TODO: Improve book loading speed (gradual loading)

//IZAR-
//TODO: load page preview


#include <QKinetic/qtscroller.h>


MobileApp::MobileApp(QWidget *parent) :QDialog(parent), ui(new Ui::MobileApp)
{
    //Set all QString to work with unicode
    QTextCodec::setCodecForCStrings(QTextCodec::codecForName("utf8"));
    QTextCodec::setCodecForTr(QTextCodec::codecForName("utf8"));

    //set stuff as null only for pertection
    currentBook = NULL;
    waitMovie = NULL;
    viewHistory = NULL;
    listdownload = NULL;
    downloader = NULL;
    wview = NULL;


    ui->setupUi(this);

    //show the about page while app loads
    ui->stackedWidget->setCurrentIndex(ABOUT_PAGE);

    QApplication::processEvents();

    QTimer::singleShot(20, this, SLOT(continueConstructor()));
}


#include "QKinetic/flickcharm.h"

// constructor continuation
void MobileApp::continueConstructor()
{
    setupSettings();


    InternalLocationInHtml = "";

    //IZAR
    //setup the wait image
     waitMovie = new QMovie(":/Images/Wait.gif");
//    waitMovie = new QMovie(":/Images/ajax-loader.gif");
//    ui->waitLBL->setMovie(waitMovie);
//    if (waitMovie) waitMovie->start();

    wview = new QWebView(this);


    QObject::connect(wview, SIGNAL(linkClicked(const QUrl &)), this , SLOT(wvlinkClicked(const QUrl &)));
    QObject::connect(wview, SIGNAL(loadFinished(bool)), this , SLOT(wvloadFinished(bool)));
    QObject::connect(wview, SIGNAL(loadStarted()), this , SLOT(wvloadStarted()));

//    wview->setZoomFactor(1.0);


    //setup the back and forword keys in the book display page
    {
        QAction *back = wview->pageAction(QWebPage::Back);
        QAction *forward = wview->pageAction(QWebPage::Forward);
        back->setIcon(QIcon(":/Icons/go-previous.png"));
        forward->setIcon(QIcon(":/Icons/go-next.png"));
        back->setText(tr("back"));
        forward->setText(tr("forward"));

        ui->backBTN->setDefaultAction(back);
        ui->forwardBTN->setDefaultAction(forward);
    }


//    viewHistory = new QWidgetList();
    viewHistory = new QList<int>;
    //the base of history should always point to the main page
    viewHistory->append( MAIN_PAGE);
    connect(ui->stackedWidget, SIGNAL(currentChanged(int)), this, SLOT(viewChanged(int)));


    ui->displayArea->layout()->addWidget(wview);

//    wview->setHtml(tr("<center><big>Loading...</big></center>"));

    wview->page()->setLinkDelegationPolicy(QWebPage::DelegateAllLinks);

    QtScroller::grabGesture(wview, QtScroller::LeftMouseButtonGesture);

    wview->show();

    // setup the search page
    showHideSearch(false);


    FlickCharm *fc = new FlickCharm(this);
    fc->activateOn(ui->treeWidget);
    fc->activateOn(ui->SearchTreeWidget);

    ui->treeWidget->setColumnWidth(0, 800);
    ui->SearchTreeWidget->setColumnWidth(0, 800);

    //Build the book list
    reloadBooklist();


    //Initialize a new FileDownloader to download the list
    listdownload = new FileDownloader();
    connect(listdownload, SIGNAL(done()), this, SLOT(listDownloadDone()));


    //Initialize a new FileDownloader object for books downloading
    downloader = new FileDownloader();

    //Connect slots to the signalls of the book downloader
    connect(downloader, SIGNAL(done()), this, SLOT(downloadDone()));
    connect(downloader, SIGNAL(downloadProgress(int)), this, SLOT(downloadProgress(int)));
    connect(downloader, SIGNAL(downloadError()), this, SLOT(downloadError()));


    ui->downloadGRP->hide();
    ui->downloadPrgBar->hide();


    //IZAR
    // hack to enable me to test downloads without internet
//    listDownloadDoneOverride();


    // the default for the menu in display page is hidden.
    ui->dispalyMenu->hide();


    //Checking if books exist is irelevant. We need to check if the SD card works, but maybe not here...
    QApplication::processEvents();
    ui->stackedWidget->setCurrentIndex(MAIN_PAGE);
}

MobileApp::~MobileApp()
{

    qDebug() << "in destructer";

    //Delete the old downloadable-books list
    QFile f(SAVEDBOOKLIST);
    f.remove();

    // delete books in htmltmp
    ClearTmp();

    delete downloader;
    delete listdownload;
    delete waitMovie;

    delete ui;
}

// *copied from desktop app*
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
#ifndef POPPLER
    //remove all WKP.* files (because pdf plugin not cleaned)
    dir = QDir::currentPath();
    list = dir.entryList(QStringList("WKP.*"));
    for (int i=0; i<list.size(); i++)
    {
        f.remove(dir.absoluteFilePath(list[i]));
    }
#endif
}

//IZAR
// reload the whole book list and tree
void MobileApp::reloadBooklist(){

    //create a new empty booklist
    bookList = BookList();

    //Refresh book list
    ui->treeWidget->clear();

    bookList.BuildFromFolder(BOOKPATH);

    // Check all uids
    bookList.CheckUid();

    QSettings settings("Orayta", "SingleUser");

    //Load books settings
//    for(unsigned int i=0; i<bookList.size(); i++)
    foreach (Book *book, bookList)
    {
        if (!book || book->getUniqueId() == -1)
            continue;

        settings.beginGroup("Book" + stringify(book->getUniqueId()));
        //default is to show no commentaries
            book->showAlone = settings.value("ShowAlone", true).toBool();
//            int n = settings.value("MixedDisplayes", 0).toInt();
            int n = book->mWeavedSources.size();
            //start from 1, ignore first source which shold always be shown.
            for (int j=1; j<n; j++)
            {
                book->mWeavedSources[j].show = settings.value("Shown" + stringify(j), false).toBool();
            }

        settings.endGroup();
    }

    bookList.displayInTree(ui->treeWidget, false);

    //if booklist has changed, reset also the books in search tree
    resetSearchBookTree();

}

//IZAR
//when going to 'books in search' page, reset the page
void MobileApp::resetSearchBookTree(){

    ui->SearchTreeWidget->clear();
    booksInSearch.BuildFromFolder(BOOKPATH);
    booksInSearch.displayInTree(ui->SearchTreeWidget, true);

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

void MobileApp::on_menuHelpBTN_clicked() { on_aboutBTN_clicked() ;}

void MobileApp::on_treeWidget_clicked(const QModelIndex &index)
{
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


void MobileApp::showBook(Book *book)
{
    if (!book)
    {
        qDebug()<< "cant open book";
        return;
    }

    //remember curently displayed book
    currentBook = book;



    //IZAR: temporary work-around. the problem is that orayta reads the global font settings ONLY on startup, and is careless if it is changed latter.
    //TODO: fix this.
    QFont font( gFontFamily, gFontSize );
    book->setFont(font);

    // display mixed selection button only if the book has commentaries
    ui->mixedSelectBTN->setEnabled(book->IsMixed());

    switch ( book->fileType() )
    {
        case ( Book::Normal ):
        {
//            ui->stackedWidget->setCurrentIndex(DISPLAY_PAGE);

            //force view to show loading...
            wvloadStarted();
            qApp->processEvents();

//          ui->stackedWidget->setCurrentIndex(DISPLAY_PAGE);

            book->readBook(1);

            wview->setHtml(book->getBookIndexHtml());

            wview->page()->setLinkDelegationPolicy(QWebPage::DelegateAllLinks);

//          booktitle = book->getNormallDisplayName();
            break;
        }
        case ( Book::Html ):
        {
            wview->load( QUrl::fromLocalFile(book->getPath()) );
            wview->page()->setLinkDelegationPolicy(QWebPage::DelegateExternalLinks);
            //TODO: title
//            ui->stackedWidget->setCurrentIndex(DISPLAY_PAGE);
            break;
        }
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
        }
        case ( Book::Link ):
        {
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

void MobileApp::wvloadFinished(bool ok)
{
//    if (ui->waitLBL->movie()) ui->waitLBL->movie()->stop();
//    ui->waitLBL->hide();


    //display the webpage title
     ui->bookNameLBL->setText(wview->title());


   if (InternalLocationInHtml != "")
   {
       QString script = "paintByHref(\"" + InternalLocationInHtml.replace("#", "$") + "\");";
       wview->page()->mainFrame()->evaluateJavaScript(script);

       qDebug() << script;
       InternalLocationInHtml="";
   }

   ui->stackedWidget->setCurrentIndex(DISPLAY_PAGE);
   QApplication::processEvents();
}

void MobileApp::wvloadStarted()
{
//    QApplication::processEvents();
    qDebug()<< "load started";
    ui->stackedWidget->setCurrentIndex(WAIT_PAGE);

    if (!waitMovie)
    {
        qDebug() << "waitMovie ceased to exist!";
//        waitMovie = new QMovie(":/Images/ajax-loader.gif");
        waitMovie = new QMovie(":/Images/Wait.gif");
    }

//    ui->displayArea->setTitle(tr("Loading..."));
//    ui->bookNameLBL->setText(tr("Loading..."));

//    ui->waitLBL->show();
    ui->waitLBL->setMovie(waitMovie);
    ui->waitLBL->movie()->start();
//    waitMovie->start();

    QApplication::processEvents();

}

void MobileApp::wvlinkClicked(const QUrl & url)
{
    QString link = QString(url.toString());

//    qDebug() << "url clicked: " << url;

    if(link.indexOf("#") != -1 )
    {
        int pos = link.indexOf("#");
        QString lnk = link.mid(pos+1);

        QString script = "paintByHref(\"$" + lnk + "\");";

        wview->page()->currentFrame()->evaluateJavaScript(script);
//        wview->page()->currentFrame()->scrollToAnchor(lnk);
    }
    //External book link
    else if(link.indexOf("!") != -1 )
    {
        int pos = link.indexOf("!");

        QString lnk = link.mid(pos+1);

        //Open the link
        QStringList parts = lnk.split(":");

        int id;
        if(ToNum(parts[0], &id))
        {
            Book* book = bookList.findBookById(id);
            if( book )
            {
                InternalLocationInHtml = "#" + parts[1];

                /*
                if (parts.size() == 3)
                    CurrentBookdisplayer()->setSearchMarker( QRegExp(unescapeFromBase32(parts[2])) );
                */

                showBook(book);
            }
        }
    }
    //Link to website
    else if(link.indexOf("^") != -1 )
    {
        int pos = link.indexOf("^");

        QString lnk = link.mid(pos+1);

        //Open using browser
        QDesktopServices::openUrl( QUrl("http://" + lnk) );
    }

    //Link to chapter in book
    else if(link.indexOf("@") != -1 )
    {
        int pos = link.indexOf("@");
        //Level mark is allways 1 digit only
        QString lvls = link.mid(pos+1, 1);
        QString lnk = link.mid(pos+2);

        BookIter itr = BookIter::fromEncodedString(lnk);

        bool ok;
        int lvl = lvls.toInt(&ok) -1;

        QString html = currentBook->getChapterHtml(itr, &bookList, true, true, ok ? lvl : 1);
        wview->setHtml(html);
    }
}



//Overrides the normal "closeEvent", so it can save tha window's state before quiting
void MobileApp::closeEvent(QCloseEvent *event)
{

    //Cancel any running searches
    stopSearchFlag = true;

    QSettings settings("Orayta", "SingleUser");

    //remmeber last open book
    settings.beginGroup("History");
     if (currentBook)
         settings.setValue("lastBook", currentBook->getUniqueId());

     //store current possision in book. not 100% acurate, but is good enough for me. (IZAR)
     currentPossision = wview->page()->currentFrame()->scrollPosition();
     settings.setValue("possision", currentPossision);
     settings.endGroup();

//    settings.beginGroup("DesktopApp");
//    settings.setValue("size", size());
//    settings.setValue("pos", pos());
//    settings.setValue("state", saveState());
//    settings.endGroup();


//    settings.beginGroup("Confs");
  //  settings.setValue("fontfamily", gFontFamily);
  //  settings.setValue("fontsize", gFontSize);
   // settings.endGroup();

  //  qDebug() << "on colse event\n" << "settings: "<<settings.fileName();

     //Save books' settings
//     for(unsigned int i=0; i<bookList.size(); i++)
     foreach (Book *book, bookList)
     {
         if (book->getUniqueId() == -1 || book->hasRandomId)
             continue;

         settings.beginGroup("Book" + stringify(book->getUniqueId()));
//             settings.setValue("MixedDisplayes", book->mWeavedSources.size());
             settings.setValue("ShowAlone", book->showAlone);
             for (int j=1; j<book->mWeavedSources.size(); j++)
             {
                 settings.setValue("Shown" + stringify(j), book->mWeavedSources[j].show);
             }
             settings.setValue("InSearch", book->IsInSearch());
         settings.endGroup();
     }

    QDialog::close();
}

void MobileApp::keyReleaseEvent(QKeyEvent *keyEvent){

    switch ( keyEvent->key() )
    {

    //back button was clicked
    case Qt::Key_MediaPrevious:
        goBack();
        break;

    //ctrl + backspace clicked , go back.
    case Qt::Key_Backspace:
        if (keyEvent->modifiers() == Qt::CTRL) goBack();
        break;

  /*  case Qt::Key_Back:
        qDebug()<<"caught back";
        return;
    case Qt::Key_Close:
        qDebug()<<"caught close";
         return;
    case Qt::Key_TopMenu:
        qDebug()<<"caught manu";
         return;*/

    //menu button was clicked
    case Qt::Key_Explorer:
    case Qt::Key_Meta:
        showMenu();
        break;

    default:
//        qDebug() << "key pressed: " << keyEvent->key();
        QDialog::keyReleaseEvent(keyEvent);
    }

    keyEvent->accept();
}

// when menu butten clicked, show the menu
void MobileApp::showMenu()
{
    qDebug() << "show menu";
    switch (ui->stackedWidget->currentIndex())
    {
    case DISPLAY_PAGE:
        //show/hide the toolbar menu
        if (ui->dispalyMenu->isVisible())
            ui->dispalyMenu->hide();
        else
            ui->dispalyMenu->show();
        return;

    case MAIN_PAGE:
        //go to the main setting page
        ui->stackedWidget->setCurrentIndex(SETTINGS_PAGE);
        return;

    case SEARCH_PAGE:
        ui->stackedWidget->setCurrentIndex(BOOK_SELECTION_PAGE);
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

    if(!viewHistory)
    {
        qDebug()<< "cant stat view history";
        return;
    }

//    if (ui->stackedWidget->currentWidget())
//        viewHistory->append(ui->stackedWidget->currentWidget());
    //add this page to history
    viewHistory->append(index);

    //IZAR
    // this is a list of things to do when we go to a certain page
    switch (index){
    //when going to books in search page, reset the page
    case (BOOK_SELECTION_PAGE):
        //canceled. not needed and does trouble.
//        resetSearchBookTree();
        break;

    //when going to get books page get the list from server.
    case (GET_BOOKS_PAGE):
        downloadDWList();
        break;

    case (SETTINGS_PAGE):
        resetSettingsPage();
        break;

    //when going to commentary selection page, reset it.
    case (MIXED_SELECTION_PAGE):
        setupMixedSelection();
        break;
    }

}

//go to previous view of stacked widget.
void MobileApp::goBack()
{
    //qDebug()<< "going back";

    if (ui->stackedWidget->currentIndex() == MAIN_PAGE)
    {
        qDebug()<< "exiting";
        close();
    }

    // if we have only one object it probably is the current view and we cant go back
    else if(!viewHistory)
    {
        qDebug()<< "no history found! exiting.";
        close();
    }

    //go back through history and find apropriate location to go to
    for (int i = viewHistory->length(); i> 0; i--)
    {
        int id = viewHistory->at(i-1);
        //id of currently displayed page
        int currentId = ui->stackedWidget->currentIndex();
        if (id != currentId)
        {
            switch (id)
            {
            //some pages should be ignored from history:
            case WAIT_PAGE:
                // we are in progress of loading a book. stop it.
                wview->stop();
            case SETTINGS_PAGE:
            case MIXED_SELECTION_PAGE :

                break;

            default:
                ui->stackedWidget->setCurrentIndex(id);
                viewHistory->removeAt(i-1);
                return;

            }
        }

        //remove this item from history
        viewHistory->removeAt(i-1);
    }

    // if we got 'till here then:
    qDebug()<< "no ware to go. exiting.";
    close();


//    else
//    {
//        //when going back from diplay page, stop loading the page.
//        if (ui->stackedWidget->currentIndex() == DISPLAY_PAGE)
//        {
//            wview->stop();
//            currentPossision = wview->page()->currentFrame()->scrollPosition();
//        }

//        //go to the one-before-last view, which is the previous view.
//        QWidget *previousView = viewHistory->at(viewHistory->length()-2);

//        //remove the last two objects. these are the current and previous view. the previous view will be re insereted later via viewChanged.
//        viewHistory->removeLast(); viewHistory->removeLast();

//        ui->stackedWidget->setCurrentWidget(previousView);

//    }
}




void MobileApp::on_toolButton_3_clicked()
{
    wview->setZoomFactor(wview->zoomFactor() + 0.1);
}

void MobileApp::on_toolButton_2_clicked()
{
    if (wview->zoomFactor() > 0.3) wview->setZoomFactor(wview->zoomFactor() - 0.1);
}

void MobileApp::on_toolButton_6_clicked()
{
    wview->page()->mainFrame()->scrollToAnchor("Top");
}


void MobileApp::on_settings_BTN_clicked()
{
     ui->stackedWidget->setCurrentIndex(SETTINGS_PAGE);
}

void MobileApp::on_saveConf_clicked()
{
    //Save font
    gFontFamily = ui->fontComboBox->currentFont().family();
    gFontSize = ui->fonSizeSpinBox->value();

    ui->saveConf->setEnabled(false);

    //Change language if needed
    QSettings settings("Orayta", "SingleUser");
    settings.beginGroup("Confs");

    //save current font settings
    settings.setValue("fontfamily", gFontFamily);
    settings.setValue("fontsize", gFontSize);

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
   qDebug() << "settings: "<<settings.fileName();

//    emit ChangeLang(LANG);
    translate(LANG);

    //Remove all temporary html files, so that books will be displayed with new settings.
    ClearTmp();
    //also, clear currently displayed book.

    // test if the previous view was the book itself. if so we want to reload the book.
    if (viewHistory->length() > 2 &&
          /* viewHistory->at(viewHistory->length()-2)->objectName() ==
           ui->stackedWidget->widget(DISPLAY_PAGE)->objectName()) */
            viewHistory->at(viewHistory->length()-2) == DISPLAY_PAGE)
    {
        if (currentBook) {
            // remove two last itmes from history. (settings page and dispaly page).
//            viewHistory->removeLast(); viewHistory->removeLast();

            //reload previously shown book
            showBook(currentBook);
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

void MobileApp::on_fonSizeSpinBox_valueChanged(int size)
{
    //Settings have changed, so the save button should be enabled
    ui->saveConf->setEnabled(true);

    //set the slider to the same value
    ui->horizontalSlider->setValue(size);

    //Show the new font in the preview box
    ui->fontPreview->setFont(QFont(ui->fontComboBox->currentFont().family(), size));
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
void MobileApp::on_SearchInBooksBTN_clicked()
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
        wview->load(QUrl(u));
        wview->page()->setLinkDelegationPolicy(QWebPage::DelegateAllLinks);
        ui->stackedWidget->setCurrentIndex(DISPLAY_PAGE);

        //done search, reset the ui
        showHideSearch(false);

}

void MobileApp::on_searchInBooksLine_returnPressed() { on_SearchInBooksBTN_clicked(); }



//Cancel search
void MobileApp::on_stopSearchBTN_clicked()
{
    stopSearchFlag = true;
    showHideSearch(false);
}

// switch the view from normal to in search mode
void MobileApp::showHideSearch(bool inSearch){
    ui->inSearchGroup->setVisible(inSearch);
    ui->searchStartGroup->setVisible(!inSearch);
   /*
    if (inSearch)
    {
        ui->searchGBX->show();
         //hide the search butten and show stop search
        ui->SearchInBooksBTN->hide();
        ui->stopSearchBTN->show();
        ui->selectBooksForSearchLink->hide();

    }
    else
    {
        ui->searchGBX->hide();
        //show the search butten and hide stop search
        ui->SearchInBooksBTN->show();
        ui->stopSearchBTN->hide();
        ui->selectBooksForSearchLink->show();
    } */
}


//IZAR initiate download of download list
void MobileApp::downloadDWList()
{
    QFile *downloadedList = new QFile(SAVEDBOOKLIST);

    //if listdownload dosn't exist we should reload.
    if (listdownload == NULL || listdownload == 0)
    {
        listdownload = new FileDownloader();
        connect(listdownload, SIGNAL(done()), this, SLOT(listDownloadDone()));
        listdownload->Download(BOOKLISTURL, SAVEDBOOKLIST, true);
    }
    //if the list already exists
    else if ( downloadedList->exists())
    {
        // continue as though the file was downloaded.
//        listDownloadDone();
        updateDownloadableList();
    }
    else
    {

//        if (listdownload ) delete listdownload;
        listdownload = new FileDownloader();
        connect(listdownload, SIGNAL(done()), this, SLOT(listDownloadDone()));

        listdownload->Download(BOOKLISTURL, SAVEDBOOKLIST, true);
    }

}

void MobileApp::listDownloadDone()
{
    //If all is ok
    if (listdownload)
    {
        if (listdownload->getFileName().contains("Android"))
        {
            updateDownloadableList();
        }
        else
        {
            listdownload = NULL;
             downloadDWList();
        }

    }
    else
        // this shouldn't happen, but os so we should recreate everything.
        downloadDWList();
}


void MobileApp::updateDownloadableList()
{
    //Get list of previously downloaded books
    QSettings settings("Orayta", "SingleUser");
    settings.beginGroup("DownloadedBooks");
        //(Fixes wierd behavior of QSettings)
    downloadedBooks = settings.allKeys().replaceInStrings("http:/", "http://");
    settings.endGroup();

    //Refresh the list
    ui->downloadListWidget->clear();
    //show aprorpriate widgets
    ui->downloadGRP->show();
    ui->listdownloadlbl->hide();


    QStringList t;
    ReadFileToList(SAVEDBOOKLIST, t, "UTF-8");

    for (int i=0; i<t.size(); i++)
    {
        QStringList tt = t[i].split(",");

        QListWidgetItem *lwi;
        if (tt.size() > 2)
        {
            if (!downloadedBooks.contains(tt[0]))
            {
                lwi= new QListWidgetItem(tt[1] + " (" + tt[2] + " MB)");
                lwi->setCheckState(Qt::Unchecked);
                lwi->setWhatsThis(tt[0]);

                ui->downloadListWidget->addItem(lwi);
                ui->downloadListWidget->setEnabled(true);
            }
        }
    }
}

// start download of the selected books.
void MobileApp::on_downloadBTN_clicked()
{
    downloader->abort();
    downloadsList.clear();

    for (int i=0; i<ui->downloadListWidget->count(); i++)
    {

        QListWidgetItem *item = ui->downloadListWidget->item(i);
        if (item->checkState() == Qt::Checked)
        {
            //Generate download url
            QString url = item->whatsThis();

            downloadsList << url;
        }
    }
    ui->downloadListWidget->setEnabled(false);
    ui->downloadBTN->setEnabled(false);

    qDebug() << downloadsList;

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
        QString target = BOOKPATH + name;

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
        ui->downloadInfo->setText(tr("download complete!"));

        //reload the book tree
        reloadBooklist();

        //Refresh the download list window
        ui->downloadListWidget->clear();
//        downloadDWList();

        //reset the download page
        ui->downloadPrgBar->hide();
        ui->downloadListWidget->setEnabled(true);
        ui->downloadBTN->setEnabled(true);

        markDownloadedBooks();

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
        //Unpack the file:
        if (!zipExtract(downloader->getFileName(), BOOKPATH))
        {
            qDebug() << "Couldn't extract:" << downloader->getFileName();

            //If extracting failed, don't mark that book as downloaded
            downloadedBooks.removeLast();
        }

        QFile f(downloader->getFileName());
        f.remove();

        ui->downloadPrgBar->hide();

        //this file has finished downloading, get the next file.
        downloadNext();
    }
}

void MobileApp::on_downloadListWidget_itemDoubleClicked(QListWidgetItem *item)
{
    if (item->checkState() == Qt::Checked)
        item->setCheckState(Qt::Unchecked);
    else
        item->setCheckState(Qt::Checked);
}

void MobileApp::markDownloadedBooks()
{
    QSettings settings("Orayta", "SingleUser");

    settings.beginGroup("DownloadedBooks");

    for(unsigned int i=0; i<downloadedBooks.size(); i++)
    {
        settings.setValue(downloadedBooks[i], "Downloaded");
    }
    settings.endGroup();
}


void MobileApp::on_settingsMenuBTN_clicked()
{
    ui->stackedWidget->setCurrentIndex(SETTINGS_PAGE);
}

void MobileApp::on_doneBTN_clicked()
{
    goBack();
}


void MobileApp::on_selectBooksForSearchLink_clicked()
{
    ui->stackedWidget->setCurrentIndex(BOOK_SELECTION_PAGE);
}

void MobileApp::on_SearchTreeWidget_itemClicked(QTreeWidgetItem *item, int column)
{
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

    QSettings settings("Orayta", "SingleUser");

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
    //TODO fix droid font and make it the default
    QString defaultFont = "Droid Sans Hebrew Orayta";
//    QString defaultFont = "Ezra SIL SR";
    gFontFamily = settings.value("fontfamily", defaultFont).toString();
    gFontSize = settings.value("fontsize",20).toInt();
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
    Book* book = currentBook;

    // no current book or no commentaries available.
    if (!book || !book->IsMixed()){
        ui->noCommemtaries->show();
        ui->mixedGroup->hide();
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
            qDebug() << "id: " << id;
            ui->selectionArea->addItem(item);
        }

        /*
        //Clear old entries
        for (int i=0; i<weavedList.size(); i++)
        {
            weavedList[i]->hide();
        }
        weavedList.clear();

        //Create new entries
        for(int i=1; i<book->mWeavedSources.size(); i++)
        {
            QCheckBox *chk = new QCheckBox(book->mWeavedSources[i].Title, ui->mixedFrame);
            ui->selectionArea->addWidget(chk);

            if (book->mWeavedSources[i].show == true)
            {
                chk->setChecked(true);
            }

            chk->setWhatsThis(stringify(book->mWeavedSources[i].id));

            weavedList << chk;

//            QSignalMapper *signalMapper = new QSignalMapper(this);

//            connect(chk, SIGNAL(clicked()), signalMapper, SLOT(map()));
//            signalMapper->setMapping(chk, weavedList.size() - 1);
//            connect(signalMapper, SIGNAL(mapped(int)), this, SLOT(weavedCheckBoxClicked(int)));
        }

        ui->showaloneCBX->setChecked(book->showAlone);
        ui->mixedFrame->setEnabled(!ui->showaloneCBX->isChecked());
        */

        /* void DesktopApp::weavedCheckBoxClicked(int btnIndex)
    //Still a bit ugly

    QTreeWidgetItem* current = ui->treeWidget->currentItem();
    Book* book = bookList.findBookByTWI(current);

    if (book == NULL) return;

    QString id = weavedList[btnIndex]->whatsThis();

    for(int i=1; i<book->mWeavedSources.size(); i++)
    {
        if (stringify(book->mWeavedSources[i].id) == id)
        {
            book->mWeavedSources[i].show = weavedList[btnIndex]->checkState();
        }
    }

    book->setTabWidget( 0 );
    */
    }
}

void MobileApp::on_openMixed_clicked()
{
    if (!currentBook) return;

    bool showalone =true;

    for(int j=1; j<currentBook->mWeavedSources.size(); j++)
    {
        QString srcId = stringify(currentBook->mWeavedSources[j].id);

        for (int i =0; i< ui->selectionArea->count(); i++){
            QListWidgetItem *item = ui->selectionArea->item(i);

              QString widgetId = item->whatsThis();

              if (srcId == widgetId) //we found the currect item
              {
                  bool checked = item->checkState() == Qt::Checked? true : false;

                  //set the showability of this item to what the user chose.
                  currentBook->mWeavedSources[j].show = checked;
                  if (checked) showalone = false;
              }
            }
    }

    currentBook->showAlone = showalone;

    //remove old html files so that book will be displayed currectly.
    ClearTmp();
    //show the book
    showBook(currentBook);

    //clear the history. we don't want the back butten to take us here.
//    viewHistory->removeLast(); viewHistory->removeLast();

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
        ui->downloadListWidget->item(i)->setCheckState(Qt::Unchecked);
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


void MobileApp::on_selectionArea_itemClicked(QListWidgetItem *item)
{
    //toggle the current check state of the item
    item->checkState() == Qt::Checked? item->setCheckState(Qt::Unchecked) : item->setCheckState(Qt::Checked);
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
    QPoint currentPossision;

    //probably we just loaded the app
    if (!currentBook)
    {
        //get last open book
        QSettings settings("Orayta", "SingleUser");
        settings.beginGroup("History");
        int lastBookId = settings.value("lastBook").toInt();
        currentBook = bookList.findBookById(lastBookId);
        currentPossision = settings.value("possision", QPoint()).toPoint();
        settings.endGroup();
    }
    if (!currentBook) return;

    // guess if the book is already loaded on wview
    if (currentBookDisplayed())
        ui->stackedWidget->setCurrentIndex(DISPLAY_PAGE);
    else
    {
        showBook(currentBook);
        QApplication::processEvents();
        wview->page()->currentFrame()->setScrollPosition(currentPossision);
    }


}

// test if 'currnetBook' is displayed now
bool MobileApp::currentBookDisplayed()
{
    if (!currentBook || !wview) return false;

   QString filename = currentBook->HTMLFileName();
   if (wview->url().toString().contains(filename)) return true;
   else return false;
}


void MobileApp::on_toGetBooksBTN_clicked()
{    ui->stackedWidget->setCurrentIndex(GET_BOOKS_PAGE); }


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
