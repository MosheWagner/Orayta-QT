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

//TODO: Search in html files too

//TODO: Added books should have a "remove" option in the context menu.
//TODO: All books should have a "book settings" option via the menu too.
//TODO: Add the book settings whatever (dialog ?)

//TODO: Search with no result dosn't need a new tab (Msgbox?)

//TODO: Menu entries don't go RTL. is it QT's bug?

//TODO: Test new runtime dependencies

//TODO: Improve search result preview

//TODO: Search result marks in books with nikud don't work

//TODO: Finish GUI changes

//TODO: JS function "ClosestElementToView" isn't accurate. (Maybe jump there but without the red mark?)

/*
  Roadmap for 0.05:

  - Allow printing. (Html + Pdf) If it's really hard, html-s can become pdf-s and then printed

  - Improve all search stuff (including better result page. using side bar?)
  
  - Solve all book issues (wierd nikud, a few books that render incorrectly, better book names in tree)

  - Improve GUI. (Remove search bar? Change in and out of search icons)
*/

//TODO: Escape books with wierd chars in their name. ( ' , " , \ , / ). (For import)

//TODO: Add date to bookmarks (somewhere), and "sort by" option?

//TODO: Improve current position detection

//TODO: Allow adding custom html books through the program

//TODO: dictionary

//TODO: Translatable welcome page
//(TODO: create help page for linux version)
//TODO: hide welcomepage from search

//TODO: document & clean code

//TODO: Requested Feature: parallel tab (like in the originall)

//TODO: see what else to do ;-)

//TODO: Add rav kook books

/*
    DB issues:

    - Full map for puretext.
    - Allow finding ktiv too (only kri is found now)
    - Make auto DB building an option.
    - Save DB to file?

    - BUG: DB in file mode doesn't save the levelMap, so it crashes
    - BUG: do not allow english letters in puretext
*/


// Global
QString gFontFamily = "Nachlieli CLM";
int gFontSize = 16;


// This is just a very simple define. anywhere in the code,
//  "CURRENT_TAB" simply represents "ui->viewTab->currentIndex()".
//
//  It just made the code so much more readable, I couldn't resist
#define CURRENT_TAB ui->viewTab->currentIndex()

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindowClass)
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
    cornerWidgetLayout->addWidget(ui->pdfPageSpin);
    cornerWidgetLayout->addWidget(ui->PDFpageLBL);
    cornerWidgetLayout->addSpacing(45);
#endif

    ui->btnBox->addButton((QAbstractButton *)ui->refreshButton, QDialogButtonBox::ActionRole);
    ui->btnBox->addButton((QAbstractButton *)ui->newTabButton, QDialogButtonBox::ActionRole);
    ui->btnBox->addButton((QAbstractButton *)ui->topButton, QDialogButtonBox::ActionRole);
    ui->btnBox->addButton((QAbstractButton *)ui->zoominButton, QDialogButtonBox::ActionRole);
    ui->btnBox->addButton((QAbstractButton *)ui->zoomoutButton, QDialogButtonBox::ActionRole);
    ui->btnBox->addButton((QAbstractButton *)ui->showSearchBarButton, QDialogButtonBox::ActionRole);

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

    //Load the bookmarks into thier list:
    buildBookMarkList();

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
void MainWindow::setDirection(bool rtl)
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

void MainWindow::connectMenuActions()
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

MainWindow::~MainWindow()
{
    //TODO: Don't I need to free all the items?
    bookList.clear();

    ClearTmp();

    delete ui;
}

//Remove all temporary html files the program created
void MainWindow::ClearTmp()
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
void MainWindow::closeEvent(QCloseEvent *event)
{
    //Cancel any running searches
    stopSearchFlag = true;

    QSettings settings("Orayta", "SingleUser");

    settings.beginGroup("MainWindow");
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
void MainWindow::restoreConfs()
{
    QSettings settings("Orayta", "SingleUser");

    settings.beginGroup("MainWindow");
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
    gFontFamily = settings.value("fontfamily", "Nachlieli CLM").toString();
    gFontSize = settings.value("fontsize",16).toInt();

    settings.endGroup();
}

void MainWindow::restoreBookConfs()
{
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
}

void MainWindow::BuildBookTree()
{
    //Add treeItems for each book to the treeWidget
    for(unsigned int i=0;i<bookList.size();i++)
    {
        if (bookList[i]->IsHidden() != true)
        {
            QTreeWidgetItem *twi;
            if(bookList[i]->getParent() == NULL)
                twi = new QTreeWidgetItem(ui->treeWidget);
            else
                twi = new QTreeWidgetItem(bookList[i]->getParent()->getTreeItemPtr());

            bookList[i]->setTreeItemPtr(twi);

            QString dn;
            if(bookList[i]->getTreeDisplayName() != "")
                dn = bookList[i]->getTreeDisplayName();
            else if (bookList[i]->getNormallDisplayName() != "")
                dn = bookList[i]->getNormallDisplayName();
            else
            {
                vector<QString> name_parts;
                splittotwo(bookList[i]->getName(), name_parts, "_");
                dn = name_parts[1];
            }
            dn.replace("שס", "ש\"ס");
            twi->setText(0, dn);

            //set the icon:
            QIcon *icon = bookIcon(bookList[i], bookList[i]->mIconState);
            twi->setIcon(0, *icon);
            delete icon;

            if ( bookList[i]->fileType() == Book::Dir || bookList[i]->fileType() == Book::Normal || bookList[i]->fileType() == Book::Html )
            {
                twi->setFlags( Qt::ItemIsSelectable | Qt::ItemIsEnabled | Qt::ItemIsUserCheckable );
                twi->setCheckState(0,Qt::Unchecked);
            }
            else
            {
                twi->setFlags( Qt::ItemIsSelectable | Qt::ItemIsEnabled );
            }
        }
    }

    //Only after the tree is done, connect the itemChanged signal
    connect (ui->treeWidget, SIGNAL(currentItemChanged(QTreeWidgetItem*,QTreeWidgetItem*)), this, SLOT(treeWidgetSelectionChanged(QTreeWidgetItem*,QTreeWidgetItem*)));
}

void MainWindow::updateBookTree()
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
void MainWindow::LoadHtmlBook(Book *book, QString markString)
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
        renderedOK = book->htmlrender(htmlfilename, shownikud, showteamim, markString);
    }
    if (renderedOK == true)
    {
        QString p =  absPath(htmlfilename);

        CurrentBookdisplayer()->load(QUrl(p));
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
void MainWindow::openBook( Book* book )
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
            LoadHtmlBook(book);
            break;

        case ( Book::Html ):
            CurrentBookdisplayer()->load( QUrl(book->getPath()) );
            break;

        case ( Book::Pdf ):
#ifdef POPPLER
            CurrentBookdisplayer()->setPdfMode(true);
            CurrentBookdisplayer()->load ( QUrl(book->getPath()) );
            ui->viewTab->setTabText(CURRENT_TAB, book->getNormallDisplayName());
#else

            CurrentBookdisplayer()->enablePlugins();
            CurrentBookdisplayer()->setHtml(pluginPage(book->getNormallDisplayName()));
            if ( CurrentBookdisplayer()->execScript("testPdfPlugin()").toString() == "yes" )
            {
                CurrentBookdisplayer()->load( QUrl( "file:///" + book->getPath() ) );
                ui->viewTab->setTabText(CURRENT_TAB, book->getNormallDisplayName());
            }
#endif
            break;

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
void MainWindow::openSelectedBook( )
{
    Book* book = bookList.findBookByTWI(ui->treeWidget->selectedItems()[0]);
    openBook( book );
}

//Opens a new tab and Calls "openBook" (using the currently selected book in the tree).
void MainWindow::openSelectedBookInNewTab()
{
    addViewTab(false);
    openSelectedBook();
}

void MainWindow::changeFont()
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

void MainWindow::deleteSelectedBook()
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

void MainWindow::on_newTabButton_clicked()
{
    //Create the new tab and switch to it
    addViewTab(true);
}

void MainWindow::addViewTab(bool empty)
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

void MainWindow::on_lineEdit_returnPressed()
{
    on_searchForward_clicked();
}

void MainWindow::addBookToSearch(Book* book)
{
    if ( book )
        book->select();

    ui->addToSearchAction->setEnabled(false);
    ui->removeFromSearchAction->setEnabled(true);
}

void MainWindow::addToSearch()
{
    Book* book = bookList.findBookByTWI(ui->treeWidget->currentItem());
    addBookToSearch(book);
}

void MainWindow::on_addAllToSearchButton_clicked()
{
    for(unsigned int i=0; i<bookList.size(); i++)
    {
        if ( bookList[i]->IsUserCheckable() )
            bookList[i]->select();
    }

    ui->addToSearchAction->setEnabled(false);
    ui->removeFromSearchAction->setEnabled(true);
}

void MainWindow::removeBookFromSearch(Book* book)
{
    if ( book )
        book->unselect();

    ui->addToSearchAction->setEnabled(true);
    ui->removeFromSearchAction->setEnabled(false);
}

void MainWindow::removeFromSearch()
{
    Book* book = bookList.findBookByTWI(ui->treeWidget->currentItem());
    removeBookFromSearch(book);
}

void MainWindow::on_removeAllFromSearchButton_clicked()
{
    for(unsigned int i=0; i<bookList.size(); i++)
        bookList[i]->unselect();

    ui->addToSearchAction->setEnabled(true);
    ui->removeFromSearchAction->setEnabled(false);
}

void MainWindow::on_zoominButton_clicked()
{
    CurrentBookdisplayer()->ZoomIn();
}

void MainWindow::on_zoomoutButton_clicked()
{
    CurrentBookdisplayer()->ZoomOut();
}

void MainWindow::on_topButton_clicked()
{
    CurrentBookdisplayer()->jumpToTop();
}

void MainWindow::on_refreshButton_clicked()
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

        LoadHtmlBook(book);
    }
}


void MainWindow::on_treeWidget_customContextMenuRequested(QPoint pos)
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
void MainWindow::collapseItem()
{
    ui->treeWidget->collapseItem(ui->treeWidget->selectedItems()[0]);
}

//Expand the currently selected item in the treewidget
void MainWindow::expandItem()
{
    ui->treeWidget->expandItem(ui->treeWidget->selectedItems()[0]);
}

//Deals with keyboard events
void MainWindow::keyPressEvent( QKeyEvent *keyEvent )
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
        else if (ui->bookmarkWidget->hasFocus())
        {
             //Act as if the current item was clicked
            on_bookmarkWidget_itemDoubleClicked(ui->bookmarkWidget->currentItem());
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
void MainWindow::printBook()
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
void MainWindow::openCommentDialog(QString link)
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
void MainWindow::addCommentAtPosition(QString link, QString comment)
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
    QtConcurrent::run( book, &Book::htmlrender, htmlfilename, shownikud, showteamim, QString("") );

}

void MainWindow::removeComment(QString link)
{
    //Adding an emtpy comment removes the existing one
    addCommentAtPosition(link, "");
}

void MainWindow::menuComment()
{
    QString link = CurrentBookdisplayer()->activeLink().replace("$","");

    //Find book's id and add it to the link
    int id = CurrentBookdisplayer()->book()->getUniqueId();
    link = stringify(id) + ":" + link;

    openCommentDialog( link );
}

void MainWindow::about()
{
    About *aboutfrm = new About(this);

    aboutfrm->show();
}

void MainWindow::closeCurrentTab()
{
    on_viewTab_tabCloseRequested(CURRENT_TAB);
}

void MainWindow::toggleNikud(bool showNikud)
{
    CurrentBookdisplayer()->showNikud(showNikud);
    LoadHtmlBook( CurrentBookdisplayer()->book() );
}

void MainWindow::toggleTeamim(bool showTeamim)
{
    CurrentBookdisplayer()->showTeamim(showTeamim);
    LoadHtmlBook( CurrentBookdisplayer()->book() );
}

void MainWindow::on_viewTab_currentChanged(int index)
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
void MainWindow::adjustMenus()
{
    bool isWebkitPdf = false;
    if (CurrentBookdisplayer()->book() != NULL)
    {
        if (CurrentBookdisplayer()->book()->getName().endsWith(".pdf", Qt::CaseInsensitive)) isWebkitPdf = true;
    }


    if (CurrentBookdisplayer()->getPdfMode())
    {
        //Show pdf buttons:
        ui->pdfPageSpin->show();
        ui->PDFpageLBL->show();

        ui->locationMenu->menuAction()->setVisible(false);
    }
    else if(isWebkitPdf)
    {
        ui->locationMenu->menuAction()->setVisible(false);

        //Disable poppler pdf buttons:
        ui->pdfPageSpin->hide();
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
        ui->pdfPageSpin->hide();
        ui->PDFpageLBL->hide();
#endif
    }
}

void MainWindow::menuErrorReport()
{
    QString link = CurrentBookdisplayer()->activeLink().replace("$","");

    if (link != "")
    {
        int id = CurrentBookdisplayer()->book()->getUniqueId();
        int index = bookList.FindBookById(id);

        errorReport *err = new errorReport( this, link, bookList[index]->getNormallDisplayName() );
        err->show();
    }
}

void MainWindow::on_treeWidget_itemClicked(QTreeWidgetItem* current, int column)
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
void MainWindow::on_treeWidget_itemDoubleClicked(QTreeWidgetItem* item, int column)
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

void MainWindow::treeWidgetSelectionChanged(QTreeWidgetItem* current, QTreeWidgetItem* old)
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

void MainWindow::weavedCheckBoxClicked(int btnIndex)
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


void MainWindow::settingsForm()
{
    //Open the settings form
    Settings *settingsform = new Settings();

    settingsform->setModal(true);

    connect (settingsform, SIGNAL(ChangeLang(QString)), this, SLOT(translate(QString)));

    settingsform->show();
}

void MainWindow::importForm()
{
    //Open the import form
    importBook *ib = new importBook();

    ib->setModal(true);

    connect(ib, SIGNAL(updateTree()), this, SLOT(updateBookTree()));

    ib->show();
}


void MainWindow::findBookForm()
{
    bookfind * bf = new bookfind(this, bookList);
    bf->show();

    connect(bf, SIGNAL(openBook(int)), this, SLOT(menuOpenBook(int)));
}

void MainWindow::menuOpenBook(int uid)
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

void MainWindow::on_viewTab_tabCloseRequested(int index)
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

void MainWindow::openExternalLink(QString lnk)
{
    QStringList parts = lnk.split(":");

    int id;
    if(ToNum(parts[0], &id))
    {
        Book* book = bookList.findBookById(id);
        if( book )
        {
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
        }
    }
}

void MainWindow::on_openMixed_clicked()
{
    Book* book = bookList.findBookByTWI(ui->treeWidget->currentItem());
    if ( book )
    {
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

void MainWindow::on_showaloneCBX_clicked(bool checked)
{
    ui->mixedFrame->setEnabled(!checked);

    Book* book = bookList.findBookByTWI(ui->treeWidget->currentItem());
    if ( book )
    {
        book->showAlone = checked;
        book->setTabWidget( 0 );
    }
}

void MainWindow::on_showSearchBarButton_clicked(bool checked)
{
    ui->searchGroupBX->setVisible(checked);
    ui->lineEdit->setFocus(Qt::ActiveWindowFocusReason);
}

void MainWindow::on_hideSearchButton_clicked()
{
    ui->showSearchBarButton->click();
}

void MainWindow::showSearchTab()
{
    ui->treeTab->setCurrentIndex(1);
}

void MainWindow::on_cancelSearchBTN_clicked()
{
    stopSearchFlag = true;
}

void MainWindow::on_treeTab_currentChanged(int index)
{
    if (index == 1)
    {
        ui->searchInBooksLine->setFocus(Qt::ActiveWindowFocusReason);
    }
}

void MainWindow::ToggleSearchBar()
{
    ui->showSearchBarButton->click();
}


void MainWindow::translate(QString newlang)
{
    LANG = newlang;

    extern QTranslator *translator;

    //Remove old translator
    QApplication::removeTranslator(translator);

    //English needs no translator, it's the default
    if (LANG != "English")
    {
        if (!translator->load(LANG + ".qm", ".")) translator->load(LANG + ".qm", "/usr/share/Orayta/");
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
    for (int i=0; i<bl->size(); i++)
    {
        (*bl)[i]->BuildSearchTextDB();
    }
}
*/

#ifdef POPPLER

void MainWindow::updatePdfPage(int current, int max)
{
    ui->pdfPageSpin->setMaximum(max);
    ui->pdfPageSpin->setValue(current);

    //Set tooltip
    ui->pdfPageSpin->setToolTip(tr("Page: ") + QString::number(current) + " / " + QString::number(max));
}

void MainWindow::on_pdfPageSpin_valueChanged(int page)
{
    if (CurrentBookdisplayer()->getPdfMode())
    {
        int page = ui->pdfPageSpin->value();
        CurrentBookdisplayer()->setPdfPage(page);

        //Set tooltip
        ui->pdfPageSpin->setToolTip(tr("Page: ") + QString::number(page) + " / " + QString::number(ui->pdfPageSpin->maximum()));
    }
}
#endif

void MainWindow::searchGuematria()
{
    ui->treeTab->setCurrentIndex(1);
    ui->guematriaCheckBox->setChecked(true);
}

bookDisplayer* MainWindow::CurrentBookdisplayer()
{
    return bookdisplayer( CURRENT_TAB );
}

bookDisplayer* MainWindow::bookdisplayer( int index )
{
    QWidget* w = ui->viewTab->widget(index);
    return qobject_cast<bookDisplayer*>(w);
}
