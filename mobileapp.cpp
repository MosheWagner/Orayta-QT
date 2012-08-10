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
#include "QKinetic/flickcharm.h"
#include "functions.h"
#include "booklist.h"
#include "search.h"
#include <QMessageBox>
#include <QDebug>
#include <QFile>
#include <QDir>
#include <QCloseEvent>
#include <QSettings>
#include <QDesktopServices>
#include <QMovie>
#include <QListWidget>
#include <QListWidgetItem>
#include <QTimer>
#include <QDesktopWidget>
#include "minibmark.h"




//TODO: CLEAN & SPLIT UP!!!!
//TODO: Improve look & feel

//TODO: Daf yomi bookmark...


// Global
QString gFontFamily = "Droid Sans Hebrew Orayta";
int gFontSize = 0;

int vp = 0;

MobileApp::MobileApp(QWidget *parent) :QDialog(parent), ui(new Ui::MobileApp)
{
    //Set all QString to work with unicode
    QTextCodec::setCodecForCStrings(QTextCodec::codecForName("utf8"));
    QTextCodec::setCodecForTr(QTextCodec::codecForName("utf8"));

    ui->setupUi(this);

    //qDebug() << dafYomi(QDate::currentDate());


    //show the about page while app loads
    ui->gtoHelp->hide();

    ui->stackedWidget->setCurrentIndex(ABOUT_PAGE);

    QApplication::processEvents();

    //set stuff as null only for pertection
    viewHistory = NULL;
    listdownload = NULL;
    downloader = NULL;

    //Initialize a new FileDownloader to download the list
    listdownload = new FileDownloader();
    connect(listdownload, SIGNAL(done()), this, SLOT(listDownloadDone()));
    //Initialize a new FileDownloader object for books downloading
    downloader = new FileDownloader();


    //Initialize the bookdisplayer object
    displayer = new textDisplayer(this, &bookList);
    ui->displayArea->layout()->addWidget(displayer);
    ui->displayArea->layout()->addWidget(ui->loadBar);

    FlickCharm *f = new FlickCharm(this);
    f->activateOn(displayer);
    connect(f, SIGNAL(leftSwipe()), displayer, SLOT(leftSwipe()));
    connect(f, SIGNAL(rightSwipe()), displayer, SLOT(rightSwipe()));
    connect(f, SIGNAL(swipeDone()), displayer, SLOT(swipeDone()));

    connect(displayer, SIGNAL(sourceChanged(QUrl)), this, SLOT(titleUpdate(QUrl)));

    connect(displayer, SIGNAL(loadStart()), this, SLOT(tdloadStarted()));
    connect(displayer, SIGNAL(loadEnd(QUrl, Book*, BookIter)), this, SLOT(tdloadFinished(QUrl, Book*, BookIter)));


    exclude << SETTINGS_PAGE<< MIXED_SELECTION_PAGE ;//<< HISTORY_PAGE;
    viewHistory = new QList<int>;
    //the base of history should always point to the main page
    viewHistory->append(MAIN_PAGE);
    connect(ui->stackedWidget, SIGNAL(currentChanged(int)), this, SLOT(viewChanged(int)));

    // setup the search page
    showHideSearch(false);

    //Setup flick charm on the treewidgets
    FlickCharm *fc = new FlickCharm(this);
    fc->activateOn(ui->treeWidget);
    fc->activateOn(ui->SearchTreeWidget);
    fc->activateOn(ui->bookMarkList);

    //Build the book list
    reloadBooklist();

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

    //load saved settings
    setupSettings();

    adjustToScreenSize();


    //Checking if books exist is irelevant. We need to check if the SD card works, but maybe not here...
    // IZAR- already checking for sdcard in java file.

    QApplication::processEvents();

    //Reopen last book, if relevant
    //get last open book
    QSettings settings("Orayta", "SingleUser");
    settings.beginGroup("History");
        int page = settings.value("lastPage").toInt();
        int lastBookId = settings.value("lastBook").toInt();
        Book *b = bookList.findBookById(lastBookId);
        BookIter itr = BookIter::fromEncodedString(settings.value("position", "").toString());
        vp =  settings.value("viewposition").toInt();
    settings.endGroup();

    // restore the bookmark list
    ui->bookMarkList->loadHistory(bookList);


    if (page != DISPLAY_PAGE || !b) ui->stackedWidget->setCurrentIndex(MAIN_PAGE);
    else
    {
        showBook(b, itr);

        //Yuchy hack. but I culdn't get it to work otherwise...
        QTimer::singleShot(100, this, SLOT(jumpToLastPos()));
    }

    ui->gtoHelp->show();

    ui->stackedWidget->currentWidget()->setFocus();

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
    qDebug() << "Screen:" << size;

    //Adjust main page icons:
    int w = (size.width() / 2);
    int max = 1000;

    //Round to the closest 20
    w = int(w / 20) * 20;

    QSize a(w -26, w - 26);
    qDebug() << "Icon size:" << a;

    int h = (size.height() / 18);
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


    ui->treeWidget->setColumnWidth(0, size.width()-20);

    // if the font size wasn't set manually by the user, we will geuss the best values
    // depending on target device dpi

    if (gFontSize < 1)
    {

        int dpix = desktop->physicalDpiX();
        int dpiy = desktop->physicalDpiY();
        int dpi = (dpix+dpiy)/2;

        qDebug() << "x: " << dpix << " y: " << dpiy << " avrage: " << dpi;


        //IZAR: this is a guess that must be tested deeper.
        int fontSize = 16;
        if (dpi >= 150) fontSize = 24;
        if (dpi >= 200) fontSize = 28;
        if (dpi >= 250) fontSize = 36;

        gFontSize = fontSize;

        QSettings settings("Orayta", "SingleUser");
        settings.beginGroup("Confs");
        //save current font settings
        settings.setValue("fontsize", gFontSize);
        settings.endGroup();
    }

    adjustFontSize();

}

//set global font size to ui.
void MobileApp::adjustFontSize()
{

    //IZAR TODO: find a better way to cange font size. this way breaks much of our styling.
    int fontSize = gFontSize;
    // font for menus should be about 50% of font for book-display.
    fontSize /= 2;
    QString styleSheet("font: " +QString::number(fontSize) +"pt;");
    ui->stackedWidget->setStyleSheet(styleSheet);
}

//Yuchy hack. but I culdn't get it to work otherwise...
void MobileApp::jumpToLastPos()
{
    ui->stackedWidget->currentWidget()->setFocus();
    displayer->setFocus();

    displayer->verticalScrollBar()->setValue(vp);
    vp = -1;
}

MobileApp::~MobileApp()
{
    qDebug() << "destructor";
    //Delete the old downloadable-books list
    QFile f(SAVEDBOOKLIST);
    f.remove();

    delete downloader;
    delete listdownload;

    delete ui;
}

//IZAR
// reload the whole book list and tree
void MobileApp::reloadBooklist(){

    //create a new empty booklist
    bookList = BookList();

    //Refresh book list
    ui->treeWidget->clear();

    bookList.BuildFromFolder(BOOKPATH);

    if (bookList.empty())
        qDebug()<<"can't find books in: " << BOOKPATH;

    // Check all uids
    bookList.CheckUid();

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

void MobileApp::showBook(Book *book, BookIter itr)
{
    ui->stackedWidget->setCurrentIndex(DISPLAY_PAGE);
    qApp->processEvents();

    //IZAR: temporary work-around. the problem is that orayta reads the global font settings ONLY on startup, and is careless if it is changed latter.
    //TODO: fix this.
    QFont font( gFontFamily, gFontSize );
    book->setFont(font);

    displayer->display(book, itr);

    // display mixed selection button only if the book has commentaries
    ui->mixedSelectBTN->setEnabled(book->IsMixed());
}

void MobileApp::showBook(Book *book)
{
    if (!book)
    {
        qDebug() << "Error! Can't open book";
        return;
    }

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
            ui->stackedWidget->setCurrentIndex(DISPLAY_PAGE);
            qApp->processEvents();

            //book->readBook(1);

            displayer->display(book);
            ui->bookNameLBL->setText(book->getNormallDisplayName());

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


void MobileApp::tdloadFinished(QUrl u, Book* book, BookIter iter)
{
   QApplication::processEvents();

   titleUpdate(u);
   displayer->setEnabled(true);
   ui->loadBar->hide();

    //IZAR- add a bookmark in new location
   if (book)
   {
       addBookMark(book, iter);
   }
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

    QSettings settings("Orayta", "SingleUser");

    //remmeber last open book
    settings.beginGroup("History");
    settings.setValue("lastPage", ui->stackedWidget->currentIndex());

    if (displayer->getCurrentBook()) settings.setValue("lastBook", displayer->getCurrentBook()->getUniqueId());

    settings.setValue("position", displayer->getCurrentIter().toEncodedString());
    settings.setValue("viewposition", displayer->verticalScrollBar()->value());
    settings.endGroup();

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

    ClearTmp();

    QDialog::close();
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

void MobileApp::keyReleaseEvent(QKeyEvent *keyEvent){

    switch ( keyEvent->key() )
    {

    //stop event sent from android. exit app
    case Qt::Key_MediaStop:
        close();
        break;

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
         return;
*/

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
        downloadDWList();
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
        // never add the same page twice
        if (viewHistory->at(viewHistory->size()-1) != index)
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
                viewHistory->removeLast();
                id = viewHistory->at(viewHistory->size()-1);
            }
            viewHistory->removeLast();
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

    adjustFontSize();

    //also, clear currently displayed book.

    // test if the previous view was the book itself. if so we want to reload the book.
    if (viewHistory->length() > 2 &&
          /* viewHistory->at(viewHistory->length()-2)->objectName() ==
           ui->stackedWidget->widget(DISPLAY_PAGE)->objectName()) */
            viewHistory->at(viewHistory->length()-2) == DISPLAY_PAGE)
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

            //qDebug() << groups[i].books[j].URL << groups[i].books[j].UnpackPath << groups[i].books[j].dateModified;
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

        //qDebug() << groups[i].name <<  groups[i].downloadState << groups[i].fullSize << groups[i].downloadSize;
    }



}

void MobileApp::downloadDWList()
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
            downloadDWList();
        }

    }
    else
        // this shouldn't happen, but os so we should recreate everything.
        downloadDWList();
}


void MobileApp::updateDownloadableList()
{
    //Test of new system:
    QList <QString> dl;
    dl.clear();
    groups.clear();

    ReadFileToList(SAVEDBOOKLIST, dl, "UTF-8");
    parseDLFile(dl);


    //show aprorpriate widgets
    ui->downloadSTKWidget->setCurrentIndex(1);

    //Show downloadble stuff:

    //Refresh the list
    ui->downloadListWidget->clear();

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
//        downloadDWList();

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


void MobileApp::on_downloadListWidget_itemClicked(QListWidgetItem *item)
{
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


void MobileApp::on_settingsMenuBTN_clicked()
{
    ui->stackedWidget->setCurrentIndex(SETTINGS_PAGE);
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
        QString defaultFont = "Droid Sans Hebrew Orayta";
        gFontFamily = settings.value("fontfamily", defaultFont).toString();

        //default set to 0. if it is so, adjustToScreenSize() will guess a better value depending on target screen dpi.
        gFontSize = settings.value("fontsize",0).toInt();
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

    bool showalone =true;

    for(int j=1; j<displayer->getCurrentBook()->mWeavedSources.size(); j++)
    {
        QString srcId = stringify(displayer->getCurrentBook()->mWeavedSources[j].id);

        for (int i =0; i< ui->selectionArea->count(); i++)
        {
            QListWidgetItem *item = ui->selectionArea->item(i);

              QString widgetId = item->whatsThis();

              if (srcId == widgetId) //we found the currect item
              {
                  bool checked = item->checkState() == Qt::Checked? true : false;

                  //set the showability of this item to what the user chose.
                  displayer->getCurrentBook()->mWeavedSources[j].show = checked;
                  if (checked) showalone = false;
                  break;
              }
        }
    }

    displayer->getCurrentBook()->showAlone = showalone;

    //show the book
    qApp->processEvents();
    showBook(displayer->getCurrentBook(), displayer->getCurrentIter());

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


void MobileApp::on_selectionArea_itemClicked(QListWidgetItem *item)
{
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


void MobileApp::addBookMark(Book * b, BookIter iter){
    // dont create a bookmark for an index page
    // TODO: let the user decide?
    if (iter.isEmpty()) return;

    ui->bookMarkList->newBookMark(b, iter);
}

void MobileApp::on_bookMarkList_itemClicked(QListWidgetItem *item)
{
    MiniBMark *bm= dynamic_cast<MiniBMark *>(item);
    if (!bm || bm == 0) return;
    BookIter it = bm->getBookIter();
    showBook(bm->getBook(), it);
    return;

}

void MobileApp::on_bookMarksBTN_clicked()
{
    ui->stackedWidget->setCurrentIndex(HISTORY_PAGE);
}

void MobileApp::on_helpBTN_clicked()
{
    on_gtoHelp_clicked();
}
