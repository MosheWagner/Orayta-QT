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
* First Author: Moshe Wagner. <moshe.wagner@gmail.com>
* Maintenance, improvements and cleanup: Yoch Melka. <yoch.melka@gmail.com>
*/

#include "bookdisplayer.h"
#include "bookviewinterface.h"
#include "bookview_orayta.h"
#include "mainwindow.h"
#include "settings.h"


/*
  Roadmap for 0.05:

  - Allow printing. (Html + Pdf) If it's really hard, html-s can become pdf-s and then printed

  - Improve all search stuff (including better result page. using side bar?)

  - Improve GUI. (Remove search bar? Change in and out of search icons)

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

//TODO: Add date to bookmarks (somewhere), and "sort by" option?

//TODO: Improve current position detection

//TODO: dictionary

//TODO: Translatable welcome page

//TODO: create help page for linux version

//TODO: Requested Feature: parallel tab (like in the originall)

//TODO: see what else to do ;-)




// This is just a very simple define. anywhere in the code,
//  It just made the code so much more readable
#define CURRENT_TAB ui->viewTab->currentIndex()


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindowClass),
    mixedCheckBox(0),
    ptrToAdditionalWidget(0)
{
    //Code executed when the window is constructed:

    //Setup the GUI
    ui->setupUi(this);

    //QObject::connect (ui->treeWidget, SIGNAL(currentItemChanged(QTreeWidgetItem*,QTreeWidgetItem*)), this, SLOT(treeWidgetSelectionChanged(QTreeWidgetItem*,QTreeWidgetItem*)));
    QObject::connect(ui->treeWidget, SIGNAL(openBookRequested(NodeBook*,bool)), this, SLOT(openBook(NodeBook*,bool)));

    //Set window title:
    setWindowTitle(tr("Orayta"));

    //Remove all temporary html files the program created
    ClearTmp();

    //Loading confs
    restoreConfs();

    ui->treeDockWidget->setFloating(false);

    //Adjust for hebrew if needed
    if (LANG == "Hebrew") setDirection(true);

    //Force the viewTab to take 80 percent of the window
    ui->verticalLayout_12->setStretchFactor(ui->viewTab, 80);
    ui->verticalLayout_12->setStretchFactor(ui->verticalLayout_9, 20);

    //Close the default tab:
    ui->viewTab->removeTab(0);

    //Corner Widget : contain standard buttons & pdf buttons
    QWidget* cornerWidget = new QWidget;

    cornerWidgetLayout = new QHBoxLayout;

    cornerWidgetLayout->setDirection(QBoxLayout::RightToLeft);
    cornerWidgetLayout->setContentsMargins(2,0,2,0);

    // add basic buttons
    cornerWidgetLayout->addWidget(ui->newTabButton);
    cornerWidgetLayout->addWidget(ui->topButton);
    cornerWidgetLayout->addWidget(ui->zoominButton);
    cornerWidgetLayout->addWidget(ui->zoomoutButton);
    cornerWidgetLayout->addWidget(ui->showSearchBarButton);

    // add horizontal layout for additionnals buttons
    cornerWidgetLayout->addSpacing(45);

    cornerWidget->setLayout(cornerWidgetLayout);
    ui->viewTab->setCornerWidget(cornerWidget, Qt::TopRightCorner);


    ui->searchGroupBX->hide();
    ui->showSearchBarButton->setChecked(false);

    ui->pbarbox->hide();

    ui->mixedGroup->hide();

    ui->treeWidget->setHeaderHidden(true);

    //Hack for windows:
    ui->treeWidget->setColumnWidth(0,350);

    connectMenuActions();

    stopSearchFlag = false;

    //Now add a good one:
    addViewTab();

    //Load mixed display confs
    ui->treeWidget->restoreBooksConfs();

    //Load the bookmarks into thier list:
    buildBookMarkList();

    //Load start page. Assuming it's book[0] of course.
    // ############## crado
    if (!ui->treeWidget->booklist().empty())
    {
        openBook(ui->treeWidget->booklist()[0]);
    }
    else
    {
        // ##########
    }
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

    //ui->treeDockWidget->setAllowedAreas(dwa);
    removeDockWidget(ui->treeDockWidget);
    addDockWidget(dwa, ui->treeDockWidget);
    ui->treeDockWidget->show();


    ui->treeTab->setLayoutDirection(dir);
    ui->booksTab->setLayoutDirection(dir);
    ui->searchTab->setLayoutDirection(dir);
    ui->bookmarkTab->setLayoutDirection(dir);
    ui->viewTab->setLayoutDirection(dir);
    ui->mixedGroup->setLayoutDirection(dir);
    //ui->mixedFrame->setLayoutDirection(dir);

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
    // set shorcuts
    ui->zoominAction->setShortcut(QKeySequence::ZoomIn);
    ui->zoomoutAction->setShortcut(QKeySequence::ZoomOut);
    //ui->printAction->setShortcut(QKeySequence::Print);
    ui->opentabAction->setShortcut(QKeySequence::AddTab);
    ui->closetabAction->setShortcut(QKeySequence::Close);
    ui->searchInBookAction->setShortcut(QKeySequence::Find);
    ui->searchForwardAction->setShortcut(QKeySequence::FindNext);
    ui->searchBackwardAction->setShortcut(QKeySequence::FindPrevious);
    ui->actionSettings->setShortcut(QKeySequence::Preferences);

    connect(ui->aboutAction, SIGNAL(triggered()), this, SLOT(about()));
    connect(ui->zoominAction, SIGNAL(triggered()), this, SLOT(on_zoominButton_clicked()));
    connect(ui->zoomoutAction, SIGNAL(triggered()), this, SLOT(on_zoomoutButton_clicked()));
    connect(ui->jumptotopAction, SIGNAL(triggered()), this, SLOT(on_topButton_clicked()));
    connect(ui->printAction, SIGNAL(triggered()), this, SLOT(printBook()));

    connect(ui->findBookAction, SIGNAL(triggered()), this, SLOT(findBookForm()));
    connect(ui->opentabAction, SIGNAL(triggered()), this, SLOT(on_newTabButton_clicked()));
    connect(ui->closetabAction, SIGNAL(triggered()), this, SLOT(closeCurrentTab()));

    connect(ui->showNikudAction, SIGNAL(triggered(bool)), this, SLOT(toggleNikud(bool)));
    connect(ui->showTeamimAction, SIGNAL(triggered(bool)), this, SLOT(toggleTeamim(bool)));

    connect(ui->bookMarkAction, SIGNAL(triggered()), this, SLOT(menuBookMark()));
    connect(ui->commentAction, SIGNAL(triggered()), this, SLOT(menuComment()));
    connect(ui->reportErrorAction, SIGNAL(triggered()), this, SLOT(menuErrorReport()));

    connect(ui->advancedSearchAction, SIGNAL(triggered()), this, SLOT(showSearchTab()));
    connect(ui->searchInBookAction, SIGNAL(triggered()), this, SLOT(toggleSearchBar()));
    connect(ui->searchForwardAction, SIGNAL(triggered()), this, SLOT(on_searchForward_clicked()));
    connect(ui->searchBackwardAction, SIGNAL(triggered()), this, SLOT(on_searchBackward_clicked()));

    connect(ui->addAllToSearchAction, SIGNAL(triggered()), this, SLOT(on_addAllToSearchButton_clicked()));
    connect(ui->removeAllFromSearchAction, SIGNAL(triggered()), this, SLOT(on_removeAllFromSearchButton_clicked()));

    connect(ui->actionSettings, SIGNAL(triggered()), this, SLOT(settingsForm()));
    connect(ui->actionImport_books, SIGNAL(triggered()), this, SLOT(importForm()));

    connect(ui->actionSearchGuematria, SIGNAL(triggered()), this, SLOT(searchGuematria()));
}

MainWindow::~MainWindow()
{
    //TODO: Don't I need to free all the items?
    ClearTmp();

    delete ui;
}

//Remove all temporary html files the program created
void MainWindow::ClearTmp()
{
    //remove all html rended files in temp path
    QDir dir = QDir(TMPPATH);
    QStringList list = dir.entryList(QStringList("*.html"));
    for (int i=0; i < list.size(); i++)
    {
        QFile::remove(dir.absoluteFilePath(list[i]));
    }
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

    ui->treeWidget->saveBooksConfs();
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
    gFont = settings.value("font", gFont).value<QFont>();
    settings.endGroup();

    settings.beginGroup("Search_confs");
    MAX_RESULTS = settings.value("maxResults", MAX_RESULTS).toInt();
    MAX_RESULTS_PER_BOOK = settings.value("maxPerBook", MAX_RESULTS_PER_BOOK).toInt();
    settings.endGroup();
}

//Open the given book (by it's id in the booklist)
void MainWindow::openBook( NodeBook* book, bool newTab )
{
    if ( !book ) return;  // sanity check

    // ###########
    if ( book->tabWidget() != 0 && book->booktype() != NodeBook::Html )
    {
        ui->viewTab->setCurrentWidget(book->tabWidget());
    }
    else
    {
        BookDisplayer* bookdisp = newTab ? addViewTab() : CurrentBookdisplayer();

        bookdisp->loadBook(book);
    }

    adjustMenus();
}

void MainWindow::menuOpenBook(NodeBook* book)
{
    openBook(book, true);
}

void MainWindow::on_newTabButton_clicked()
{
    //Create the new tab and switch to it
    addViewTab();
}

BookDisplayer* MainWindow::addViewTab(bool set_current)
{
    BookDisplayer* bookdisp = new BookDisplayer(this, ui->viewTab);
    //Create new tab:
    int ind = ui->viewTab->addTab(bookdisp, tr("Orayta"));

    //Switch to the new tab
    if (set_current)
        ui->viewTab->setCurrentIndex(ind);

    return bookdisp;
}

void MainWindow::addNodeToSearch(BaseNodeItem* node)
{
    if ( node ) node->setSelected(true);
}

void MainWindow::addToSearch()
{
    addNodeToSearch(dynamic_cast<BaseNodeItem*>(ui->treeWidget->currentItem()));
}

void MainWindow::on_addAllToSearchButton_clicked()
{
    ui->treeWidget->root()->setSelected(true);
}

void MainWindow::removeNodeFromSearch(BaseNodeItem* node)
{
    if ( node ) node->setSelected(false);
}

void MainWindow::removeFromSearch()
{
    removeNodeFromSearch(dynamic_cast<BaseNodeItem*>(ui->treeWidget->currentItem()));
}

void MainWindow::on_removeAllFromSearchButton_clicked()
{
    ui->treeWidget->root()->setSelected(false);
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

/*    ################  A revoir  */
//Deals with keyboard events
void MainWindow::keyPressEvent( QKeyEvent *keyEvent )
{
    //Enter pressed
    if ( keyEvent->key() == Qt::Key_Return || keyEvent->key() == Qt::Key_Enter )
    {
        if (ui->bookmarkWidget->hasFocus())
        {
             //Act as if the current item was clicked
            on_bookmarkWidget_itemDoubleClicked(ui->bookmarkWidget->currentItem());
        }
    }
    QWidget::keyPressEvent(keyEvent);
}

/* */
//Print the current book (all pages I think...)
void MainWindow::printBook()
{
    CurrentBookdisplayer()->print();
}

void MainWindow::menuComment()
{
    BookDisplayer* currentBookdisp = CurrentBookdisplayer();
    OraytaBookView* view = dynamic_cast<OraytaBookView*>(currentBookdisp->bookView());

    if (!view) return;

    QString link = view->activeLink();

    //Find book's id and add it to the link
    int id = CurrentBookdisplayer()->book()->getRealUniqueId();

    if (id != -1) view->openCommentDialog( QString::number(id) + ":" + link );
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
    OraytaBookView* oview = dynamic_cast<OraytaBookView*>(CurrentBookdisplayer()->bookView());
    if (!oview) return;
    oview->showNikud(showNikud);
}

void MainWindow::toggleTeamim(bool showTeamim)
{
    OraytaBookView* oview = dynamic_cast<OraytaBookView*>(CurrentBookdisplayer()->bookView());
    if (!oview) return;
    oview->showTeamim(showTeamim);
}

void MainWindow::adjustVtabCornerLayout(BookDisplayer* bookdisp)
{
    if (ptrToAdditionalWidget)
    {
        cornerWidgetLayout->removeWidget(ptrToAdditionalWidget);
        ptrToAdditionalWidget->hide();
    }

    ptrToAdditionalWidget = bookdisp->additionalButtons();

    if (ptrToAdditionalWidget)
    {
        ptrToAdditionalWidget->show();
        cornerWidgetLayout->addWidget(ptrToAdditionalWidget);
    }
}

void MainWindow::on_viewTab_currentChanged(int index)
{
    BookDisplayer* currentBookdisp = bookdisplayer(index);
    if (!currentBookdisp) return;  // sanity check

    adjustVtabCornerLayout(currentBookdisp);
    //Show / hide menu options depending on the book:
    adjustMenus();
}

// Show/hide buttons and menus depending on the currently visible book
void MainWindow::adjustMenus()
{
    // Disable / Enable ui->locationMenu
    // ######## if its user book...
    if (CurrentBookdisplayer()->displayedBookType() == NodeBook::Orayta)
        ui->locationMenu->setEnabled(true);
    else
        ui->locationMenu->setEnabled(false);

    //Show / Hide nikud and teamim buttons
    ui->showNikudAction->setVisible( CurrentBookdisplayer()->hasNikud() );
    ui->showTeamimAction->setVisible( CurrentBookdisplayer()->hasTeamim() );
}

void MainWindow::menuErrorReport()
{
    BookDisplayer* currentBookdisp = CurrentBookdisplayer();
    OraytaBookView* view = dynamic_cast<OraytaBookView*>(currentBookdisp->bookView());

    if (!view || !currentBookdisp->book()) return;

    QString link = view->activeLink();

    errorReport *err = new errorReport( this, link, currentBookdisp->book()->getTreeDisplayName() );
    err->show();
}

void MainWindow::on_treeWidget_itemClicked(QTreeWidgetItem* current, int column)
{
    BaseNodeItem* node = dynamic_cast<BaseNodeItem*>(current);

//    qDebug() << "Access to " << node->getTreeDisplayName();

    if (current->checkState(column) == Qt::Checked)
        addNodeToSearch(node);
    else if (current->checkState(column) == Qt::Unchecked)
        removeNodeFromSearch(node);
}

// ##########
//Called when a TreeItem is double clicked
void MainWindow::on_treeWidget_itemDoubleClicked(QTreeWidgetItem* item, int column)
{
    NodeBook* book = dynamic_cast<NodeBook*>(item);
    if ( book ) openBook(book, false);
}

// ######### A gérer ailleurs
void MainWindow::on_treeWidget_currentItemChanged(QTreeWidgetItem* current, QTreeWidgetItem* old)
{
/*   ############ moche... */
    OraytaBookItem* book = dynamic_cast<OraytaBookItem*>(current);
    if ( !book )
    {
        ui->mixedGroup->hide();
        return;
    }

    //Show / Hide mixed display stuff
    if (book->IsMixed())
    {
        ui->mixedGroup->show();

        QVBoxLayout* mixedDisplayLayout = new QVBoxLayout;

        // remove current group box
        if (mixedCheckBox)
        {
            ui->verticalLayout_8->removeWidget(mixedCheckBox);
            delete mixedCheckBox;
        }

        mixedCheckBox = new QGroupBox;

        QSignalMapper *signalMapper = new QSignalMapper(mixedCheckBox);

        //Create new entries
        const QList<weavedSource> weavedSources = book->weavedSources();
        for(int i=1; i < weavedSources.size(); i++)
        {
            MixedCheckBox *chk = new MixedCheckBox(weavedSources[i].Title);
            chk->setChecked(weavedSources[i].show);
            chk->setData(i);

            mixedDisplayLayout->addWidget(chk);

            connect(chk, SIGNAL(clicked()), signalMapper, SLOT(map()));
            signalMapper->setMapping(chk, chk);
        }

        connect(signalMapper, SIGNAL(mapped(QWidget*)), this, SLOT(weavedCheckBoxClicked(QWidget*)));

        ui->showaloneCBX->setChecked(book->ShowAlone());

        mixedCheckBox->setLayout(mixedDisplayLayout);
        mixedCheckBox->setEnabled(!ui->showaloneCBX->isChecked());

        ui->verticalLayout_8->insertWidget(0, mixedCheckBox);
    }
    else ui->mixedGroup->hide();
}

void MainWindow::weavedCheckBoxClicked(QWidget* checkBox)
{
    //Still a bit ugly
    OraytaBookItem* obook = dynamic_cast<OraytaBookItem*>(ui->treeWidget->currentItem());
    MixedCheckBox* mCheckBox = qobject_cast<MixedCheckBox*>(checkBox);

    if (!obook || !mCheckBox) return;

    obook->setWeavedSourceState (mCheckBox->getData(), mCheckBox->checkState());
    obook->setTabWidget( 0 );    /// car l'affichage du livre a changé. ce point devrait eventuellement etre géré ailleurs...

}

void MainWindow::on_showaloneCBX_clicked(bool checked)
{
    mixedCheckBox->setEnabled(!checked);

    OraytaBookItem* obook = dynamic_cast<OraytaBookItem*>(ui->treeWidget->currentItem());
    if ( obook )
    {
        obook->setShowAlone( checked );
        obook->setTabWidget( 0 );
    }
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

    connect(ib, SIGNAL(updateTreeSignal()), ui->treeWidget, SLOT(updateTree()));

    ib->show();
}

void MainWindow::findBookForm()
{
    bookfind * bf = new bookfind(this, ui->treeWidget->booklist());
    bf->show();

    connect(bf, SIGNAL(openBookRequested(NodeBook*)), this, SLOT(menuOpenBook(NodeBook*)));
}

void MainWindow::on_viewTab_tabCloseRequested(int index)
{
    /* // #######  inutile, il me semble
    if (bookdisplayer(index)->book() != NULL)
        bookdisplayer(index)->book()->setTabWidget( 0 ); */

    bookdisplayer(index)->deleteLater();

    //Destroy tab
    ui->viewTab->removeTab(index);

    if (ui->viewTab->count() == 0)
    {
        addViewTab();
    }
}

// ########### Bad Design
void MainWindow::openExternalLink(QString lnk)
{
    QStringList parts = lnk.split(":");

    //qDebug() << "entry in openExternal : " << parts;

    int id;
    if( ToNum(parts[0], &id) )
    {
        NodeBook* book = ui->treeWidget->findBookById(id);
        if( !book ) return;  // sanity check

        // if this book is already open, don't add a new tab
        BookDisplayer* tabWidget = book->tabWidget();
        if ( tabWidget != 0 )
        {
            ui->viewTab->setCurrentWidget(tabWidget);
            tabWidget->setInternalLocation(parts[1]);

            if (parts.size() == 3)
            {
                OraytaBookView* bw = dynamic_cast<OraytaBookView*>(tabWidget->bookView());
                if (bw) bw->highlight( QRegExp(allowNikudAndTeamim(unescapeFromBase64(parts[2]))) );
            }
        }
        else
        {
            // ######## Crade. (si le livre est en mode mixed, on évite de le recharger comme ca...)
            OraytaBookItem* obook = dynamic_cast<OraytaBookItem*>(book);
            if (obook)
            {
                bool memShowAloneOpt = obook->ShowAlone();
                obook->setShowAlone(true);

                openBook(obook, true);

                obook->setShowAlone(memShowAloneOpt);
            }
            else openBook(book, true);

            CurrentBookdisplayer()->setInternalLocation(parts[1]);

            if (parts.size() == 3)
            {
                OraytaBookView* bw = dynamic_cast<OraytaBookView*>(CurrentBookdisplayer()->bookView());
                if (bw) bw->highlight( QRegExp(allowNikudAndTeamim(unescapeFromBase64(parts[2]))) );
            }
        }
    }
}

void MainWindow::on_openMixed_clicked()
{
    OraytaBookItem* book = dynamic_cast<OraytaBookItem*>(ui->treeWidget->currentItem());
    if ( book )
    {
        //If it's the same book:
        if ( CurrentBookdisplayer()->book() == book )
        {
            OraytaBookView* oview = dynamic_cast<OraytaBookView*>(CurrentBookdisplayer()->bookView());
            if (oview) oview->reloadAtSameLocation();
        }
        else
        {
            openBook(book);
        }
    }
}

////////////////////////////////////////////////////
//      search-in-current-book ui controller
////////////////////////////////////////////////////

void MainWindow::on_showSearchBarButton_clicked()
{
    ui->searchGroupBX->show();
    ui->lineEdit->setFocus(Qt::ActiveWindowFocusReason);
}

void MainWindow::on_hideSearchButton_clicked()
{
    ui->searchGroupBX->hide();
}

void MainWindow::toggleSearchBar()
{
    ui->searchGroupBX->setVisible(!ui->searchGroupBX->isVisible());
    if (ui->searchGroupBX->isVisible())
        ui->lineEdit->setFocus(Qt::ActiveWindowFocusReason);
}

void MainWindow::on_lineEdit_returnPressed()
{
    on_searchForward_clicked();
}

void MainWindow::on_searchForward_clicked()
{
    CurrentBookdisplayer()->searchText(ui->lineEdit->text(), false);
}

void MainWindow::on_searchBackward_clicked()
{
    CurrentBookdisplayer()->searchText(ui->lineEdit->text(), true);
}

////////////////////////////////////////////////////
//          global search ui controller
////////////////////////////////////////////////////

void MainWindow::showSearchTab()
{
    ui->treeTab->setCurrentIndex(1);
}

// ############# ugly
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

//////////////////////////////////////////////////////

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


void MainWindow::searchGuematria()
{
    showSearchTab();
    ui->guematriaCheckBox->setChecked(true);
}

BookDisplayer* MainWindow::CurrentBookdisplayer()
{
    return bookdisplayer( CURRENT_TAB );
}

BookDisplayer* MainWindow::bookdisplayer( int index )
{
    QWidget* w = ui->viewTab->widget(index);
    return /*dynamic_cast*/qobject_cast<BookDisplayer*>(w);
}

int MainWindow::currentTab()
{
    return ui->viewTab->currentIndex();
}
