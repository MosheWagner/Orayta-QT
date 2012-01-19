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


//TODO: Catch android phisical button events (back & menu)
//TODO: Select books for search

//TODO: Get rid of horrible text over text bug
//TODO: Test landscape - portrait switching
//TODO: Improve look & feel
//TODO: Bookmarks
//TODO: Improve book loading speed (gradual loading)
//TODO: implement weaved view.

//IZAR-
//TODO: load page preview




#include <QKinetic/qtscroller.h>




MobileApp::MobileApp(QWidget *parent) :QDialog(parent), ui(new Ui::MobileApp)
{
    //Set all QString to work with unicode
    QTextCodec::setCodecForCStrings(QTextCodec::codecForName("utf8"));
    QTextCodec::setCodecForTr(QTextCodec::codecForName("utf8"));

    ui->setupUi(this);

    ui->stackedWidget->setCurrentIndex(ABOUT_PAGE);


    QApplication::processEvents();


    //check whare is the bset location for settings.
   // if (!settings.isWritable()) {
      //  qDebug() << "cant write to default settings location: " << settings.fileName();
    //}
   // settings.beginGroup("Confs");
   // gFontFamily = settings.value("fontfamily", "Droid Sans Hebrew").toString();
   // gFontSize = settings.value("fontsize",20).toInt();
   // settings.endGroup();

    //setup the settings page
    setupSettings();

    InternalLocationInHtml = "";

    wview = new myWebView(this);


    QObject::connect(wview, SIGNAL(linkClicked(const QUrl &)), this , SLOT(wvlinkClicked(const QUrl &)));
    QObject::connect(wview, SIGNAL(loadFinished(bool)), this , SLOT(wvloadFinished(bool)));


    viewHistory = new QWidgetList();
    connect(ui->stackedWidget, SIGNAL(currentChanged(int)), this, SLOT(viewChanged(int)));

//    ui->wview_layout->addWidget(wview);

    ui->displayArea->layout()->addWidget(wview);
//    ui->wview = wview;




    wview->setHtml(tr("<center><big>Loading...</big></center>"));


    wview->page()->setLinkDelegationPolicy(QWebPage::DelegateAllLinks);


    QtScroller::grabGesture(wview, QtScroller::LeftMouseButtonGesture);


    //IZAR
    //prevent wview from reciving back key.
//    MobileApp::grabKeyboard();
//    wview->keyPressEvent();

    wview->show();

//    ui->wview->show();


    // setup the search page
    showHideSearch(false);

    ui->downloadListWidget->setIconSize(QSize(40,40));



    QtScroller::grabGesture(ui->treeWidget, QtScroller::LeftMouseButtonGesture);
    ui->treeWidget->setColumnWidth(0,800);
    QtScroller::grabGesture(ui->SearchTreeWidget, QtScroller::LeftMouseButtonGesture);
     ui->SearchTreeWidget->setColumnWidth(0,800);


    //IZAR
    //TODO - use QMessageBox::aboutQt ?

    //IZAR
    //setup the wait image
    waitMovie = new QMovie(":/Images/Wait.gif");
    ui->waitLBL->setMovie(waitMovie);
    if (waitMovie) waitMovie->start();


    //Build the book list

    reloadBooklist();

    if (bookList.empty())
    {
        qDebug()<< "bookpath: " << BOOKPATH << " current dir: " << QDir::currentPath();
        ui->label->setText(tr("<center><b> No books found! \nCheck your installation, or contact the developer.</b></center>"));
        ui->label->setWordWrap(true);
    }

    //Initialize a new FileDownloader to download the list
//    listdownload = NULL;
    listdownload = new FileDownloader();
    connect(listdownload, SIGNAL(done()), this, SLOT(listDownloadDone()));


    //Initialize a new FileDownloader object for books downloading
    downloader = new FileDownloader();

    //Connect slots to the signalls of the book downloader
    connect(downloader, SIGNAL(done()), this, SLOT(downloadDone()));
    connect(downloader, SIGNAL(downloadProgress(int)), this, SLOT(downloadProgress(int)));
    connect(downloader, SIGNAL(downloadError()), this, SLOT(downloadError()));

    //Download the list of books that could be downloaded
//    downloadDWList();

    ui->downloadGRP->hide();
    ui->downloadPrgBar->hide();


    //IZAR
    // hack to enable me to test downloads without internet
//    listDownloadDoneOverride();


    // the default for the menu in display page is hidden.
//    ui->dispalyMenu->hide();

    if (!bookList.empty())
    {
        QApplication::processEvents();
        ui->stackedWidget->setCurrentIndex(MAIN_PAGE);
    }
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

    bookList.displayInTree(ui->treeWidget, false);


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
    if (ui->treeWidget->isExpanded(index)) ui->treeWidget->collapse(index);
    else ui->treeWidget->expand(index);
}


void MobileApp::on_openMixed_clicked()
{
    if ( ui->treeWidget->currentItem() == 0)
        return;
    Book *b = bookList.findBookByTWI(ui->treeWidget->currentItem());
    if (!b->IsDir()) showBook(b);
}


void MobileApp::on_treeWidget_itemDoubleClicked(QTreeWidgetItem *item, int column)
{
    Book *b = bookList.findBookByTWI(item);

    if (!b->IsDir()) showBook(b);
}


void MobileApp::showBook(Book *book)
{
    if (!book) return;

    //remember curently displayed book
    currentBook = book;

    ui->waitLBL->show();
    if (waitMovie)
    {
    ui->waitLBL->setMovie(waitMovie);
    ui->waitLBL->movie()->start();
    }

    //IZAR: temporary work-around. the problem is that orayta reads the global font settings ONLY on startup, and is careless if it is changed latter.
    //TODO: fix this.
    QFont font( gFontFamily, gFontSize );
    book->setFont(font);

    // display mixed selection button only if the book has commentaries
    if (book->IsMixed()) ui->mixedSelectBTN->show();
    else ui->mixedSelectBTN->hide();

    switch ( book->fileType() )
    {
        case ( Book::Normal ):
        {
            ui->stackedWidget->setCurrentIndex(DISPLAY_PAGE);
            ui->displayArea->setTitle(tr("Loading..."));

            //Generate filename representing this file, the commentreis that should open, and it's nikud (or teamim) mode
            //  This way the file is rendered again only if it needs to be shown differently (if other commenteries were requested)
            QString htmlfilename = book->HTMLFileName() + ".html";

            //Check if file already exists. If not, make sure it renders ok.
            QFile f(htmlfilename);
            bool renderedOK = true;

            if (!f.exists())
            {
                renderedOK = book->htmlrender(htmlfilename, true, true, "");
            }

            if (renderedOK == true)
            {
                QString p =  absPath(htmlfilename);
                QUrl u = QUrl::fromLocalFile(p);

                ui->stackedWidget->setCurrentIndex(DISPLAY_PAGE);

                wview->load(u);

                wview->page()->setLinkDelegationPolicy(QWebPage::DelegateAllLinks);

                booktitle = book->getNormallDisplayName();
            }
            break;
        }
        case ( Book::Html ):
        {
            wview->load( QUrl::fromLocalFile(book->getPath()) );
            wview->page()->setLinkDelegationPolicy(QWebPage::DelegateExternalLinks);
            //TODO: title
            ui->stackedWidget->setCurrentIndex(DISPLAY_PAGE);
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



//Overrides the normal "closeEvent", so it can save tha window's state before quiting
void MobileApp::closeEvent(QCloseEvent *event)
{

    //Cancel any running searches
    stopSearchFlag = true;

//    QSettings settings("Orayta", "SingleUser");

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

    if (ui->stackedWidget->currentWidget())
        viewHistory->append(ui->stackedWidget->currentWidget());

    //IZAR
    // this is a list of things to do when we go to a certain page
    switch (index){
    //when going to books in search page, reset the page
    case (BOOK_SELECTION_PAGE):
        resetSearchBookTree();
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

//IZAR
//when going to 'books in search' page, reset the page
void MobileApp::resetSearchBookTree(){

    ui->SearchTreeWidget->clear();
    bookList.displayInTree(ui->SearchTreeWidget, true);

}

//go to previous view of stacked widget.
void MobileApp::goBack()
{
    qDebug()<< "going back";

    // if we have only one object it probably is the current view and we cant go back
    if((!viewHistory) || (viewHistory->length() < 2))
    {
        qDebug()<< "noware to go. exiting.";

        close();
    }
    else if (ui->stackedWidget->currentIndex() == MAIN_PAGE)
    {
        qDebug()<< "exiting";
//        exit(0);
        close();
    }
    else
    {
        //when going back from diplay page, stop loading the page.
        if (ui->stackedWidget->currentIndex() == DISPLAY_PAGE)
            wview->stop();

        //go to the one-before-last view, which is the previous view.
        QWidget *previousView = viewHistory->at(viewHistory->length()-2);

        //remove the last two objects. these are the current and previous view. the previous view will be re insereted later via viewChanged.
        viewHistory->removeLast(); viewHistory->removeLast();

        ui->stackedWidget->setCurrentWidget(previousView);


    }
}


void MobileApp::wvloadFinished(bool ok)
{
    if (ui->waitLBL->movie()) ui->waitLBL->movie()->stop();
    ui->waitLBL->hide();


    wview->page()->setLinkDelegationPolicy(QWebPage::DelegateAllLinks);

    ui->displayArea->setTitle(booktitle);


   if (InternalLocationInHtml != "")
   {
       QString script = "paintByHref(\"" + InternalLocationInHtml.replace("#", "$") + "\");";
       wview->page()->mainFrame()->evaluateJavaScript(script);

       qDebug() << script;
       InternalLocationInHtml="";
   }
}

void MobileApp::wvlinkClicked(const QUrl & url)
{
    QString link = QString(url.toString());

    if(link.indexOf("#") != -1 )
    {
        int pos = link.indexOf("#");
        QString lnk = link.mid(pos+1);

        QString script = "paintByHref(\"$" + lnk + "\");";

        wview->page()->mainFrame()->evaluateJavaScript(script);
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

    //Change language if needed
    settings.setValue("systemLang",ui->systemLangCbox->isChecked());
    if (ui->systemLangCbox->isChecked())
    {
        LANG = QLocale::languageToString(QLocale::system().language());
    }
    //Use custom language only if "useSystemLang" is not checked
    else
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
//    //TODO: make this reload the book automatically.
//    wview->setHtml(tr("<center><big>please go back and reload book.</big></center>"));

    // test if the previous view was the book itself. if so we want to reload the book.
    if (viewHistory->length() > 2 &&
            viewHistory->at(viewHistory->length()-2)->objectName() ==
            ui->stackedWidget->widget(DISPLAY_PAGE)->objectName())
    {
        if (currentBook) {
            // remove two last itmes from history. (settings page and dispaly page).
            viewHistory->removeLast(); viewHistory->removeLast();

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

void MobileApp::on_horizontalSlider_sliderMoved(int position)
{
    //set this value to the SpinBox
    ui->fonSizeSpinBox->setValue(position);
    MobileApp::on_fonSizeSpinBox_valueChanged(position);
}


void MobileApp::on_cancelBTN_clicked()
{
//    ui->stackedWidget->setCurrentIndex(MAIN_PAGE);
    goBack();
    resetSettingsPage();
}

//bool inSearch = false;
void MobileApp::on_SearchInBooksBTN_clicked()
{
//    if (!inSearch)
//    {
        //Do search
        QString otxt = ui->searchInBooksLine->text();
        QString stxt = otxt;
        QRegExp regexp;

        /*
        if (ui->fuzzyCheckBox->isChecked())
            stxt = AllowKtivHasser(stxt);
        */

        regexp = QRegExp( createSearchPattern (stxt) );
        regexp.setMinimal(true);


        //Set the title of the tab to show what it's searching for
        //QString title = tr("Searching: "); title += "\"" + otxt + "\"" + " ...";
        //ui->viewTab->setTabText(CURRENT_TAB, title);
//        ui->searchGBX->show();
//        inSearch = true;
//        ui->SearchInBooksBTN->setText(tr("Cancel search"));
//        ui->stopSearchBTN->show();
//        ui->SearchInBooksBTN->hide();

        //show the stop button and search bar
        showHideSearch(true);

        QApplication::processEvents();

        QUrl u = SearchInBooks (regexp, otxt, bookList /*.BooksInSearch()*/, ui->progressBar);
        wview->load(QUrl(u));
        wview->page()->setLinkDelegationPolicy(QWebPage::DelegateAllLinks);
        ui->stackedWidget->setCurrentIndex(DISPLAY_PAGE);

        //done search, reset the ui
        showHideSearch(false);

//        ui->searchGBX->hide();
//        inSearch = false;
//        ui->searchBTN->setText(tr("Search"));
//        ui->SearchInBooksBTN->show();
//        ui->stopSearchBTN->hide();
//    }
//    else
//    {
//        //Cancel search
//        stopSearchFlag = true;

//        ui->searchGBX->hide();
//        inSearch = false;
//        ui->SearchInBooksBTN->setText(tr("Search"));
//    }
}

//Cancel search
void MobileApp::on_stopSearchBTN_clicked()
{

    stopSearchFlag = true;
    showHideSearch(false);
//    ui->searchGBX->hide();
//    inSearch = false;
//    ui->SearchInBooksBTN->setText(tr("Search"));
//    ui->SearchInBooksBTN->show();
//    ui->stopSearchBTN->hide();
}

// switch the view from normal to in search mode
void MobileApp::showHideSearch(bool inSearch){
    if (inSearch)
    {
        ui->searchGBX->show();
         //hide the search butten and show stop search
        ui->SearchInBooksBTN->hide();
        ui->stopSearchBTN->show();
    }
    else
    {
        ui->searchGBX->hide();
        //show the search butten and hide stop search
        ui->SearchInBooksBTN->show();
        ui->stopSearchBTN->hide();
    }
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
    //TODO :
    // * make this recursivly set the state of childrean
    // * make sure the search function reads this.
    if (item->checkState(column) == Qt::Checked)
    {
        item->setCheckState(column, Qt::Unchecked);

        QList<QTreeWidgetItem *> children = item->takeChildren();
//        foreach (QTreeWidgetItem *child, children)
//        {
//           item->setCheckState(column, Qt::Unchecked);
//        }
//        item->addChildren(children);
//        ui->SearchTreeWidget->reset();
    }
    else
    {
        item->setCheckState(column, Qt::Checked);
//        QList<QTreeWidgetItem *> children = item->takeChildren();
//        foreach (QTreeWidgetItem *child, children)
//        {
//           item->setCheckState(column, Qt::Checked);
//        }
//        item->addChildren(children);
//         ui->SearchTreeWidget->reset();
    }
}

void MobileApp::on_SearchTreeWidget_clicked(const QModelIndex &index)
{
    // TODO - create a method for checking the box without expanding the tree.
}


//---------------------------------------
//the folowing methods where copied from Settings.cpp, but modified.
//TODO: they shold be implemented in a separate class.

void MobileApp::on_systemLangCbox_clicked(bool checked)
{
    // Disable language choosing if the "use system language" is selected
    ui->groupBox->setEnabled(!checked);

    //Settings have changed, so the save button should be enabled
    ui->saveConf->setEnabled(true);
}

void MobileApp::setupSettings(){

    //Set available languages
    langs << "Hebrew" << "English" << "French";
    langsDisplay << tr("עברית") << "English" << tr("Français");

    //Show available languages in the language combobox
    for (int i=0; i<langs.size(); i++)
    {
        ui->langComboBox->addItem(langsDisplay[i]);
    }

    //Check if "use system lang" is set
    QSettings settings("Orayta", "SingleUser");
    settings.beginGroup("Confs");
    bool useSystemLang = settings.value("systemLang",true).toBool();
    settings.endGroup();

    ui->systemLangCbox->setChecked(useSystemLang);
    ui->groupBox->setEnabled(!useSystemLang);

    //Show current language
    int is = -1;
    for (int i=0; i<langs.size(); i++) if (LANG == langs[i]) is = i;
    ui->langComboBox->setCurrentIndex(is);

    //get stored settings for display font
    settings.beginGroup("Confs");
    gFontFamily = settings.value("fontfamily", "Droid Sans Hebrew").toString();
    gFontSize = settings.value("fontsize",20).toInt();
    settings.endGroup();

    resetSettingsPage();


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

        for(int i=1; i<book->mWeavedSources.size(); i++){
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

void MobileApp::on_openMixed_2_clicked()
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
    viewHistory->removeLast(); viewHistory->removeLast();

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

void MobileApp::on_selectionArea_itemClicked(QListWidgetItem *item)
{
    //toggle the current check state of the item
    item->checkState() == Qt::Checked? item->setCheckState(Qt::Unchecked) : item->setCheckState(Qt::Checked);
}


void MobileApp::on_moreInfoBTN_clicked()
{
    //show wellcome page
    Book *wellcome = bookList.findBookById(1);
    if (!wellcome)
    {
        qDebug()<< "cant find wellcome page";
        return;
    }

    showBook(wellcome);
}
