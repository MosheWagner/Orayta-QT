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

#include <QTextStream>
#include <QIcon>
#include <QUrl>
#include <QtWebKit>
#include <QtGui/QVBoxLayout>
#include <QtGui/QWidget>
#include <QtWebKit/QWebView>
#include <QString>
#include <QFile>
#include <QMenu>
#include <QPrinter>
#include <QTextBrowser>
#include <QPrintDialog>
#include <qevent.h>
#include <qprocess.h>
#include <QMessageBox>
#include <QDesktopServices>
#include <QInputDialog>
#include <QMouseEvent>
#include <QShortcut>
#include <QFuture>
#include <QFontDialog>

#include "../OraytaBase/functions.h"
#include "../OraytaBase/htmlgen.h"
#include "../OraytaBase/search.h"
#include "../OraytaBase/quazip/quazip.h"
#include "../OraytaBase/quazip/quazipfile.h"
#include "../OraytaBase/bookfind.h"

#include "desktopapp.h"

#include "addcomment.h"
#include "mywebview.h"
#include "about.h"
#include "settings.h"
#include "errorreport.h"
#include "bookdisplayer.h"
#include "mytreetab.h"
#include "importbook.h"


/*
  Roadmap for 0.9:

  - Allow printing. (Html + Pdf) If it's really hard, html-s can become pdf-s and then printed

  - Improve all search stuff (including better result page. using side bar?)

  - Solve all book issues (wierd nikud, a few books that render incorrectly, better book names in tree)

  - Improve GUI. (Remove search bar? Change in and out of search icons)
*/


/*
  Search issues:

    - TODO: Search in html files too
    - TODO: Seach in pdf files too

    - TODO: Search with no result dosn't need a new tab (Msgbox?)
    - TODO: Search result marks in books with nikud don't work
    - TODO: Improve search result preview

    - TODO: Allow finding ktiv too (only kri is found now)

    - TODO: Make auto DB building an option.
    - TODO: Make DB in files an option
*/

//TODO: Added books should have a "remove" option in the context menu.
//TODO: All books should have a "book settings" option via the menu too.
//TODO: Add the book settings whatever (dialog ?)

//TODO: Menu entries don't go RTL. is it QT's bug?
//TODO: Bracets in hebrew text flipped. QT bug?

//TODO: Finish GUI changes

//TODO: JS function "ClosestElementToView" isn't accurate. (Maybe jump there but without the red mark?)

//TODO: Escape books with wierd chars in their name. ( ' , " , \ , / ). (At import)

//TODO: Improve current position detection

//TODO: dictionary

//TODO: Translatable welcome page
//TODO: create help page for linux version

//TODO: document & clean code

//TODO: Requested Feature: parallel tab (like in the originall)

//TODO: see what else to do ;-)


// Global
//IZAR- changed this to use dejavu sans free font in all versions (linux, windows and android)
//QString gFontFamily = "Nachlieli CLM";
QString gFontFamily = "Droid Sans Hebrew Orayta";
int gFontSize = 16;



// This is just a very simple define. anywhere in the code,
//  "CURRENT_TAB" simply represents "ui->viewTab->currentIndex()".
//
//  It just made the code so much more readable, I couldn't resist
#define CURRENT_TAB ui->viewTab->currentIndex()

DesktopApp::DesktopApp(QWidget *parent) : QMainWindow(parent), ui(new Ui::DesktopAppClass)
{
    //Code executed when the window is constructed:

    //Setup the GUI
    ui->setupUi(this);

    //Remove all temporary html files the program created
    ClearTmp();

    //Loading confs
    restoreConfs();

    //Set all QString to work with unicode
    QTextCodec::setCodecForCStrings(QTextCodec::codecForName("utf8"));
    QTextCodec::setCodecForTr(QTextCodec::codecForName("utf8"));

    ui->treeDockWidget->setFloating(false);

    //Adjust for hebrew if needed
    if (LANG == "Hebrew") setDirection(true);

    //Force the viewTab to take 80 percent of the window
    ui->verticalLayout_12->setStretchFactor(ui->viewTab, 80);
    ui->verticalLayout_12->setStretchFactor(ui->verticalLayout_9, 20);

    //Close the default tab:
    ui->viewTab->removeTab(0);
    //Now add a good one:
    addViewTab(false);
    //Set window title:
    setWindowTitle(tr("Orayta"));

    //Generate book list and display it
    updateBookTree();

    //Load mixed display confs
    restoreBookConfs();

    /*
    //Sort the book tree by the second column ( that holds the whole name, including the number before it)
    ui->treeWidget->setColumnCount(2);  //Small hack to allow sorting the right way...
    ui->treeWidget->sortItems(1, Qt::AscendingOrder);
    ui->treeWidget->setColumnCount(1);
    */

    //Load start page. Assuming it's book[0] of course.
    if (!bookList[0]->IsDir())
    {
        //@@@@
        //bookdisplayer(0)->setInternalLocation("*");
        openBook(bookList[0]);
    }
    else
    {
        bookdisplayer(0)->setHtml(simpleHtmlPage(tr("Orayta"), tr("Jewish books")));
        ui->viewTab->setTabText(CURRENT_TAB, tr("Orayta"));
    }

    //Corner Widget : contain standard buttons & pdf buttons
    QWidget* cornerWidget = new QWidget;
    QHBoxLayout* cornerWidgetLayout = new QHBoxLayout;
    cornerWidgetLayout->setContentsMargins(2,0,2,0);

#ifdef POPPLER
    //Pdf buttons:
    //cornerWidgetLayout->addWidget(ui->pdfPageSpin);
    cornerWidgetLayout->addWidget(ui->pdfDropBox);
    cornerWidgetLayout->addWidget(ui->PDFpageLBL);
    cornerWidgetLayout->addSpacing(45);
#endif

    ui->btnBox->addButton((QAbstractButton *)ui->refreshButton, QDialogButtonBox::ActionRole);
    ui->btnBox->addButton((QAbstractButton *)ui->newTabButton, QDialogButtonBox::ActionRole);
    //ui->btnBox->addButton((QAbstractButton *)ui->topButton, QDialogButtonBox::ActionRole);
    ui->btnBox->addButton((QAbstractButton *)ui->zoominButton, QDialogButtonBox::ActionRole);
    ui->btnBox->addButton((QAbstractButton *)ui->zoomoutButton, QDialogButtonBox::ActionRole);
    ui->btnBox->addButton((QAbstractButton *)ui->showSearchBarButton, QDialogButtonBox::ActionRole);
    ui->btnBox->addButton((QAbstractButton *)ui->backBTN, QDialogButtonBox::ActionRole);
    ui->btnBox->addButton((QAbstractButton *)ui->rightChap, QDialogButtonBox::ActionRole);
    ui->btnBox->addButton((QAbstractButton *)ui->leftChap, QDialogButtonBox::ActionRole);

    cornerWidgetLayout->addWidget(ui->btnBox);

    cornerWidget->setLayout(cornerWidgetLayout);
    ui->viewTab->setCornerWidget(cornerWidget, Qt::TopRightCorner);


    ui->searchGroupBX->hide();
    ui->showSearchBarButton->setChecked(false);

    ui->pbarbox->hide();

    ui->mixedGroup->hide();

    ui->treeWidget->setHeaderHidden(true);

    //Hack for windows:
    ui->treeWidget->setColumnWidth(0,350);

    //Load the bookmarks into their list:
    ui->bookMarkList->loadHistory(bookList);

    connectMenuActions();

    adjustMenus();

    stopSearchFlag = false;


    /* -------------------------
    //Output a new conf file:
    QString c="";
    for (int i=0; i<gBookList.size(); i++)
        if (!gBookList[i]->IsDir())
        {
            c+= "* " + fixPath(gBookList[i]->getPath()) + "\n";
            c+=gBookList[i]->confEntry();
        }
    print (c);
     --------------------------- */

    //Build the search DB while the program is running
    //QtConcurrent::run(buidSearchDBinBG, &bookList);
}

//Switch GUI direction to RTL
void DesktopApp::setDirection(bool rtl)
{
    Qt::LayoutDirection dir;
    Qt::DockWidgetArea dwa;

    if (rtl == true)
    {
        dir = Qt::RightToLeft;
        dwa = Qt::RightDockWidgetArea;
    }
    else
    {
        dir = Qt::LeftToRight;
        dwa = Qt::LeftDockWidgetArea;
    }

    this->setLayoutDirection(dir);

    ui->progressBar->setLayoutDirection(dir);

    ui->searchGroupBX->setLayoutDirection(dir);

    ui->treeDockWidget->setAllowedAreas(dwa);

    removeDockWidget(ui->treeDockWidget);
    addDockWidget(dwa, ui->treeDockWidget);
    ui->treeDockWidget->show();

    ui->treeTab->setLayoutDirection(dir);
    ui->booksTab->setLayoutDirection(dir);
    ui->searchTab->setLayoutDirection(dir);
    ui->bookmarkTab->setLayoutDirection(dir);
    ui->viewTab->setLayoutDirection(dir);
    ui->mixedGroup->setLayoutDirection(dir);
    ui->mixedFrame->setLayoutDirection(dir);

    ui->treeWidget->setLayoutDirection(dir);

    if (rtl == true)
    {
        ui->searchForward->setIcon(QIcon(":/Icons/search-left.png"));
        ui->searchBackward->setIcon(QIcon(":/Icons/search-right.png"));

        ui->searchForwardAction->setIcon(QIcon(":/Icons/search-left.png"));
        ui->searchBackwardAction->setIcon(QIcon(":/Icons/search-right.png"));
    }
    else
    {
        ui->searchForward->setIcon(QIcon(":/Icons/search-right.png"));
        ui->searchBackward->setIcon(QIcon(":/Icons/search-left.png"));

        ui->searchForwardAction->setIcon(QIcon(":/Icons/search-right.png"));
        ui->searchBackwardAction->setIcon(QIcon(":/Icons/search-left.png"));
    }

    ui->menuBar->setLayoutDirection(dir);

    ui->menu->setLayoutDirection(dir);
    ui->menu_2->setLayoutDirection(dir);
    ui->menu_3->setLayoutDirection(dir);
    ui->locationMenu->setLayoutDirection(dir);
    ui->menu_5->setLayoutDirection(dir);
}

void DesktopApp::connectMenuActions()
{
    connect(ui->aboutAction, SIGNAL(triggered()), this, SLOT(about()));
    connect(ui->zoominAction, SIGNAL(triggered()), this, SLOT(on_zoominButton_clicked()));
    connect(ui->zoomoutAction, SIGNAL(triggered()), this, SLOT(on_zoomoutButton_clicked()));
    connect(ui->jumptotopAction, SIGNAL(triggered()), this, SLOT(on_topButton_clicked()));
    //connect(ui->printAction, SIGNAL(triggered()), this, SLOT(printBook()));

    connect(ui->findBookAction, SIGNAL(triggered()), this, SLOT(findBookForm()));
    connect(ui->opentabAction, SIGNAL(triggered()), this, SLOT(on_newTabButton_clicked()));
    connect(ui->closetabAction, SIGNAL(triggered()), this, SLOT(closeCurrentTab()));

    connect(ui->showNikudAction, SIGNAL(triggered(bool)), this, SLOT(toggleNikud(bool)));
    connect(ui->showTeamimAction, SIGNAL(triggered(bool)), this, SLOT(toggleTeamim(bool)));

    connect(ui->bookMarkAction, SIGNAL(triggered()), this, SLOT(menuBookMark()));
    connect(ui->commentAction, SIGNAL(triggered()), this, SLOT(menuComment()));
    connect(ui->reportErrorAction, SIGNAL(triggered()), this, SLOT(menuErrorReport()));

    connect(ui->advancedSearchAction, SIGNAL(triggered()), this, SLOT(showSearchTab()));
    connect(ui->searchForwardAction, SIGNAL(triggered()), this, SLOT(on_searchForward_clicked()));
    connect(ui->searchBackwardAction, SIGNAL(triggered()), this, SLOT(on_searchBackward_clicked()));

    connect(ui->addAllToSearchAction, SIGNAL(triggered()), this, SLOT(on_addAllToSearchButton_clicked()));
    connect(ui->removeAllFromSearchAction, SIGNAL(triggered()), this, SLOT(on_removeAllFromSearchButton_clicked()));
    connect(ui->addToSearchAction, SIGNAL(triggered()), this, SLOT(addToSearch()));
    connect(ui->removeFromSearchAction, SIGNAL(triggered()), this, SLOT(removeFromSearch()));

    connect(ui->actionSettings, SIGNAL(triggered()), this, SLOT(settingsForm()));
    connect(ui->actionImport_books, SIGNAL(triggered()), this, SLOT(importForm()));

    connect(ui->actionSearchGuematria, SIGNAL(triggered()), this, SLOT(searchGuematria()));
}

DesktopApp::~DesktopApp()
{
    //TODO: Don't I need to free all the items?
    bookList.clear();

    ClearTmp();

    delete ui;
}

//Remove all temporary html files the program created
void DesktopApp::ClearTmp()
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

//Overrides the normal "closeEvent", so it can save tha window's state before quiting
void DesktopApp::closeEvent(QCloseEvent *event)
{
    //Cancel any running searches
    stopSearchFlag = true;

    QSettings settings("Orayta", "SingleUser");

    settings.beginGroup("DesktopApp");
    settings.setValue("size", size());
    settings.setValue("pos", pos());
    settings.setValue("state", saveState());
    settings.endGroup();


    settings.beginGroup("Confs");
    settings.setValue("fontfamily", gFontFamily);
    settings.setValue("fontsize", gFontSize);
    settings.endGroup();


    //Save books' settings
    for(unsigned int i=0; i<bookList.size(); i++)
    {
        if (bookList[i]->getUniqueId() == -1 || bookList[i]->hasRandomId)
            continue;

        settings.beginGroup("Book" + stringify(bookList[i]->getUniqueId()));
            settings.setValue("MixedDisplayes", bookList[i]->mWeavedSources.size());
            settings.setValue("ShowAlone", bookList[i]->showAlone);
            for (int j=0; j<bookList[i]->mWeavedSources.size(); j++)
            {
                settings.setValue("Shown" + stringify(j), bookList[i]->mWeavedSources[j].show);
            }
            settings.setValue("InSearch", bookList[i]->IsInSearch());
        settings.endGroup();
    }
}

//Restores the window's state from the last run
void DesktopApp::restoreConfs()
{

    QSettings settings("Orayta", "SingleUser");

    settings.beginGroup("DesktopApp");
    QDesktopWidget *widget = QApplication::desktop();
    int desktop_width = widget->width();
    int desktop_height = widget->height();

    restoreState(settings.value("state", "").toByteArray());
    //The previous line causes problems when switching languages. So:
    if (LANG != "Hebrew")
    {
        ui->treeDockWidget->setAllowedAreas(Qt::LeftDockWidgetArea);
        removeDockWidget(ui->treeDockWidget);
        addDockWidget(Qt::LeftDockWidgetArea, ui->treeDockWidget);
        ui->treeDockWidget->show();
    }

    resize(settings.value("size", QSize(desktop_width, desktop_height-50)).toSize());
    move(settings.value("pos", QPoint(0, 0)).toPoint());
    settings.endGroup();


    settings.beginGroup("Confs");
//    gFontFamily = settings.value("fontfamily", "Nachlieli CLM").toString();
    gFontFamily = settings.value("fontfamily", "Droid Sans Hebrew Orayta").toString();
    gFontSize = settings.value("fontsize",16).toInt();

    settings.endGroup();
}

void DesktopApp::restoreBookConfs()
{
    /*
    QSettings settings("Orayta", "SingleUser");

    //Load books settings
    for(unsigned int i=0; i<bookList.size(); i++)
    {
        if (bookList[i]->getUniqueId() == -1)
            continue;

        settings.beginGroup("Book" + stringify(bookList[i]->getUniqueId()));
            bookList[i]->showAlone = settings.value("ShowAlone", false).toBool();
            int n = settings.value("MixedDisplayes", 0).toInt();
            for (int j=0; j<n; j++)
            {
                bookList[i]->mWeavedSources[j].show = settings.value("Shown" + stringify(j), true).toBool();
            }
            bool inSearch = settings.value("InSearch", false).toBool();
            if (inSearch) bookList[i]->select();
            //else bookList[i]->unselect();
        settings.endGroup();
    }
    */
}

void DesktopApp::BuildBookTree()
{
    bookList.displayInTree(ui->treeWidget, true);

    //Only after the tree is done, connect the itemChanged signal
    connect (ui->treeWidget, SIGNAL(currentItemChanged(QTreeWidgetItem*,QTreeWidgetItem*)), this, SLOT(treeWidgetSelectionChanged(QTreeWidgetItem*,QTreeWidgetItem*)));
}

void DesktopApp::updateBookTree()
{
    ui->treeWidget->clear();

    static unsigned end;

    if ( bookList.empty() )
    {
        bookList.BuildFromFolder(BOOKPATH);
        end = bookList.size();
    }
    else
    {
        //Erase user books only
        for (BookList::iterator it = bookList.begin() + end; it != bookList.end(); ++it)
            delete (*it);

        bookList.erase(bookList.begin() + end, bookList.end());
    }

    //Add user-added books
    bookList.BuildFromFolder(USERPATH + "Books", true);

    qDebug() << USERPATH + "Books";

    /*
      TODO: Re-enable this eventually
    //Create SearchDB for user added books
    for (int i=0; i<bookList.size(); i++)
    {
        if (bookList[i]->getPath().startsWith(USERPATH + "Books"))
        {
            bookList[i]->BuildSearchTextDB();
        }
    }
    */

    if (bookList.empty())
    {
        QMessageBox msgBox;
        msgBox.setText(tr("No books found! \nCheck your installation, or contact the developer."));
        msgBox.exec();

        //No books found
        exit(2);
    }

    // Check alls uids
    bookList.CheckUid();
    //Insert all the books in the tree Widget
    BuildBookTree();

    //Restore books icons state
    for (vector<Book*>::iterator it = bookList.begin(); it != bookList.begin() + end; ++it)
    {
        (*it)->restoreIconeState();
    }
}


//Load the generated Html file of the given book into the WebView widget
void DesktopApp::RenderAndLoad(Book *book, QString markString)
{
    bool shownikud = ui->showNikudAction->isChecked();
    bool showteamim = ui->showTeamimAction->isChecked();

    //Show the wait sign even before rendering
    CurrentBookdisplayer()->ShowWaitPage();

    ui->viewTab->setTabText(CURRENT_TAB, tr("Loading..."));

    //Generate filename representing this file, the commentreis that should open, and it's nikud (or teamim) mode
    //  This way the file is rendered again only if it needs to be shown differently (if other commenteries were requested)
    QString htmlfilename = book->HTMLFileName() + "_" + stringify(shownikud) + stringify(showteamim) + ".html";

    //Check if file already exists. If not, make sure it renders ok.
    QFile f(htmlfilename);
    bool renderedOK = true;

    if (!f.exists() || markString.simplified() != "")
    {
        //TODO: @@@

        //renderedOK = book->htmlrender(htmlfilename, shownikud, showteamim, markString);
    }
    if (renderedOK == true)
    {
        QString p =  absPath(htmlfilename);

        CurrentBookdisplayer()->load(QUrl::fromLocalFile(p));
    }
    else
    {
        CurrentBookdisplayer()->HideWaitPage();

        CurrentBookdisplayer()->setHtml( tr("<h2>An error occured...</h2><br />"
                                            "Please report this bug : 'http://code.google.com/p/orayta/issues/entry'") );

        ui->treeWidget->setCurrentItem(NULL);

        return;
    }

    //Dirty hack to expand tree to the opend book.
    QList <QTreeWidgetItem *> nodes;

    QTreeWidgetItem * parent = book->getTreeItemPtr();
    while (parent != NULL)
    {
        nodes.append(parent);
        parent = parent->parent();
    }

    for (int i=nodes.size()-1; i>=0; i--)
    {
        ui->treeWidget->expandItem(nodes[i]);
    }

    ui->treeWidget->setCurrentItem(book->getTreeItemPtr());
}

//Open the given book (by it's id in the booklist)
void DesktopApp::openBook( Book* book )
{
    if ( book && book->IsDir() == false )
    {
        // erase tabWidget of the previous book
        if (CurrentBookdisplayer()->book() != NULL)
            CurrentBookdisplayer()->book()->setTabWidget( 0 );

#ifdef POPPLER
        //Default
        CurrentBookdisplayer()->setPdfMode(false);
#endif

        switch ( book->fileType() )
        {
        case ( Book::Normal ):

            if (CurrentBookdisplayer()->InternalLocationInHtml == "")
            {
                //TODO: Who said 1 is the best?
                book->readBook(1);
                QUrl u = book->renderBookIndex();
                CurrentBookdisplayer()->load(u);
            }
            else
            {
                QString l = CurrentBookdisplayer()->InternalLocationInHtml;

                //Remove the "#"
                l = l.mid(1);

                qDebug() << l;
                BookIter itr = BookIter::fromEncodedString(l);

                QUrl u = book->renderChapterHtml(&itr, &bookList, true, true);
                CurrentBookdisplayer()->load(u);
                CurrentBookdisplayer()->currentLocation = itr;

                CurrentBookdisplayer()->InternalLocationInHtml = "";
            }

            break;

        case ( Book::Html ):
            CurrentBookdisplayer()->load( QUrl::fromLocalFile(book->getPath()) );
            break;

        case ( Book::Pdf ):
#ifdef POPPLER
            CurrentBookdisplayer()->setPdfMode(true);
            CurrentBookdisplayer()->load ( QUrl::fromLocalFile(book->getPath()) );
            ui->viewTab->setTabText(CURRENT_TAB, book->getNormallDisplayName());
#else

            CurrentBookdisplayer()->enablePlugins();
            CurrentBookdisplayer()->setHtml(pluginPage(book->getNormallDisplayName()));
            if ( CurrentBookdisplayer()->execScript("testPdfPlugin()").toString() == "yes" )
            {
                CurrentBookdisplayer()->load( QUrl::fromLocalFile( "file:///" + book->getPath() ) );
                ui->viewTab->setTabText(CURRENT_TAB, book->getNormallDisplayName());
            }
#endif
            break;

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

                if (lId != -1) openBook( bookList.findBookById(lId) );
                else qDebug("Invalid link!");

                break;
            }
        default:
            qDebug("Unkown file type");
            break;
        }

        // set tabWidget of this book
        book->setTabWidget(ui->viewTab->currentWidget());

        CurrentBookdisplayer()->setBook(book);

        adjustMenus();
    }
}

//Calls "openBook", but uses the currently selected book in the tree
void DesktopApp::openSelectedBook( )
{
    Book* book = bookList.findBookByTWI(ui->treeWidget->selectedItems()[0]);
    openBook( book );
}

//Opens a new tab and Calls "openBook" (using the currently selected book in the tree).
void DesktopApp::openSelectedBookInNewTab()
{
    addViewTab(false);
    openSelectedBook();
}

void DesktopApp::changeFont()
{
    Book* book = bookList.findBookByTWI(ui->treeWidget->selectedItems()[0]);
    QFont currentFont = book->getFont();

    bool ok;
    QFont font = QFontDialog::getFont(&ok, QFont(currentFont.family(), currentFont.pointSize()), this);
    if (ok)
    {
        book->setFont(font);
    }
}

void DesktopApp::deleteSelectedBook()
{
    QTreeWidgetItem * selectedItem = ui->treeWidget->selectedItems()[0];

    Book* book = bookList.findBookByTWI( selectedItem );
    QString path = book->getPath();

    QString displayText = tr("Are you sure you want to remove this ")
                          + ( book->IsDir() ? tr("books directory ?\nThis will remove all the books in this directory.")
                                            : tr("book ?") );

    int ret = QMessageBox::warning(this, tr("Deleting book"),
                                   displayText,
                                   QMessageBox::Ok | QMessageBox::Cancel,
                                   QMessageBox::Cancel);
    switch (ret) {
    case QMessageBox::Ok:

        if (book->IsDir())
        {
            deleteBooksFolder( path );
        }
        else
        {
            if ( !QFile::remove( path ) )
                qDebug() << "Couldn't remove file: " << path;
            if ( book->fileType() == Book::Normal )
            {
                QString confFilename = path.replace(".txt", ".conf", Qt::CaseInsensitive);
                QFile confFile(confFilename);
                if ( confFile.exists() )
                    confFile.remove();
            }
        }
        updateBookTree();
        break;
    case QMessageBox::Cancel:
        break;
    default:
        break;
    }
}

void DesktopApp::on_newTabButton_clicked()
{
    //Create the new tab and switch to it
    addViewTab(true);
}

void DesktopApp::addViewTab(bool empty)
{
    //Create new tab:
    bookDisplayer * bd = new bookDisplayer(this, ui->viewTab);
    ui->viewTab->addTab(bd, tr("Orayta"));

    if (empty == true) bd->setHtml(simpleHtmlPage(tr("Orayta"), ""));

    //Switch to the new tab
    ui->viewTab->setCurrentIndex(ui->viewTab->count()-1);

    ui->viewTab->setTabText(CURRENT_TAB, tr("Orayta"));

    connect(bd, SIGNAL(externalLink(QString)), this, SLOT(openExternalLink(QString)));
}

void DesktopApp::on_lineEdit_returnPressed()
{
    on_searchForward_clicked();
}

void DesktopApp::addBookToSearch(Book* book)
{
    if ( book )
        book->select();

    ui->addToSearchAction->setEnabled(false);
    ui->removeFromSearchAction->setEnabled(true);
}

void DesktopApp::addToSearch()
{
    Book* book = bookList.findBookByTWI(ui->treeWidget->currentItem());
    addBookToSearch(book);
}

void DesktopApp::on_addAllToSearchButton_clicked()
{
    for(unsigned int i=0; i<bookList.size(); i++)
    {
        if ( bookList[i]->IsUserCheckable(bookList[i]->getTreeItemPtr() ) )
            bookList[i]->select();
    }

    ui->addToSearchAction->setEnabled(false);
    ui->removeFromSearchAction->setEnabled(true);
}

void DesktopApp::removeBookFromSearch(Book* book)
{
    if ( book )
        book->unselect();

    ui->addToSearchAction->setEnabled(true);
    ui->removeFromSearchAction->setEnabled(false);
}

void DesktopApp::removeFromSearch()
{
    Book* book = bookList.findBookByTWI(ui->treeWidget->currentItem());
    removeBookFromSearch(book);
}

void DesktopApp::on_removeAllFromSearchButton_clicked()
{
    for(unsigned int i=0; i<bookList.size(); i++)
        bookList[i]->unselect();

    ui->addToSearchAction->setEnabled(true);
    ui->removeFromSearchAction->setEnabled(false);
}

void DesktopApp::on_zoominButton_clicked()
{
    CurrentBookdisplayer()->ZoomIn();
}

void DesktopApp::on_zoomoutButton_clicked()
{
    CurrentBookdisplayer()->ZoomOut();
}

void DesktopApp::on_topButton_clicked()
{
    CurrentBookdisplayer()->jumpToTop();
}

void DesktopApp::on_backBTN_clicked()
{
    CurrentBookdisplayer()->jumpToTop();
    //CurrentBookdisplayer()->goBack();
}

void DesktopApp::on_refreshButton_clicked()
{
    if (CurrentBookdisplayer()->getPdfMode()) return;

    Book* book = CurrentBookdisplayer()->book();
    if ( book != NULL )
    {
        bool shownikud = ui->showNikudAction->isChecked();
        bool showteamim = ui->showTeamimAction->isChecked();

        //Generate filename representing this file, the commentreis that should open, and it's nikud (or teamim) mode
        //  This way the file is rendered again only if it needs to be shown differently (if other commenteries were requested)
        QString htmlfilename = book->HTMLFileName() + "_" + stringify(shownikud) + stringify(showteamim) + ".html";

        QFile::remove(htmlfilename);

        RenderAndLoad(book);
    }
}


void DesktopApp::on_treeWidget_customContextMenuRequested(QPoint pos)
{
    QTreeWidgetItem * item = 0 ;
    item = ui->treeWidget->itemAt(pos);

    Book* book = NULL;
    if ( item != 0 )
    {
        book = bookList.findBookByTWI(item);
    }

    if( book )
    {
        bool setMenu = false;
        QMenu menu(ui->treeWidget);

        if ( book->IsDir() == false )
        {
            QAction *openbook = new QAction(tr("Open book"), &menu);
            openbook->setIcon(QIcon(":/Icons/book-blue.png"));
            QAction *openbooknewtab = new QAction(tr("Open in new tab"), &menu);
            openbooknewtab->setIcon(QIcon(":/Icons/tab-new.png"));

            QObject::connect(openbook, SIGNAL(triggered()), this , SLOT(openSelectedBook()));
            QObject::connect(openbooknewtab, SIGNAL(triggered()), this , SLOT(openSelectedBookInNewTab()));

            menu.addAction(openbook);
            menu.addAction(openbooknewtab);

            setMenu = true;
        }
        if ( book->IsUserBook() == true )
        {
            QAction *deleteBook = new QAction(tr("Delete book"), &menu);
            deleteBook->setIcon(QIcon(":/Icons/edit-delete.png"));

            QObject::connect(deleteBook, SIGNAL(triggered()), this, SLOT(deleteSelectedBook()));

            menu.addAction(deleteBook);

            setMenu = true;
        }
        else
        {
            QAction *changefont = new QAction(tr("Change font"), &menu);
            changefont->setIcon(QIcon(":/Icons/font.png"));

            QObject::connect(changefont, SIGNAL(triggered()), this , SLOT(changeFont()));

            menu.addAction(changefont);

            setMenu = true;
        }

        if ( setMenu )
        {
            menu.setLayoutDirection(Qt::RightToLeft);
            //Open the menu to the left of the cursor's position
            QPoint point = QPoint(QCursor::pos().x() - menu.width(), QCursor::pos().y());
            menu.exec(point);
        }
    }
}

//Collapse the currently selected item in the treewidget
void DesktopApp::collapseItem()
{
    ui->treeWidget->collapseItem(ui->treeWidget->selectedItems()[0]);
}

//Expand the currently selected item in the treewidget
void DesktopApp::expandItem()
{
    ui->treeWidget->expandItem(ui->treeWidget->selectedItems()[0]);
}

//Deals with keyboard events
void DesktopApp::keyPressEvent( QKeyEvent *keyEvent )
{
    //Enter pressed
    if ( (keyEvent->key() == Qt::Key_Return) || (keyEvent->key() == Qt::Key_Enter) )
    {
        if (ui->treeWidget->hasFocus())
        {
            Book* book = bookList.findBookByTWI(ui->treeWidget->selectedItems()[0]);
            if ( book )
            {
                if ( book->IsDir() )
                {
                    if ( ui->treeWidget->selectedItems()[0]->isExpanded() )
                        ui->treeWidget->collapseItem(ui->treeWidget->selectedItems()[0]);
                    else
                        ui->treeWidget->expandItem(ui->treeWidget->selectedItems()[0]);
                }
                else openBook(book);
            }
        }
        else if (ui->bookMarkList->hasFocus())
        {
             //Act as if the current item was clicked
            on_bookMarkList_itemDoubleClicked(ui->bookMarkList->currentItem());
        }
    }
    //Ctrl-F, show "search in books" bar
    if ( keyEvent->key() == Qt::Key_F && keyEvent->modifiers() == Qt::CTRL )
    {
        if (    CurrentBookdisplayer()->book()->fileType() == Book::Normal
             || CurrentBookdisplayer()->book()->fileType() == Book::Pdf
             || CurrentBookdisplayer()->book()->fileType() == Book::Html )
            ToggleSearchBar();
    }
    //Ctrl-T, new tab
    if ( keyEvent->key() == Qt::Key_T && keyEvent->modifiers() == Qt::CTRL )
    {
        on_newTabButton_clicked();
    }
    //Ctrl-W, close tab
    if ( keyEvent->key() == Qt::Key_W && keyEvent->modifiers() == Qt::CTRL )
    {
        closeCurrentTab();
    }
}

/*
//Print the current book (all pages I think...)
void DesktopApp::printBook()
{
    QPrinter printer;

    QPrintDialog *dialog = new QPrintDialog(&printer, this);
    dialog->setWindowTitle("הדפס ספר");
    if (dialog->exec() != QDialog::Accepted)
        return;
    else
    {
        CurrentBookdisplayer()->print(&printer);
    }
}
*/

//Show the comment adding / editing dialog
void DesktopApp::openCommentDialog(QString link)
{
    QString previosComment = "";

    //Read all comments (to see if one allready exists here)
    vector<QString> comment_titles, comment_texts;
    ReadCommentFile(USERPATH + "CommentList.txt", comment_titles, comment_texts, "UTF-8");

    //See if there is a match in the titles
    vector<QString>::iterator vitr = std::find(comment_titles.begin(), comment_titles.end(), link);
    int index = distance (comment_titles.begin (), vitr);

    if (index != comment_titles.size())
    {
        previosComment = comment_texts[index];
    }

    AddComment * addcommentdialog = new AddComment(this, link, previosComment.replace("\\|", "|").replace("|", "<BR>"));

    //Connect the dialog's "commentWritten" signal to the window's "commentAtPosition" slot
    connect(addcommentdialog, SIGNAL(commentWritten(QString,QString)), this, SLOT(addCommentAtPosition(QString,QString)));

    addcommentdialog->show();
}


//Add the given comment at the given book position (called from the "add comment" dialog's signal)
void DesktopApp::addCommentAtPosition(QString link, QString comment)
{
    //Escape the comment's text
    QString text  = comment.replace("|", "\\|").replace("\n", "|");

    //Read all comments (to see if one allready exists here)
    vector<QString> comment_titles, comment_texts;
    ReadCommentFile(USERPATH + "CommentList.txt", comment_titles, comment_texts, "UTF-8");

    //See if there is a match in the titles
    vector<QString>::iterator vitr = std::find(comment_titles.begin(), comment_titles.end(), link);
    int index = distance (comment_titles.begin (), vitr);

    //If it allready has a comment, rewrite the file without it
    if (index != comment_titles.size())
    {
        comment_titles.erase( comment_titles.begin() + index);
        comment_texts.erase( comment_texts.begin() + index);

        QString str = "";
        for (unsigned int i=0; i<comment_titles.size(); i++)
        {
            str += comment_titles[i] + "\n" + comment_texts[i] + "\n";
        }
        writetofile(USERPATH + "CommentList.txt", str, "UTF-8", true);
    }

    //In any case, add the new comment (if it isn't empty) to the file
    if ( comment != "" )
        writetofile(USERPATH + "CommentList.txt", link + "\n" + text + "\n", "UTF-8", false);

    //Add this commen into the page
    QString lnk = link.mid(link.indexOf(":") + 1);
    QString script = "addComment(\"" + lnk + "\",\"" + comment + "\");";
    CurrentBookdisplayer()->execScript(script);

    //Recreate this page with new comment in other thread (new render of the book)
    bool shownikud = CurrentBookdisplayer()->isNikudShown();
    bool showteamim = CurrentBookdisplayer()->areTeamimShown();
    Book* book = CurrentBookdisplayer()->book();
    QString htmlfilename = book->HTMLFileName() + "_" + stringify(shownikud) + stringify(showteamim) + ".html";

    //TODO: @@@
    //QtConcurrent::run( book, &Book::htmlrender, htmlfilename, shownikud, showteamim, QString("") );

}

void DesktopApp::removeComment(QString link)
{
    //Adding an emtpy comment removes the existing one
    addCommentAtPosition(link, "");
}

void DesktopApp::menuComment()
{
    QString link = CurrentBookdisplayer()->activeLink().replace("$","");

    //Find book's id and add it to the link
    int id = CurrentBookdisplayer()->book()->getUniqueId();
    link = stringify(id) + ":" + link;

    openCommentDialog( link );
}

void DesktopApp::about()
{
    About *aboutfrm = new About(this);

    aboutfrm->show();
}

void DesktopApp::closeCurrentTab()
{
    on_viewTab_tabCloseRequested(CURRENT_TAB);
}

void DesktopApp::toggleNikud(bool showNikud)
{
    CurrentBookdisplayer()->showNikud(showNikud);
    RenderAndLoad(CurrentBookdisplayer()->book() );
}

void DesktopApp::toggleTeamim(bool showTeamim)
{
    CurrentBookdisplayer()->showTeamim(showTeamim);
    RenderAndLoad( CurrentBookdisplayer()->book() );
}

void DesktopApp::on_viewTab_currentChanged(int index)
{
    if ( index != -1 )
    {
        //Show / hide menu options depending on the book:
       // if ( bookDisplayerList.size() > index && bookDisplayerList.size() > 0)
        {
            adjustMenus();
        }

#ifdef POPPLER
        if ( bookdisplayer(index)->book() != NULL && bookdisplayer(index)->getPdfMode() == true )
        {
            PdfWidget* pdfWidget = bookdisplayer(index)->pdfview();
            if ( pdfWidget )
            {
                updatePdfPage(pdfWidget->currentPage(), pdfWidget->numPages());
            }
        }
#endif

    }
}


// Show/hide buttons and menus depending on the currently visible book
void DesktopApp::adjustMenus()
{
    bool isWebkitPdf = false;
    if (CurrentBookdisplayer()->book() != NULL)
    {
        if (CurrentBookdisplayer()->book()->getName().endsWith(".pdf", Qt::CaseInsensitive)) isWebkitPdf = true;
    }


    if (CurrentBookdisplayer()->getPdfMode())
    {
        //Show pdf buttons:
        ui->pdfDropBox->show();
        ui->PDFpageLBL->show();

        ui->locationMenu->menuAction()->setVisible(false);
    }
    else if(isWebkitPdf)
    {
        ui->locationMenu->menuAction()->setVisible(false);

        //Disable poppler pdf buttons:
        ui->pdfDropBox->hide();
        ui->PDFpageLBL->hide();
    }
    else
    {
        ui->locationMenu->menuAction()->setVisible(true);

        if ( CurrentBookdisplayer()->book() != NULL )
        {
            //Show / Hide nikud and teamim buttons
            ui->showNikudAction->setVisible( CurrentBookdisplayer()->book()->hasNikud );
            ui->showTeamimAction->setVisible( CurrentBookdisplayer()->book()->hasTeamim );
        }

        //Disable / Enable them by what's displayed
        ui->showNikudAction->setChecked( CurrentBookdisplayer()->isNikudShown() );
        ui->showTeamimAction->setChecked( CurrentBookdisplayer()->areTeamimShown() );

#ifdef POPPLER
        //Disable poppler pdf buttons:
        ui->pdfDropBox->hide();
        ui->PDFpageLBL->hide();
#endif
    }
}

void DesktopApp::menuErrorReport()
{
    QString link = CurrentBookdisplayer()->activeLink().replace("$","");
    int id = CurrentBookdisplayer()->book()->getUniqueId();
    int index = bookList.FindBookById(id);

    errorReport *err = new errorReport( this, link, bookList[index]->getNormallDisplayName() );
    err->show();
}

void DesktopApp::on_treeWidget_itemClicked(QTreeWidgetItem* current, int column)
{
    Book* book = bookList.findBookByTWI(current);

    if (current->checkState(0) == Qt::Checked) addBookToSearch(book);
    if (current->checkState(0) == Qt::Unchecked) removeBookFromSearch(book);

    if ( book->mIconState == BLUE )
    {
        ui->addToSearchAction->setEnabled(false);
        ui->removeFromSearchAction->setEnabled(true);
    }
    else if ( book->mIconState == GREY )
    {
        ui->addToSearchAction->setEnabled(true);
        ui->removeFromSearchAction->setEnabled(false);
    }
    else
    {
        ui->addToSearchAction->setEnabled(true);
        ui->removeFromSearchAction->setEnabled(true);
    }
}

//Called when a TreeItem is double clicked
void DesktopApp::on_treeWidget_itemDoubleClicked(QTreeWidgetItem* item, int column)
{
    Book* book = bookList.findBookByTWI(item);

    if ( book->tabWidget() != 0 )
    {
        ui->viewTab->setCurrentWidget( book->tabWidget() );
    }
    else
    {
        //Open the selected book
        openBook(book);
    }
}

QList <QCheckBox *> weavedList;

void DesktopApp::treeWidgetSelectionChanged(QTreeWidgetItem* current, QTreeWidgetItem* old)
{
    Book* book = bookList.findBookByTWI(current);

    //Show / Hide mixed display stuff
    if (book->IsMixed())
    {
        ui->mixedGroup->show();

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
            ui->verticalLayout_11->addWidget(chk);

            if (book->mWeavedSources[i].show == true)
            {
                chk->setChecked(true);
            }

            chk->setWhatsThis(stringify(book->mWeavedSources[i].id));

            weavedList << chk;

            QSignalMapper *signalMapper = new QSignalMapper(this);

            connect(chk, SIGNAL(clicked()), signalMapper, SLOT(map()));
            signalMapper->setMapping(chk, weavedList.size() - 1);
            connect(signalMapper, SIGNAL(mapped(int)), this, SLOT(weavedCheckBoxClicked(int)));
        }

        ui->showaloneCBX->setChecked(book->showAlone);
        ui->mixedFrame->setEnabled(!ui->showaloneCBX->isChecked());
    }
    else ui->mixedGroup->hide();
}

void DesktopApp::weavedCheckBoxClicked(int btnIndex)
{
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
}


void DesktopApp::settingsForm()
{
    //Open the settings form
    Settings *settingsform = new Settings();

    settingsform->setModal(true);

    connect (settingsform, SIGNAL(ChangeLang(QString)), this, SLOT(translate(QString)));

    settingsform->show();
}

void DesktopApp::importForm()
{
    //Open the import form
    importBook *ib = new importBook();

    ib->setModal(true);

    connect(ib, SIGNAL(updateTree()), this, SLOT(updateBookTree()));

    ib->show();
}


void DesktopApp::findBookForm()
{
    bookfind * bf = new bookfind(this, bookList);
    bf->show();

    connect(bf, SIGNAL(openBook(int)), this, SLOT(menuOpenBook(int)));
}

void DesktopApp::menuOpenBook(int uid)
{
    Book* book = bookList.findBookById(uid);

    if ( book )
    {
        if ( book->tabWidget() != 0 )
        {
            ui->viewTab->setCurrentWidget (book->tabWidget());
        }
        else
        {
            addViewTab(false);
            openBook(book);
        }
    }
}

void DesktopApp::on_viewTab_tabCloseRequested(int index)
{
    if (bookdisplayer(index)->book() != NULL)
        bookdisplayer(index)->book()->setTabWidget( 0 );

    bookdisplayer(index)->deleteLater();

    //Destroy tab
/*
    ui->viewTab->widget(index)->layout()->deleteLater();
    ui->viewTab->widget(index)->deleteLater();
*/
    ui->viewTab->removeTab(index);

    if (ui->viewTab->count() == 0)
    {
        addViewTab(false);
        bookdisplayer(0)->setHtml(simpleHtmlPage(tr("Orayta - Jewish books"), ""));
    }

    //Update the stupid tab widget
    on_viewTab_currentChanged(index - 1);
}

void DesktopApp::openExternalLink(QString lnk)
{
    QStringList parts = lnk.split(":");

    int id;
    if(ToNum(parts[0], &id))
    {
        Book* book = bookList.findBookById(id);
        if( book )
        {
            /*
            // if this book is already open, don't add a new tab
            QWidget* tabWidget = book->tabWidget();
            if ( tabWidget != 0 )
            {
                ui->viewTab->setCurrentWidget (tabWidget);

                CurrentBookdisplayer()->webview()->page()->mainFrame()->scrollToAnchor("#" + parts[1]);

                CurrentBookdisplayer()->execScript(QString("paintByHref(\"$" + parts[1] + "\");"));

                if (parts.size() == 3)
                    CurrentBookdisplayer()->highlight( QRegExp(unescapeFromBase32(parts[2])) );
            }
            else
            {
                //Add a new tab and open the link there
                addViewTab(false);
                ui->viewTab->setTabText(CURRENT_TAB, tr("Orayta"));

                CurrentBookdisplayer()->setInternalLocation("#" + parts[1]);

                if (parts.size() == 3)
                    CurrentBookdisplayer()->setSearchMarker( QRegExp(unescapeFromBase32(parts[2])) );

                openBook(book);
            }
            */
            //Add a new tab and open the link there
            addViewTab(false);
            ui->viewTab->setTabText(CURRENT_TAB, tr("Orayta"));

            CurrentBookdisplayer()->setInternalLocation("#" + parts[1]);

            if (parts.size() == 3)
                CurrentBookdisplayer()->setSearchMarker( QRegExp(unescapeFromBase32(parts[2])) );

            openBook(book);
        }
    }
}

void DesktopApp::on_openMixed_clicked()
{
    Book* book = bookList.findBookByTWI(ui->treeWidget->currentItem());
    if ( book )
    {
        //@@@@@@@@@

        //Reopen at current position, if exists

        //TODO: If active part is in view, use it instead

        //If it's the same book:
        if ( CurrentBookdisplayer()->book() == book )
        {
            QString script = "var obj = ClosestElementToView();";
            CurrentBookdisplayer()->execScript(script);
            QString closest = CurrentBookdisplayer()->getJSVar("obj.href");

            QString link = closest.mid(closest.indexOf("$")+1);
            if (link != "") CurrentBookdisplayer()->setInternalLocation("#" + link);
        }

        openBook(book);
    }
}

void DesktopApp::on_showaloneCBX_clicked(bool checked)
{
    ui->mixedFrame->setEnabled(!checked);

    Book* book = bookList.findBookByTWI(ui->treeWidget->currentItem());
    if ( book )
    {
        book->showAlone = checked;
        book->setTabWidget( 0 );
    }
}

void DesktopApp::on_showSearchBarButton_clicked(bool checked)
{
    ui->searchGroupBX->setVisible(checked);
    ui->lineEdit->setFocus(Qt::ActiveWindowFocusReason);
}

void DesktopApp::on_hideSearchButton_clicked()
{
    ui->showSearchBarButton->click();
}

void DesktopApp::showSearchTab()
{
    ui->treeTab->setCurrentIndex(1);
}

void DesktopApp::on_cancelSearchBTN_clicked()
{
    stopSearchFlag = true;
}

void DesktopApp::on_treeTab_currentChanged(int index)
{
    if (index == 1)
    {
        ui->searchInBooksLine->setFocus(Qt::ActiveWindowFocusReason);
    }
}

void DesktopApp::ToggleSearchBar()
{
    ui->showSearchBarButton->click();
}


void DesktopApp::translate(QString newlang)
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

    if (LANG == "Hebrew") setDirection(true);
    else setDirection(false);
}

/*
//Builds all book's search DB's, while the program is running
void buidSearchDBinBG(BookList * bl)
{
    for (int i=bl->size()-1; i>=0 && bl->size() > 0; i--)
    {
        (*bl)[i]->BuildSearchTextDB();
        qDebug() << "Building DB - "  << i;
    }
}
*/

#ifdef POPPLER

void DesktopApp::updatePdfPage(int current, int max)
{
    ui->pdfDropBox->setStatusTip(stringify(max));
    ui->pdfDropBox->clear();

    for (int i=1; i<=max; i++)
    {
        ui->pdfDropBox->addItem(stringify(i));
    }
    ui->pdfDropBox->setCurrentIndex(current - 1);

    //Set tooltip
    //ui->pdfDropBox->setToolTip(tr("Page: ") + QString::number(current) + " / " + QString::number(max));
}

void DesktopApp::on_pdfDropBox_currentIndexChanged(QString val)
{
    if (stringify(ui->pdfDropBox->count()) == ui->pdfDropBox->statusTip())
    {
        int page = 0;
        if (ToNum(val, &page))
        {
            if (CurrentBookdisplayer()->getPdfMode())
            {
                CurrentBookdisplayer()->setPdfPage(page);

                //Set tooltip
                //ui->pdfDropBox->setToolTip(tr("Page: ") + val + " / " + ui->pdfDropBox->itemText(ui->pdfDropBox->count() - 1));
            }
        }
    }
}


#endif


void DesktopApp::on_groupBox_toggled(bool checked)
{
    if (checked)
    {
        ui->guematriaCheckBox->setChecked(false);
    }
}

void DesktopApp::on_guematriaCheckBox_toggled(bool checked)
{
    if (checked)
    {
        ui->groupBox->setChecked (false);
    }
}

void DesktopApp::on_radioButton_2_toggled(bool checked)
{
    if (checked)
        ui->spinBox->setEnabled (true);
    else
        ui->spinBox->setEnabled (false);
}

void DesktopApp::on_radioButton_3_toggled(bool checked)
{
    if (checked)
        ui->fullCheckBox->setEnabled (false);
    else
        ui->fullCheckBox->setEnabled (true);
}

void DesktopApp::on_searchInBooksLine_returnPressed()
{
    on_SearchInBooksBTN_clicked();
}


void DesktopApp::searchGuematria()
{
    ui->treeTab->setCurrentIndex(1);
    ui->guematriaCheckBox->setChecked(true);
}

bookDisplayer* DesktopApp::CurrentBookdisplayer()
{
    return bookdisplayer( CURRENT_TAB );
}

bookDisplayer* DesktopApp::bookdisplayer( int index )
{
    QWidget* w = ui->viewTab->widget(index);
    return qobject_cast<bookDisplayer*>(w);
}

void DesktopApp::on_progressBar_valueChanged(int value)
{
    if (value == 0 || value == 100) ui->pbarbox->hide();
    else ui->pbarbox->show();
}



void DesktopApp::on_SearchInBooksBTN_clicked()
{
    QString otxt = ui->searchInBooksLine->text();
    QString stxt = otxt;
    QRegExp regexp;


    if (ui->guematriaCheckBox->isChecked())
    {
        SearchGuematria (otxt);
    }
    else
    {
        if (ui->fuzzyCheckBox->isChecked())
            stxt = AllowKtivHasser(stxt);

        bool allwords = ui->radioButton_2->isChecked();
        bool fullwords = ui->fullCheckBox->isChecked();
        int spacing = ui->spinBox->value();

        if (ui->radioButton_3->isChecked())
            regexp = QRegExp(stxt);
        else
            regexp = QRegExp( createSearchPattern (stxt, allwords, fullwords, spacing) );
        regexp.setMinimal(true);

        //Make a new tab if the current tab has something in it
        if (ui->viewTab->tabText(CURRENT_TAB).indexOf(tr("Orayta")) == -1)
        {
            addViewTab(false);
            CurrentBookdisplayer()->normalZoom();
            CurrentBookdisplayer()->setHtml(simpleHtmlPage(tr("Orayta"), ""));
            ui->viewTab->setTabText(CURRENT_TAB, (tr("Orayta")));
        }

        bookDisplayer *pCurrentBookdisplayer = CurrentBookdisplayer();
        pCurrentBookdisplayer->ShowWaitPage();

        //Set the title of the tab to show what it's searching for
        QString title = tr("Searching: "); title += "\"" + otxt + "\"" + " ...";
        ui->viewTab->setTabText(CURRENT_TAB, title);

        QUrl u = SearchInBooks (regexp, otxt, bookList.BooksInSearch(), ui->progressBar);
        pCurrentBookdisplayer->load(u);
    }
}

QString lastSearch = "";

void DesktopApp::on_searchForward_clicked()
{
    if (ui->lineEdit->text().replace(" ","") == "")
        return;

    if (CurrentBookdisplayer()->book())
    {
        if ( CurrentBookdisplayer()->book()->fileType() == Book::Normal )
        {
            QRegExp regexp = withNikudAndTeamim(ui->lineEdit->text());

            QString text = CurrentBookdisplayer()->pageText();

            int ppp = CurrentBookdisplayer()->searchPos();

            //Find the closest occurence of the RegExp
            QString next = "";

            int pos = regexp.indexIn(text, ppp + 1);
            if (pos > -1)
            {
                next = regexp.cap(0);
                CurrentBookdisplayer()->setSearchPos(pos);
                CurrentBookdisplayer()->searchText(next, false);
            }
        }
        else if ( CurrentBookdisplayer()->book()->fileType() == Book::Html )
        {
            CurrentBookdisplayer()->webview()->findText ( ui->lineEdit->text() );
        }
#ifdef POPPLER
        else if ( CurrentBookdisplayer()->book()->fileType() == Book::Pdf )
        {
            CurrentBookdisplayer()->pdfview()->searchForwards( ui->lineEdit->text() );
        }
#endif //POPPLER
    }
    else
    {
        CurrentBookdisplayer()->webview()->findText ( ui->lineEdit->text() );
    }
}

void DesktopApp::on_searchBackward_clicked()
{
    if (ui->lineEdit->text().replace(" ","") == "")
        return;

    if (CurrentBookdisplayer()->book())
    {
        if ( CurrentBookdisplayer()->book()->fileType() == Book::Normal )
        {
            QString text = CurrentBookdisplayer()->pageText();

            int ppp = CurrentBookdisplayer()->searchPos();

            QRegExp regexp = withNikudAndTeamim(ui->lineEdit->text());

            //Find the closest occurence of the RegExp, backwards
            QString last = "";

            int pos = regexp.lastIndexIn(text, -(text.length()-ppp+1));
            if (pos > -1)
            {
                last = regexp.cap(0);
                CurrentBookdisplayer()->setSearchPos(pos);
                CurrentBookdisplayer()->searchText(last, true);
            }
        }
        else if ( CurrentBookdisplayer()->book()->fileType() == Book::Html )
        {
            CurrentBookdisplayer()->webview()->findText ( ui->lineEdit->text(), QWebPage::FindBackward );
        }
#ifdef POPPLER
        else if ( CurrentBookdisplayer()->book()->fileType() == Book::Pdf )
        {
            CurrentBookdisplayer()->pdfview()->searchBackwards( ui->lineEdit->text() );
        }
#endif
    }
    else
    {
        CurrentBookdisplayer()->webview()->findText ( ui->lineEdit->text(), QWebPage::FindBackward );
    }
}

void DesktopApp::SearchGuematria (QString txt)
{
    QString title = "", Htmlhead = "", Html = "";

    int guematria = GematriaValueOfString ( txt );

    if ( guematria != 0 )
    {

        Book* tanachRoot = bookList.FindBookByName(" מקרא");
        if (tanachRoot == NULL)
        {
            qDebug("tanach root not found !");
            return;
        }

        vector<Book*> tanach = bookList.Children( tanachRoot );

        //Make a new tab if the current tab has something in it
        if (ui->viewTab->tabText(CURRENT_TAB).indexOf(tr("Orayta")) == -1)
        {
            addViewTab(false);
            CurrentBookdisplayer()->normalZoom();
            CurrentBookdisplayer()->setHtml(simpleHtmlPage(tr("Orayta"), ""));
        }

        ui->pbarbox->show();
        ui->progressBar->setValue(0);
        float percentPerBook = (100.0 / tanach.size());

        bookDisplayer *pCurrentBookdisplayer = CurrentBookdisplayer();
        pCurrentBookdisplayer->ShowWaitPage();

        //Set the title of the tab to show what it's searching for
        title = tr("Searching guematria for: ");
        title += "\"" + txt + "\"" + " ...";

        ui->viewTab->setTabText(CURRENT_TAB, title);

        int results = 0, nbBooksInSearch = 0;
        for (unsigned i=0; i < tanach.size() && stopSearchFlag == false; i++)
        {
            if (tanach[i]->IsInSearch() == false)
                continue;

            nbBooksInSearch++;
            vector <GuematriaDb> bookGuematriaDb = tanach[i]->getGuematriaDb();

            ui->progressBar->setValue( (i + 1) * percentPerBook ) ;

            for (unsigned j=0; j < bookGuematriaDb.size(); j++)
            {
                //Let the animation move...
                QApplication::processEvents();

                // search the guematria
                int calc = bookGuematriaDb[j].values[0];
                unsigned first = 0, last = 0;
                while ( last < bookGuematriaDb[j].values.size() )
                {
                    if (calc == guematria)
                    {
                        results ++;

                        QString occ = "";
                        for (unsigned cmpt = first; cmpt <= last; cmpt++)
                        {
                            occ += bookGuematriaDb[j].words[cmpt];
                            if (cmpt < last) occ += " ";
                        }

                        //Get the text best to show for this reult's description
                        QString linkdisplay /* = tanach[i]->getNormallDisplayName() + " "*/;  // title not needed in tanach...
                        linkdisplay += bookGuematriaDb[j].itr.humanDisplay();
                        //Add the full result to the page
                        Html += "<span style=\"font-size:20px\">";
                        Html += "<a href=!" + stringify(tanach[i]->getUniqueId()) + ":";
                        Html += bookGuematriaDb[j].itr.toEncodedString();
                        Html += ":" + escapeToBase32(occ) + ">";
                        Html += stringify(results) + ") " + linkdisplay;
                        Html += "</a><br></span>\n";
                        Html += bookGuematriaDb[j].words.join(" ").replace( QRegExp("(" + occ + ")"), "<span style='background-color:Yellow'>\\1</span>" );
                        Html += "<br><br>\n";

                        last = ++first;
                        if (last < bookGuematriaDb[j].values.size())
                            calc = bookGuematriaDb[j].values[first];
                    }

                    while (calc < guematria)
                    {
                        last++;
                        if (last < bookGuematriaDb[j].values.size())
                            calc += bookGuematriaDb[j].values[last];
                        else
                            break;
                    }
                    while (calc > guematria)
                    {
                        calc -= bookGuematriaDb[j].values[first];
                        first++;
                    }
                }
            }
        }

        //Head and title of the Html
        title = tr("Search results for guematria : ") + "\"" + txt + "\"";
        title += " (" + stringify(guematria) + ")";


        Htmlhead = html_head(title, QFont(gFontFamily, gFontSize));
        Htmlhead += "<body>";
        Htmlhead += "<div style=\"font-size:30px\"><b><i><center>";
        Htmlhead += title + ":" + "</center></i></b></div><BR>";
        Htmlhead += "\n<div class=\"Contect\">";

        if (stopSearchFlag == true)
        {
            Htmlhead += "<BR><BR>" + tr("(Search stopped by user)") + "<BR><BR>";
            stopSearchFlag = false;
        }

        if (nbBooksInSearch == 0)
        {
            Htmlhead +="<br><br><center><b>";
            Htmlhead += tr("No tanach books selected : please select books in tanach and search again.");
            Htmlhead += "</b></center><br>";
        }
        else if(results == 0)
        {
            Htmlhead +="<br><b>";
            Htmlhead += tr("No guematria results found:");
            Htmlhead += "\"" + txt + "\"";
            Htmlhead += "</b><br>";
        }
        else
        {
            Htmlhead += "<b>";
            Htmlhead += tr("Result list: ");
            Htmlhead += "</b> "+ QString::number(results) + tr(" results founds.") + "<br><br>";
        }

        Html = Htmlhead + Html;

        Html += "</div>\n";
        Html += "\n</body>\n</html>";

        writetofile(TMPPATH + "Guematria.html", Html, "UTF-8");

        pCurrentBookdisplayer->load(QUrl(TMPPATH + "Guematria.html"));

        ui->pbarbox->hide();
    }
}


BookList * DesktopApp::getBookList() { return &bookList;}

void DesktopApp::on_leftChap_clicked()
{
    //@@@
    //Shouldn't this be done in bookdisplayer?

    BookIter itr = CurrentBookdisplayer()->currentLocation;
    if (LANG == "Hebrew") itr = CurrentBookdisplayer()->book()->nextChap(itr);
    else itr = CurrentBookdisplayer()->book()->prevChap(itr);

    QUrl u = CurrentBookdisplayer()->book()->renderChapterHtml(&itr, &bookList, true, true);
    CurrentBookdisplayer()->load(u);
    CurrentBookdisplayer()->currentLocation = itr;
}

void DesktopApp::on_rightChap_clicked()
{
    //@@@
    //Shouldn't this be done in bookdisplayer?

    BookIter itr = CurrentBookdisplayer()->currentLocation;

    if (LANG == "Hebrew") itr = CurrentBookdisplayer()->book()->prevChap(itr);
    else itr = CurrentBookdisplayer()->book()->nextChap(itr);

    QUrl u = CurrentBookdisplayer()->book()->renderChapterHtml(&itr, &bookList, true, true);
    CurrentBookdisplayer()->load(u);
    CurrentBookdisplayer()->currentLocation = itr;
}
