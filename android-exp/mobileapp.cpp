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

//#include <QScroller>

#define MAIN_PAGE 0
#define ABOUT_PAGE 1
#define DISPLAY_PAGE 2
#define LIST_PAGE 3
#define SEARCH_PAGE 4
#define GET_BOOKS_PAGE 5
#define SETTINGS_PAGE 6


//TODO: Test book downloading thoroughly

//TODO: Select books for search

//TODO: Get rid of horrible text over text bug
//TODO: Test landscape - portrait switching
//TODO: Improve look & feel
//TODO: Make kinetic scrolling work (webview + booklist)
//TODO: Search
//TODO: Bookmarks
//TODO: Improve book loading speed (gradual loading)
//TODO: implement weaved view.

//IZAR-
//TODO: load page preview




#include <QKinetic/qtscroller.h>




MobileApp::MobileApp(QWidget *parent) :QDialog(parent), ui(new Ui::MobileApp)
{
    ui->setupUi(this);

    ui->stackedWidget->setCurrentIndex(ABOUT_PAGE);

    QApplication::processEvents();

    //IZAR- changed this to use dejavu sans free font in all versions (linux, windows and android)
    //QString gFontFamily = "Nachlieli CLM";
    gFontFamily = "Droid Sans Hebrew";
    gFontSize = 20;
    ui->fontComboBox->setFont(QFont(gFontFamily));
    ui->fonSizeSpinBox->setValue(gFontSize);
    ui->horizontalSlider->setValue(gFontSize);

    InternalLocationInHtml = "";

    wview = new QWebView(this);

    QObject::connect(wview, SIGNAL(linkClicked(const QUrl &)), this , SLOT(wvlinkClicked(const QUrl &)));
    QObject::connect(wview, SIGNAL(loadFinished(bool)), this , SLOT(wvloadFinished(bool)));

    ui->displaypage->layout()->addWidget(wview);


    wview->setHtml("<center><big>Loading...</big></center>");

    wview->page()->setLinkDelegationPolicy(QWebPage::DelegateAllLinks);

    QtScroller::grabGesture(wview, QtScroller::LeftMouseButtonGesture);

    wview->show();


    //QtScroller::grabGesture(ui->treeWidget);
    ui->searchGBX->hide();

    ui->downloadListWidget->setIconSize(QSize::QSize(20,20));


    QtScroller::grabGesture(ui->treeWidget, QtScroller::LeftMouseButtonGesture);
    ui->treeWidget->setColumnWidth(0,800);


    //IZAR
    //TODO - use QMessageBox::aboutQt ?
   /* QString htmlLabl;

    htmlLabl += "Orayta 4 android: Version 1.1\n";
    htmlLabl += "Compiled with qt-version:"  ; htmlLabl +=QT_VERSION_STR  ; htmlLabl +="\n";
    htmlLabl += "Runtime qt-version:"  ; htmlLabl +=qVersion()  ; htmlLabl +="\n";
    htmlLabl += "Compiled qt-webkit-version:"  ; htmlLabl +=QTWEBKIT_VERSION_STR  ;


    ui->label->setText(htmlLabl); */

//    bookList.BuildFromFolder(BOOKPATH);

//    // Check all uids
//    bookList.CheckUid();

//    bookList.displayInTree(ui->treeWidget, false);
    reloadBooklist();

    if (bookList.empty())
    {
        qDebug()<< "bookpath: " << BOOKPATH;
        ui->label->setText(tr("<center><b> No books found! \nCheck your installation, or contact the developer.</b></center>"));
        ui->label->setWordWrap(true);
    }

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

    //try to increase the size of the font size slider
//    ui->horizontalSlider->setBaseSize(100,30);


    //IZAR
    //another atempt for a wait page
//    QString waitimage(":/Images/Wait.gif");
//    QGraphicsScene *wait;
//    wait = new QGraphicsScene();
//    wait->addPixmap(QPixmap::QPixmap(waitimage));
//    ui->graphicsView->setScene(wait);

    //Initialize a new FileDownloader to download the list
    listdownload = new FileDownloader();
    connect(listdownload, SIGNAL(done()), this, SLOT(listDownloadDone()));


//    listdownload->Download("http://orayta.googlecode.com/files/Android Books", TMPPATH + "Android-Books");
//    downloadDWList();

    if (!bookList.empty())
    {
        QApplication::processEvents();
        ui->stackedWidget->setCurrentIndex(MAIN_PAGE);
    }
}

MobileApp::~MobileApp()
{
    delete downloader;
    delete listdownload;

    delete ui;
}

//IZAR
// reload the whole book list and tree
void MobileApp::reloadBooklist(){

    //create a new empty booklist
    bookList = BookList::BookList();

    //Refresh book list
    ui->treeWidget->clear();


    bookList.BuildFromFolder(BOOKPATH);

    // Check all uids
    bookList.CheckUid();

    bookList.displayInTree(ui->treeWidget, false);

    //Refresh the download list window
    ui->downloadListWidget->clear();
    downloadDWList();


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
     ui->waitLBL->show();
//     ui->graphicsView->show();


    //IZAR: temporary work-around. the problem is that orayta reads the global font settings ONLY on startup, and is careless if it is changed latter.
    //TODO: fix this.
    QFont font( gFontFamily, gFontSize );
    book->setFont(font);

    switch ( book->fileType() )
    {
        case ( Book::Normal ):
        {
            ui->stackedWidget->setCurrentIndex(DISPLAY_PAGE);
            ui->titlelbl->setText("Loading...");


            //IZAR: temporary work-around. the problem is that orayta reads the global font settings ONLY on startup, and is careless if it is changed latter.
            //TODO: fix this.
            QFont font( gFontFamily, gFontSize );
            book->setFont(font);


            //ui->stackedWidget->setCurrentIndex(DISPLAY_PAGE);
            //QApplication::processEvents();


            //Generate filename representing this file, the commentreis that should open, and it's nikud (or teamim) mode
            //  This way the file is rendered again only if it needs to be shown differently (if other commenteries were requested)
            QString htmlfilename = book->HTMLFileName() + ".html";


            qDebug() << "rendering book; font: " << gFontFamily;

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

void MobileApp::on_toolButton_clicked()
{
    if (ui->stackedWidget->currentIndex() == DISPLAY_PAGE) ui->stackedWidget->setCurrentIndex(LIST_PAGE);
    else if (ui->stackedWidget->currentIndex() == MAIN_PAGE) exit(0);
    else ui->stackedWidget->setCurrentIndex(MAIN_PAGE);
}


//Catch android "back" button
void MobileApp::closeEvent(QCloseEvent *event)
{
    qDebug() << "Oh no!!!";

    if (ui->stackedWidget->currentIndex() == DISPLAY_PAGE)
    {
        ui->stackedWidget->setCurrentIndex(LIST_PAGE);
        event->ignore();
    }
    else if (ui->stackedWidget->currentIndex() == MAIN_PAGE || ui->stackedWidget->currentIndex() == ABOUT_PAGE)
    {
        event->accept();
        exit(0);
    }
    else
    {
        ui->stackedWidget->setCurrentIndex(MAIN_PAGE);
        event->ignore();
    }
}


void MobileApp::wvloadFinished(bool ok)
{

    ui->waitLBL->hide();
//    ui->graphicsView->hide();


    wview->page()->setLinkDelegationPolicy(QWebPage::DelegateAllLinks);

   ui->titlelbl->setText(booktitle);


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

void MobileApp::on_title_clicked()
{
    on_toolButton_clicked();
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

    settings.setValue("systemLang",ui->systemLangCbox->isChecked());
    if (ui->systemLangCbox->isChecked())
    {
        LANG = QLocale::languageToString(QLocale::system().language());
    }
    //Use custom language only if "useSystemLang" is not checked
    else
    {
//        int i = langsDisplay.indexOf(ui->langComboBox->currentText());
//        if (i != -1)
//        {
//            settings.setValue("lang", langs[i]);
//            settings.endGroup();
//            LANG = langs[i];
//        }
    }

//    emit ChangeLang(LANG);
    ui->stackedWidget->setCurrentIndex(MAIN_PAGE);
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
    ui->stackedWidget->setCurrentIndex(MAIN_PAGE);
}

bool inSearch = false;
void MobileApp::on_SearchInBooksBTN_clicked()
{
    if (!inSearch)
    {
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

        /*
        pCurrentBookdisplayer->ShowWaitPage();
        */

        //Set the title of the tab to show what it's searching for
        //QString title = tr("Searching: "); title += "\"" + otxt + "\"" + " ...";
        //ui->viewTab->setTabText(CURRENT_TAB, title);
        ui->searchGBX->show();
        inSearch = true;
        ui->SearchInBooksBTN->setText(tr("Cancel search"));

        QApplication::processEvents();

        QUrl u = SearchInBooks (regexp, otxt, bookList /*.BooksInSearch()*/, ui->progressBar);
        wview->load(QUrl(u));
        wview->page()->setLinkDelegationPolicy(QWebPage::DelegateAllLinks);
        ui->stackedWidget->setCurrentIndex(DISPLAY_PAGE);

        ui->searchGBX->hide();
        inSearch = false;
        ui->searchBTN->setText(tr("Search"));
    }
    else
    {
        //Cancel search
        stopSearchFlag = true;

        ui->searchGBX->hide();
        inSearch = false;
        ui->SearchInBooksBTN->setText(tr("Search"));
    }
}

void MobileApp::on_downloadBTN_clicked()
{
    downloader->abort();
    downloadsList = QStringList();

    for (int i=0; i<ui->downloadListWidget->count(); i++)
    {

        QListWidgetItem *item = ui->downloadListWidget->item(i);
        if (item-> checkState() == Qt::Checked)
        {
            //Generate download url

//            QString url = ui->downloadListWidget->itemAt(i, 0)->toolTip();
            QString url =item->whatsThis();

//            booksToDownload << url;
            downloadsList << url;
            //downloader->Download(url, target);
        }
    }
    ui->downloadListWidget->setEnabled(false);
    ui->downloadBTN->setEnabled(false);

    // download the next file in downloadsList.
    downloadNext();

}

// download the next file in downloadsList.
void MobileApp::downloadNext(){
    if (!downloadsList.isEmpty()){

        //Reset small progress bar
        ui->downloadPrgBar->setValue(0);
        ui->downloadPrgBar->show();


        QString url = downloadsList.first();
        QString name = url.mid(url.lastIndexOf("/") + 1);
        //Generate download target
        QString target = BOOKPATH + name;

        qDebug() <<"download file to: "<< target;
         downloader->Download(url, target, false);

         downloadsList.removeFirst();
    }
    //No more books to download
    else
    {
        qDebug() << "done downloading";

        //reload the book tree
        reloadBooklist();

        //reset the download page
        ui->downloadPrgBar->hide();
        ui->downloadListWidget->setEnabled(true);
        ui->downloadBTN->setEnabled(true);

         markDownloadedBooks();

        //switch view to book tree to see the new books
        if (ui->stackedWidget->currentIndex() == GET_BOOKS_PAGE)
            ui->stackedWidget->setCurrentIndex(LIST_PAGE);

    }
}


void MobileApp::downloadProgress(int val)
{
    ui->downloadPrgBar->setValue(val);
}

void MobileApp::downloadError()
{
    qDebug() << "Error downloading:" + downloader->getFileName();
    downloadNext();
}


void MobileApp::listDownloadDone()
{
    //List was downloaded
    if (listdownload)
    {
        QSettings settings("Orayta", "SingleUser");
        settings.beginGroup("DownloadedBooks");
        downloadedBooks = settings.allKeys().replaceInStrings("http:/", "http://");
        settings.endGroup();

        if (listdownload->getFileName().contains("Android"))
        {
//            qDebug() << "listDownload: "<< listdownload->getFileName() ;

            ui->downloadListWidget->clear();
            ui->downloadGRP->show();

            ui->listdownloadlbl->hide();

            QStringList t;
            ReadFileToList(listdownload->getFileName(), t, "UTF-8");

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

            QFile f(listdownload->getFileName());
            f.remove();

            return;
        }
    }
}

//IZAR
// hack to enable me to test downloads without internet
void MobileApp::listDownloadDoneOverride(){
    qDebug() << "<<<<<<<listDownloadDoneOverride, dont forget to remove before release>>>>>>>";

    ui->downloadGRP->show();

    ui->listdownloadlbl->hide();

    QStringList t;
    ReadFileToList(MAINPATH + "Android-Books", t, "UTF-8");

    qDebug() << "download list is: " << t;

    for (int i=0; i<t.size(); i++)
    {
        QStringList tt = t[i].split(",");

        QListWidgetItem *lwi;
        if (tt.size() > 2)
        {
            lwi= new QListWidgetItem(tt[1] + " (" + tt[2] + " MB)");
            lwi->setCheckState(Qt::Unchecked);
//            lwi->setToolTip(tt[0]);
            lwi->setWhatsThis(tt[0]);

            ui->downloadListWidget->addItem(lwi);
            ui->downloadListWidget->setEnabled(true);
        }
    }
}

void MobileApp::downloadDone()
{
    //Book downloaded
    if (downloader)
    {
        //Unpack the file:
        if (!zipExtract(downloader->getFileName(), BOOKPATH))
        {
            qDebug() << "Couldn't extract:" << downloader->getFileName();
        } else {
             downloadedBooks << downloader->getFileName();
        }

        QFile f(downloader->getFileName());
        f.remove();

        ui->downloadPrgBar->hide();



        //this file has finished downloading, get the next file.
        downloadNext();
    }


}

//void MobileApp::on_ReloadListBTN_clicked()
//{
//    downloadDWList();
//}

//IZAR initiate download of download list
void MobileApp::downloadDWList()
{
//  listdownload->Download("http://orayta.googlecode.com/files/Android-Books", TMPPATH + "Android-Books");   ;
    listdownload->Download(BOOKLISTURL, TMPPATH + "Android-Books", true);
}

void MobileApp::on_downloadListWidget_itemDoubleClicked(QListWidgetItem *item)
{
    if (item->checkState() == Qt::Checked)
        item->setCheckState(Qt::Unchecked);
    else
        item->setCheckState(Qt::Checked);
}


//Overrides the normal "closeEvent", so it can save tha window's state before quiting
void MobileApp::markDownloadedBooks()
{

    qDebug() << downloadedBooks;

    QSettings settings("Orayta", "SingleUser");

    settings.beginGroup("DownloadedBooks");

    for(unsigned int i=0; i<downloadedBooks.size(); i++)
    {
        settings.setValue(downloadedBooks[i], "Downloaded");
    }
    settings.endGroup();
}

