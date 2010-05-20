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


//TODO: Re-build mixed display



//TODO: Remove wierd books

/* KHTML TODOs:
TODO: Finish up KHTML:
    TODO: See what about windows
    TODO: BUG: KHTML anchores miss when zoomed in


TODO: KHTML progress bar
    http://api.kde.org/3.5-api/kdelibs-apidocs/kparts/html/classKParts_1_1ReadOnlyPart.html#a2d8af6ecaf7d70e2670af6420fee7af3
    http://api.kde.org/3.5-api/kdelibs-apidocs/kio/html/classKIO_1_1Job.html#a9727943a6d95ebf8fdccdf8a9c022509
*/

//TODO: Edit welcome image

//TODO: Allow adding custom html books through the program

//TODO: Mixed display choose what to show

//TODO: dictionary

//TODO: allow search to be stopped or canceled

//TODO: disable checkbox & color changes for books not in search

//TODO: Fix the way ספר המידות looks (compare to the original TE)

//TODO: English welcpme page
//TODO: Whole new welcome page
//TODO: לזכר רועי רוט
//(TODO: create help page for linux version)
//TODO: hide welcomepage from search

//TODO: Speed up search. (Better progress bar too?)

//TODO: Speed up HhtmlGen. Especially for mixed books.

//TODO: Add rav kook books

//TODO: Fix עשרת הדיברות's teamim

//TODO: Remove wierd nikud

//TODO: see if printing dialog really appears in hebrew on hebrew locals

//TODO: document & clean code

//TODO: more of the search - two word search, etc'

//TODO: see what else to do ;-)

//TODO: (right align window title)

//TODO: Requested Feature: parallel tab (like in the originall)

//TODO: Fix mixed displays font-family and font size system






// This is just a very simple define. every place in the code,
//  "CURRENT_TAB" simply represents "ui->viewTab->currentIndex()".
//
//  It just made the code much more readable, I couldn't resist
# define CURRENT_TAB ui->viewTab->currentIndex()

//Gloabals:

//Gloabal BookList
BookList gBookList;

//Global webview and hbox vector

#ifndef KHTML
vector<myWebView *> gWebViewList;
#else
vector<KHTMLWidget *> gWebViewList;
#endif

vector<QVBoxLayout *> gVboxList;

int gHtmlCnt = 0;
bool gPhaseOne = false;
QString gInternalLocationInHtml = "";

//Holds the available languages and the way they should be displayed (such as "עברית" for "Hebrew")
QStringList gLangs, gLangsDisplay;

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindowClass)
{
    //Code executed when the window is constructed:

    //Setup the GUI
    ui->setupUi(this);

    //Loading confs
    restoreConfs();

    //Set all QString to work with unicode
    QTextCodec::setCodecForCStrings(QTextCodec::codecForName("utf8"));
    QTextCodec::setCodecForTr(QTextCodec::codecForName("utf8"));

    ui->treeDockWidget->setFloating(false);

    //Adjust for hebrew if needed
    if (LANG == "Hebrew") toRTL();

    //Force the viewTab to take 80 percent of the window
    ui->horizontalLayout->setStretchFactor(ui->viewTab, 80);
    ui->horizontalLayout->setStretchFactor(ui->verticalLayout_9, 20);

    //Close the tab:
    ui->viewTab->removeTab(0);
    //Now add a good one:
    addViewTab(false);

    //Set window title:
    setWindowTitle(tr("Orayta"));

    //Generate the BookList(s), and put it in the tree
    gBookList.BuildFromFolder(BOOKPATH);

    if (gBookList.size() == 0)
    {
        QMessageBox msgBox;
        msgBox.setText(tr("No books found! \nCheck your installation, or contact the developer."));
        msgBox.exec();

        //No books found
        exit(2);
    }

    BuildBookTree();

    //Sort the book tree by the second column ( that holds the whole name, including the number before it)
    ui->treeWidget->setColumnCount(2);  //Small hack to allow sorting the right way...
    ui->treeWidget->sortItems(1, Qt::AscendingOrder);
    ui->treeWidget->setColumnCount(1);

    //Load start page. Assuming it's book[0] of course.

    LoadBook(gBookList[0]);


    ui->mixedGroup->hide();
    ui->verticalLayout_11->setDirection(QBoxLayout::BottomToTop);

    ui->searchDockWidget->setMaximumHeight(30);

    ui->viewTab->setCornerWidget(ui->newTabButton, Qt::TopRightCorner);

    ui->treeWidget->setHeaderHidden(true);
    //Hack for windows:
    ui->treeWidget->setColumnWidth(0,1000);

    //Load the bookmarks into thier list:
    buildBookMarkList();

    connectMenuActions();

    //Set available languages
    gLangs << "Hebrew" << "English" << "French";
    gLangsDisplay << "עברית" << "English" << "Français";;

    //Show available languages in the language combobox
    for (int i=0; i<gLangs.size(); i++)
    {
        ui->langComboBox->addItem(gLangsDisplay[i]);
    }

    //Show current language
    int is = -1;
    for (int i=0; i<gLangs.size(); i++) if (LANG == gLangs[i]) is = i;
    ui->langComboBox->setCurrentIndex(is);

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
}

//Switch GUI direction to RTL
void MainWindow::toRTL()
{
    this->setLayoutDirection(Qt::RightToLeft);
    ui->dockWidgetContents->setLayoutDirection(Qt::RightToLeft);
    ui->dockWidgetContents_2->setLayoutDirection(Qt::RightToLeft);

    ui->treeDockWidget->setAllowedAreas(Qt::RightDockWidgetArea);

    removeDockWidget(ui->treeDockWidget);
    addDockWidget(Qt::RightDockWidgetArea, ui->treeDockWidget);
    ui->treeDockWidget->show();

    ui->treeTab->setLayoutDirection(Qt::RightToLeft);
    ui->viewTab->setLayoutDirection(Qt::RightToLeft);

    ui->treeWidget->setLayoutDirection(Qt::RightToLeft);

    ui->searchForward->setIcon(QIcon(":/Icons/search-left.png"));
    ui->searchBackwords->setIcon(QIcon(":/Icons/search-right.png"));

    ui->searchForwardAction->setIcon(QIcon(":/Icons/search-left.png"));
    ui->searchBackwardsAction->setIcon(QIcon(":/Icons/search-right.png"));

    ui->scrollArea->setLayoutDirection(Qt::RightToLeft);

    ui->menuBar->setLayoutDirection(Qt::RightToLeft);
    ui->menu->setLayoutDirection(Qt::RightToLeft);
    ui->menu_2->setLayoutDirection(Qt::RightToLeft);
    ui->menu_3->setLayoutDirection(Qt::RightToLeft);
    ui->menu_4->setLayoutDirection(Qt::RightToLeft);
    ui->menu_5->setLayoutDirection(Qt::RightToLeft);
}

void MainWindow::connectMenuActions()
{
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

    connect(ui->advancedSearchAction, SIGNAL(triggered()), this, SLOT(advancedSearch()));
    connect(ui->searchForwardAction, SIGNAL(triggered()), this, SLOT(on_searchForward_clicked()));
    connect(ui->searchBackwardsAction, SIGNAL(triggered()), this, SLOT(on_searchBackwords_clicked()));

    connect(ui->addAllToSearchAction, SIGNAL(triggered()), this, SLOT(on_addAllToSearchButton_clicked()));
    connect(ui->removeAllFromSearchAction, SIGNAL(triggered()), this, SLOT(on_removeAllFromSearchButton_clicked()));
    connect(ui->addToSearchAction, SIGNAL(triggered()), this, SLOT(addToSearch()));
    connect(ui->removeFromSearchAction, SIGNAL(triggered()), this, SLOT(removeFromSearch()));

}

MainWindow::~MainWindow()
{
    //Remove all html tmp files created
    QFile f;
    for (int i=0; i <= gHtmlCnt; i++)
    {
        f.remove(TMPPATH + "Orayta" + stringify(i) + ".html");
        f.remove(TMPPATH + "Orayta" + stringify(i) + ".html.header.html");
    }

    //TODO: Don't I need to free all the items?
    gWebViewList.clear();
    gVboxList.clear();
    delete ui;
}


//Overrides the normal "closeEvent", so it can save tha window's state before quiting
void MainWindow::closeEvent(QCloseEvent *event)
{
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

    QFont f (gFontFamily, gFontSize);
    ui->fontPreview->setFont(f);
    ui->fontComboBox->setCurrentFont(f);
    ui->fonSizeSpinBox->setValue(gFontSize);
    settings.endGroup();
}

void MainWindow::BuildBookTree()
{
    //Add treeItems for each book to the treeWidget
    for(unsigned int i=0;i<gBookList.size();i++)
    {
        if (gBookList[i]->IsHidden() != true)
        {
            QTreeWidgetItem *twi;
            if(gBookList[i]->getParent() == NULL)
                twi = new QTreeWidgetItem(ui->treeWidget);
            else
                twi = new QTreeWidgetItem(gBookList[i]->getParent()->getTreeItemPtr());

            gBookList[i]->setTreeItemPtr(twi);

            //Just for the sorting...
            twi->setText(1, gBookList[i]->getName());


            //Show a checkbox next to the item
            twi->setCheckState(0,Qt::Checked);


            QString dn;
            if(gBookList[i]->getTreeDisplayName() != "")
                dn = gBookList[i]->getTreeDisplayName();
            else if (gBookList[i]->getNormallDisplayName() != "")
                dn = gBookList[i]->getNormallDisplayName();
            else
            {
                vector<QString> name_parts;
                splittotwo(gBookList[i]->getName(), name_parts, "_");
                dn = name_parts[1];
            }
            dn = dn.replace("שס", "ש\"ס");
            twi->setText(0, dn);

            //set the icon:
            QIcon *icon = bookIcon(gBookList[i]->IsDir(), gBookList[i]->isMixed(), BLUE);

            twi->setIcon(0, *icon);

            delete icon;
        }
    }

    //Only after the tree is done, connect the itemChanged signal
    connect (ui->treeWidget, SIGNAL(currentItemChanged(QTreeWidgetItem*,QTreeWidgetItem*)), this, SLOT(treeWidgetSelectionChanged(QTreeWidgetItem*,QTreeWidgetItem*)));
}

//Load the generated Html file of the given book into the WebView widget
void MainWindow::LoadBook(Book *book, QString markString)
{
    ui->progressBar->show();
    ui->progressBar->setValue(10);

    //NikudMode nikudmode = gWebViewList[CURRENT_TAB]->mNikudMode;
    bool shownikud = gWebViewList[CURRENT_TAB]->ShowNikud;
    bool showteamim = gWebViewList[CURRENT_TAB]->ShowTeamim;

    QApplication::setOverrideCursor(Qt::WaitCursor);

    gHtmlCnt ++;
    //Check if the file renders OK
    if (book->htmlrender(TMPPATH + "Orayta" + stringify(gHtmlCnt) + ".html", shownikud, showteamim, markString))
    {
        if(gInternalLocationInHtml == "")
        {
            //Load html header only; The rest will be loaded when this is done
#ifndef KHTML
            ui->progressBar->setValue(50);
            gPhaseOne = true;
            gWebViewList[CURRENT_TAB]->load(QUrl(TMPPATH + "Orayta" + stringify(gHtmlCnt) + ".html" + ".header.html"));
#else
            gWebViewList[CURRENT_TAB]->load(QUrl(TMPPATH + "Orayta" + stringify(gHtmlCnt) + ".html"));
#endif
        }
        else
        {
            QString p =  absPath(TMPPATH + "Orayta" + stringify(gHtmlCnt) + ".html" + gInternalLocationInHtml);
            gWebViewList[CURRENT_TAB]->load(QUrl(p));
        }
        gWebViewList[CURRENT_TAB]->setBook(book);
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
    //

    //Show / Hide nikud and teamim buttons
    ui->showNikudAction->setVisible( book->hasNikud );
    ui->showTeamimAction->setVisible( book->hasTeamim );
}

//Signall omitted when the webView finishes loading a file
#ifdef KHTML
void MainWindow::webView_loadFinished()
{
    if (gWebViewList[CURRENT_TAB]->url().toString() == "") return;
#else
    void MainWindow::webView_loadFinished(bool)
    {
#endif
        QString s="";

        //If only header was loaded, load the full page now
        if(gPhaseOne == true)
        {
            ui->progressBar->show();
            ui->progressBar->setValue(20);

            gPhaseOne = false;

            //Load the full page
            ui->viewTab->setTabText(CURRENT_TAB, tr("Loading..."));


            QString str = gWebViewList[CURRENT_TAB]->url().toString();
            if (str.right(12) == ".header.html")
            {
                //Cut off the ".header.html" from the path
                s = str.mid(0, str.length() -  12);

                gWebViewList[CURRENT_TAB]->load(QUrl(s));
            }
        }
        else
        {
            //change the tabs title from "Loading..." to the book's title
            ui->viewTab->setTabText(CURRENT_TAB, gWebViewList[CURRENT_TAB]->title());

            //Hide the progressbar
            ui->progressBar->hide();

            if ( gInternalLocationInHtml != "" && gWebViewList[CURRENT_TAB]->url().toString().indexOf("Empty") == -1 )
            {
                //Mark location as "active"
                QString script = "paintByHref(\"" + gInternalLocationInHtml.replace("#", "$") + "\");";

                gWebViewList[CURRENT_TAB]->execScript(script);

#ifndef KHTML
                gInternalLocationInHtml="";

                //Scroll up in the page a bit
                if ( gWebViewList[CURRENT_TAB]->page()->mainFrame()->scrollBarMaximum(Qt::Vertical) != gWebViewList[CURRENT_TAB]->page()->mainFrame()->scrollBarValue(Qt::Vertical) )
                {
                    gWebViewList[CURRENT_TAB]->page()->mainFrame()->setScrollBarValue(Qt::Vertical, gWebViewList[CURRENT_TAB]->page()->mainFrame()->scrollBarValue(Qt::Vertical) - 70);
                }
#endif
            }
        }

        //Return cusror to normal
        QApplication::restoreOverrideCursor();
    }

    void MainWindow::webView_loadProgress (int progress)
    {
        ui->progressBar->setValue(progress);
    }

    void MainWindow::webView_loadStarted()
    {
        ui->progressBar->setValue(0);
    }


    QString last = "";

    //Omitted when a link was clicked in the webView
    void MainWindow::webView_linkClicked(QUrl url)
    {
        QString link = QString(url.path());

#ifdef KHTML
        //Yuck!!! Disgusting!!!
        if (link == last)
        {
            last = "";
            return;
        }
        last = link;
#endif

        //Not actually a link. A menu should open here
        if(link.indexOf("$") != -1 )
        {
            //Find book's id and add it to the link
            int id = gWebViewList[CURRENT_TAB]->book()->getUniqueId();

            int p = link.indexOf("$");
            link = stringify(id) + ":" + link.mid(p+1);

#ifdef KHTML
            QMenu menu(gWebViewList[CURRENT_TAB]->getQWidget());
#else
            QMenu menu(gWebViewList[CURRENT_TAB]);
#endif
            QAction *mark = new QAction(tr("Add bookmark here..."), &menu);

            mark->setIcon(QIcon(":/Icons/bookmarks.png"));
            menu.addAction(mark);

            QSignalMapper *signalMapper = new QSignalMapper(this);

            signalMapper->setMapping(mark, QString(link));
            connect(mark, SIGNAL(triggered()), signalMapper, SLOT (map()));
            connect(signalMapper, SIGNAL(mapped(const QString &)), this, SLOT(bookMarkPosition(QString)));

            QAction *comment = new QAction(tr("Add/edit comment..."), &menu);

            menu.addAction(comment);
            comment->setIcon(QIcon(":/Icons/edit.png"));

            QSignalMapper *csignalMapper = new QSignalMapper(this);
            csignalMapper->setMapping(comment, QString(link));
            connect(comment, SIGNAL(triggered()), csignalMapper, SLOT (map()));
            connect(csignalMapper, SIGNAL(mapped(const QString &)), this, SLOT(openCommentDialog(QString)));

            menu.setLayoutDirection(Qt::RightToLeft);

            //Open the menu to the left of the cursor's position
            QPoint pos = QPoint(QCursor::pos().x() - menu.width(), QCursor::pos().y());
            menu.exec(pos);

            delete signalMapper;
            delete csignalMapper;
        }
        //A comment clicked. A menu should open here too
        else if(link.indexOf("*") != -1 )
        {
            //Find book's id and add it to the link
            int id = gWebViewList[CURRENT_TAB]->book()->getUniqueId();

            int p = link.indexOf("*");
            link = stringify(id) + ":" + link.mid(p+1);


            QMenu menu(ui->treeWidget);

            QSignalMapper *signalMapper = new QSignalMapper(this);

            QAction *comment = new QAction(tr("Edit comment..."), &menu);

            comment->setIcon(QIcon(":/Icons/edit.png"));
            menu.addAction(comment);

            QSignalMapper *csignalMapper = new QSignalMapper(this);
            csignalMapper->setMapping(comment, QString(link));
            connect(comment, SIGNAL(triggered()), csignalMapper, SLOT (map()));
            connect(csignalMapper, SIGNAL(mapped(const QString &)), this, SLOT(openCommentDialog(QString)));


            QAction *delcomment = new QAction(tr("Delete comment"), &menu);
            delcomment->setIcon(QIcon(":/Icons/edit-delete.png"));
            menu.addAction(delcomment);

            QSignalMapper *dcsignalMapper = new QSignalMapper(this);
            dcsignalMapper->setMapping(delcomment, QString(link));
            connect(delcomment, SIGNAL(triggered()), dcsignalMapper, SLOT (map()));
            connect(dcsignalMapper, SIGNAL(mapped(const QString &)), this, SLOT(removeComment(QString)));

            menu.setLayoutDirection(Qt::RightToLeft);

            //Open the menu to the left of the cursor's position
            QPoint pos = QPoint(QCursor::pos().x() - menu.width(), QCursor::pos().y());
            menu.exec(pos);

            delete signalMapper;
            delete csignalMapper;
            delete dcsignalMapper;
        }

        //External book link
        else if(link.indexOf("!") != -1 )
        {
            int pos = link.indexOf("!");

            QString lnk = "";
            lnk = link.mid(pos+1);

            vector<QString> parts;
            split(lnk, parts, ":");
            int id;
            if(ToNum(parts[0], &id))
            {
                int index = gBookList.FindBookById(id);
                if( index != -1)
                {
                    gInternalLocationInHtml = "#" + parts[1];

                    //Add a new tab and open the link there
                    addViewTab(false);
                    ui->viewTab->setTabText(CURRENT_TAB, tr("Orayta"));

                    ///%%%%%%%%%
                    if (parts.size() == 3)
                    {
                        LoadBook(gBookList[index], parts[2]);
                    }
                    else
                        LoadBook(gBookList[index]);
                }
            }
        }
        //Link to website
        else if(link.indexOf("^") != -1 )
        {
            int pos = link.indexOf("^");

            QString lnk = "";
            lnk = link.mid(pos+1);

            //Open using browser
            QDesktopServices::openUrl("http://" + lnk);
        }
    }

//Called when a TreeItem is double clicked
void MainWindow::on_treeWidget_itemDoubleClicked(QTreeWidgetItem* item, int column)
{
    int ind = gBookList.FindBookByTWI(item);

    //Open the selected book
    openBook( ind );
}

//Open the given book (by it's id in the booklist)
void MainWindow::openBook( int ind )
{
    if (ind >= 0 && ind < gBookList.size())
    {
        if(gBookList[ind]->IsDir() == false)
        {
            //If the filename ends with ".html", load it as is, without rendering
            if((gBookList[ind]->getPath().right(5) == ".html") || (gBookList[ind]->getPath().right(4) == ".htm"))
            {
                gWebViewList[CURRENT_TAB]->load( QUrl( gBookList[ind]->getPath()));
            }
            else
            {
                LoadBook(gBookList[ind]);
            }
        }
    }
}

//Calls "openBook", but uses the currently selected book in the tree
void MainWindow::openSelectedBook( )
{
    int ind = gBookList.FindBookByTWI(ui->treeWidget->selectedItems()[0]);

    openBook(ind);
}

//Opens a new tab and Calls "openBook" (using the currently selected book in the tree).
void MainWindow::openSelectedBookInNewTab()
{
    addViewTab(false);

    int ind = gBookList.FindBookByTWI(ui->treeWidget->selectedItems()[0]);
    openBook(ind);
}

void MainWindow::on_newTabButton_clicked()
{
    //Create the new tab and switch to it
    addViewTab(true);
}

void MainWindow::addViewTab(bool empty)
{
    //Create new tab:
    QWidget *tab = new QWidget();
    ui->viewTab->addTab(tab, tr("Orayta"));

    //Create new webview
#ifdef KHTML
    KHTMLWidget *newview = new KHTMLWidget(this);
#else
    myWebView *newview = new myWebView();
#endif

    gWebViewList.push_back(newview);

    //Connect the signalls sent from the new widgets to their slots
#ifdef KHTML
    QObject::connect(newview, SIGNAL(completed()), this , SLOT(webView_loadFinished()));
    QObject::connect(newview, SIGNAL(LinkClicked(QUrl)), this , SLOT(webView_linkClicked(QUrl)));
#else
    //QObject::connect(newview, SIGNAL(started(KIO::Job*)), this , SLOT(webView_loadStarted(KIO::Job*)));
    QObject::connect(newview, SIGNAL(loadFinished(bool)), this , SLOT(webView_loadFinished(bool)));
    QObject::connect(newview, SIGNAL(linkClicked(QUrl)), this , SLOT(webView_linkClicked(QUrl)));
    QObject::connect(newview, SIGNAL(loadStarted()), this , SLOT(webView_loadStarted()));
    QObject::connect(newview, SIGNAL(loadProgress (int)), this , SLOT(webView_loadProgress(int)));

    //Prevent context menus. If needed, they will be created manually
    newview->setContextMenuPolicy(Qt::PreventContextMenu);

    //The myWebView class will handle all link, but will emit "LinkMenu" when one should be shown
    newview->page()->setLinkDelegationPolicy(QWebPage::DontDelegateLinks);
#endif
    QObject::connect(newview, SIGNAL(LinkClicked(QUrl)), this, SLOT(webView_linkClicked(QUrl)));

    //Create new hbox
    QVBoxLayout *vbox = new QVBoxLayout(tab);
    gVboxList.push_back(vbox);
#ifdef KHTML
    vbox->addWidget(newview->getQWidget());
#else
    vbox->addWidget(newview);
#endif
    vbox->setMargin(0);

    if (empty == true) newview->setHtml("<html></html>");

    //Switch to the new tab
    ui->viewTab->setCurrentIndex(ui->viewTab->count()-1);
}

void MainWindow::on_lineEdit_returnPressed()
{
    simpleSearch(ui->lineEdit->text());
}

void MainWindow::on_searchButton_clicked()
{
    simpleSearch(ui->lineEdit->text());
}

void MainWindow::addToSearch()
{
    int ind = gBookList.FindBookByTWI(ui->treeWidget->currentItem());
    if (ind != -1)
        gBookList[ind]->select();

    ui->addToSearchAction->setEnabled(false);
    ui->removeFromSearchAction->setEnabled(true);
}

void MainWindow::on_addAllToSearchButton_clicked()
{
    for(unsigned int i=0; i<gBookList.size(); i++)
    {
        gBookList[i]->select();
    }

    ui->addToSearchAction->setEnabled(false);
    ui->removeFromSearchAction->setEnabled(true);
}

void MainWindow::removeFromSearch()
{
    int ind = gBookList.FindBookByTWI(ui->treeWidget->currentItem());
    if (ind != -1)
        gBookList[ind]->unselect();

    ui->addToSearchAction->setEnabled(true);
    ui->removeFromSearchAction->setEnabled(false);
}

void MainWindow::on_removeAllFromSearchButton_clicked()
{
    for(unsigned int i=0; i<gBookList.size(); i++)
        gBookList[i]->unselect();

    ui->addToSearchAction->setEnabled(true);
    ui->removeFromSearchAction->setEnabled(false);
}

void MainWindow::on_zoominButton_clicked()
{
    gWebViewList[CURRENT_TAB]->setTextSizeMultiplier(gWebViewList[CURRENT_TAB]->textSizeMultiplier() + 0.1);
}

void MainWindow::on_zoomoutButton_clicked()
{
    if (gWebViewList[CURRENT_TAB]->textSizeMultiplier() > 0.1)
    {
        gWebViewList[CURRENT_TAB]->setTextSizeMultiplier(gWebViewList[CURRENT_TAB]->textSizeMultiplier() - 0.1);
    }
}


void MainWindow::on_topButton_clicked()
{
    //Jump to the top of the page using javascript
    gWebViewList[CURRENT_TAB]->execScript("window.scrollTo(0, 0)");
}


void MainWindow::advancedSearch()
{
    SearchForm *sf = new SearchForm(this);

    connect(sf, SIGNAL(Search(QRegExp,QString)), this, SLOT(SearchInBooks(QRegExp,QString)));
    connect(sf, SIGNAL(Search(QString)), this, SLOT(simpleSearch(QString)));

    sf->show();
}

void MainWindow::simpleSearch(QString phrase)
{
    SearchInBooks(withNikudAndTeamim(phrase) , phrase);
}

#define RESULTS_MAX 200
#define CHAR_LIMIT 200
#define LINES_TO_SHOW 6

void MainWindow::SearchInBooks (QRegExp regexp, QString disp)
{
    //TODO: find two words with line break between them

    //TODO: remove gmara externall links from preview
    //TODO: remove level signs from preview

    QString title, Html="",Htmlhead="", HtmlTopLinks="";

    int results = 0;
    vector <QString> text;

    if ( regexp.pattern() != "" )
    {
        ui->progressBar->show();
        ui->progressBar->setValue(5);
        float percentPerBook = (90.0 / gBookList.size());

        //Make a new tab if the current tab has something in it
        if (ui->viewTab->tabText(CURRENT_TAB).indexOf(tr("Orayta")) == -1)
        {
            addViewTab(false);
            gWebViewList[CURRENT_TAB]->setTextSizeMultiplier(1);
            gWebViewList[CURRENT_TAB]->setHtml("<html></html>");
        }

        //Set the title of the tab to show what it's searching for
        title = tr("Searching: "); title += "\"" + disp + "\"" + " ...";
        ui->viewTab->setTabText(CURRENT_TAB,title);

        //Head and title of the Html
        title = "תוצאות חיפוש: "; title +="\"" + disp+ "\"";
        Htmlhead = html_head(title);

        Htmlhead += "<body><div class=\"Section1\" dir=\"RTL\">";
        Htmlhead += "<div style=\"font-size:30px\"><b><i><center>";
        Htmlhead += title + ":" + "</center></i></b></div><BR>";

        Htmlhead += "\n<span style=\"font-size:17px\">";

        for (int i=0; i<gBookList.size() && (results <= RESULTS_MAX); i++)
        {
            ui->progressBar->setValue( 5 + (i + 1) * percentPerBook ) ;

            if (gBookList[i]->IsDir() == false && !gBookList[i]->isMixed() && gBookList[i]->IsInSearch() == true)
            {
                text.clear();

                if(!ReadFileToVector(absPath(gBookList[i]->getPath()), text, "UTF-8" , true))
                {
                    //File not found
                    print("ERROR! file " + absPath(gBookList[i]->getPath()) + " not found.");
                }

                else
                {
                    //Iter holding the current position in the book
                    BookIter itr;
                    for (int j=0; j<text.size(); j++)
                    {
                        if ((text[j][0] == '!') || (text[j][0] == '~') || (text[j][0] == '@') || (text[j][0] == '#') || (text[j][0] == '^'))
                        {
                            itr.SetLevelFromLine(text[j]);
                        }
                        else if (text[j].mid(0,6) == "<!--ex")
                        { //Ignore lines starting with "<!--ex"
                        }
                        //A line that should be searched
                        else
                        {
                            QString thisline = text[j];

                            //Remove all HTML tags in this line
                            thisline.replace( QRegExp("<[^>]*>"), "" );
                            thisline = removeTeamim(thisline);

                            //Find if the search phrase appears is in this line (after omitting html tags)
                            int pos = thisline.indexOf (regexp);
                            while (pos < thisline.length() && pos!= -1 && (results <= RESULTS_MAX))
                            {
                                results ++;

                                //Get the text best to show for this reult's description
                                QString linkdisplay = "";

                                //Regular books:
                                if (gBookList[i]->getNormallDisplayName().indexOf("תלמוד") == -1)
                                {

                                    //This is an overkill, but I couldn't resist:
                                    //linkdisplay += BookSearchDisplay (gBookList[i]->getNormallDisplayName() ,itr.toHumanString());

                                    linkdisplay += gBookList[i]->getNormallDisplayName() + " " + itr.toHumanString();
                                }
                                //Gmarot:
                                else
                                {
                                    linkdisplay += gBookList[i]->getNormallDisplayName().mid(13) + " ";
                                    linkdisplay += itr.toGmaraString();
                                }

                                //Add a small link (at the inex) to the full result
                                HtmlTopLinks += reddot() + "&nbsp&nbsp&nbsp<a href=\"#" + stringify(results) + "\">" + linkdisplay  + "</a><BR>";

                                //Add the full result to the page
                                Html += "<span style=\"font-size:23px\">";
                                Html += "<a name=\"" + stringify(results) + "\"></a>";

                                Html += "<a href=!" + stringify(gBookList[i]->getUniqueId()) + ":";
                                Html += itr.toStringForLinks();
                                Html += ":" + escapeToBase32(regexp.pattern()) + ">";

                                Html += linkdisplay;
                                Html += "</a><BR></span>\n";


                                QString str = "";

                                for (int k = j-LINES_TO_SHOW; k < j ; k++)
                                {
                                    if (( k > 0) && ( k < text.size()))
                                    {
                                        if (text[k][0] == '!')
                                            str += "(" +  text[k].mid(3, text[k].length() - 4) + ") ";
                                        else if ((text[k][0] != '~') || (text[k][0] != '@') || (text[k][0] != '#') || (text[k][0] != '^'))
                                            str += text[k] + " ";
                                    }
                                }
                                //Remove HTML tags and nikud
                                str.replace( QRegExp("<[^>]*>"), "" );
                                str = removeTeamim(str);


                                //Chop to the maximum length
                                //Html += startChop(str, CHAR_LIMIT);


                                str += thisline.mid(0,pos);

                                Html += startChop(str, CHAR_LIMIT);


                                Html += "<span style=\"background-color:#FFF532\">";
                                Html += thisline.mid(pos, regexp.cap(0).length());
                                Html += "</span>";


                                str = "";
                                str += thisline.mid(pos + regexp.cap(0).length()) + " ";

                                for (unsigned int k=1; k <= LINES_TO_SHOW ; k++)
                                {
                                    if (j+k < text.size())
                                    {
                                        if (text[j+k][0] == '!')
                                            str += "(" +  text[j+k].mid(3, text[j+k].length() - 4) + ") ";
                                        else if ((text[j+k][0] != '~') || (text[j+k][0] != '@') || (text[j+k][0] != '#') || (text[j+k][0] != '^'))
                                            str += text[j+k] + " ";
                                    }
                                }
                                //Remove HTML tags and nikud
                                str.replace( QRegExp("<[^>]*>"), "" );
                                str = removeTeamim(str);

                                //Chop to the maximum length
                                Html += endChop(str, CHAR_LIMIT);

                                Html += "<br><br><br>\n";

                                //Search the rest of the line:
                                pos = thisline.indexOf (regexp, pos + 1);

                            }
                        }
                    }
                }
            }
        }
        if(results == 0)
        {
            //TODO: write better explenation
            Htmlhead +="<BR><BR>"; Htmlhead += tr("No serach results found:"); Htmlhead += "\"" + disp + "\"";
            Htmlhead += "</B><BR>";
        }
        else
        {
            Htmlhead += "<B>"; Htmlhead += tr("Short result list: "); Htmlhead += "</B><BR>";
            if (results >= RESULTS_MAX)
            {
                Htmlhead += "(" +  stringify(RESULTS_MAX) + " ";
                Htmlhead += "תוצאות ראשונות בלבד )"; Htmlhead += "<BR><BR>";
            }
            Htmlhead += HtmlTopLinks;

            Htmlhead += "<BR><BR><B>" +  tr("Full result list:") + "</B><BR><BR>";
        }
        Html = Htmlhead + Html;

        Html += "</span></div>\n";
        Html += "\n</body>\n</html>";

        gHtmlCnt ++;
        writetofile(TMPPATH + "Orayta" + stringify(gHtmlCnt) + ".html", Html, "UTF-8");

        gWebViewList[CURRENT_TAB]->load(QUrl(TMPPATH + "Orayta" + stringify(gHtmlCnt) + ".html"));
    }
}


void MainWindow::on_lineEdit_textChanged(QString text)
{
#ifdef KHTML
    gWebViewList[ui->viewTab->currentIndex()]->buildSearch(text);
#endif
}


QString lastSearch = "";

///TODO: Fix for KHTML
void MainWindow::on_searchForward_clicked()
{
    if (ui->lineEdit->text().replace(" ","") == "") return;

    QRegExp regexp = withNikudAndTeamim(ui->lineEdit->text());

    //BEWARE: Amaizingly ugly hack! Watch out!

    //Mark current cursor position using JS, grab the Html, and hide the marking
    gWebViewList[CURRENT_TAB]->execScript("markCursorPos();");

#ifdef KHTML
    QString text = gWebViewList[ui->viewTab->currentIndex()]->document().toString().string();
#else
    QString text = gWebViewList[ui->viewTab->currentIndex()]->page()->mainFrame()->toPlainText();
#endif

    int ppp = text.indexOf("**|*|**");

    //Hide the marking in the html
    gWebViewList[CURRENT_TAB]->execScript("unMarkCursorPos();");

    //Search only in the text after the cursor's position
    if (ppp != -1) text = text.mid(ppp + 7 + 1);

    //Find the closest occurence of the RegExp
    QString next = "";

    int pos = regexp.indexIn(text);
    if (pos > -1)
    {
        next = regexp.cap(0);
    }

#ifdef KHTML
    if (next != lastSearch) gWebViewList[ui->viewTab->currentIndex()]->buildSearch (next);
    lastSearch = next;

    //Now make the webview search for that normally
    gWebViewList[ui->viewTab->currentIndex()]->searchText(false);
#else
    //Now make the webview search for that normally
    if (next != "") gWebViewList[ui->viewTab->currentIndex()]->findText(next,0);
#endif
}

void MainWindow::on_searchBackwords_clicked()
{
    if (ui->lineEdit->text().replace(" ","") == "") return;

    //BEWARE: Amaizingly ugly hack! Watch out!

    //Mark current cursor position using JS, grab the Html, and hide the marking
    gWebViewList[CURRENT_TAB]->execScript("markCursorPos();");

#ifdef KHTML
    QString text = gWebViewList[ui->viewTab->currentIndex()]->document().toString().string();
#else
    QString text = gWebViewList[ui->viewTab->currentIndex()]->page()->mainFrame()->toPlainText();
#endif

    int ppp = text.indexOf("**|*|**");
    int ppb = text.indexOf("</SCRIPT");

    //Hide the marking in the html
    gWebViewList[CURRENT_TAB]->execScript("unMarkCursorPos();");

    //Search only in the text before the cursor's position
    if (ppp != -1) text = text.mid(ppb, ppp - 1 - ppb);

    QRegExp regexp = withNikudAndTeamim(ui->lineEdit->text());

    //Find the closest occurence of the RegExp, backwards
    QString last = "";

    int pos = regexp.lastIndexIn(text);
    if (pos > -1) {
        last = regexp.cap(0);
    }
    else return;

#ifdef KHTML
    if (last != lastSearch) gWebViewList[ui->viewTab->currentIndex()]->buildSearch (last);
    lastSearch = last;

    //Now make the webview search for that normally
    gWebViewList[ui->viewTab->currentIndex()]->searchText(true);
#else
    //Now make the webview search for that normally
    if (last != "") gWebViewList[ui->viewTab->currentIndex()]->findText(last,QWebPage::FindBackward);
#endif
}

void MainWindow::on_treeWidget_customContextMenuRequested(QPoint pos)
{
    QTreeWidgetItem * item = 0 ;
    item = ui->treeWidget->itemAt(pos);

    int ind = -1;
    if ( item != 0 )
    {
        ind = gBookList.FindBookByTWI(item);
    }

    if( ind != -1)
    {
        if ( gBookList[ind]->IsDir() == false)
        {
            QMenu menu(ui->treeWidget);

            QAction *openbook = new QAction(tr("Open book"), &menu);
            openbook->setIcon(QIcon(":/Icons/book-blue.png"));
            QAction *openbooknewtab = new QAction(tr("Open in new tab"), &menu);
            openbooknewtab->setIcon(QIcon(":/Icons/tab-new.png"));

            QObject::connect(openbook, SIGNAL(triggered()), this , SLOT(openSelectedBook()));
            QObject::connect(openbooknewtab, SIGNAL(triggered()), this , SLOT(openSelectedBookInNewTab()));

            menu.addAction(openbook);
            menu.addAction(openbooknewtab);

            menu.setLayoutDirection(Qt::RightToLeft);

            //Open the menu to the left of the cursor's position
            QPoint point = QPoint(QCursor::pos().x() - menu.width(), QCursor::pos().y());
            menu.exec(point);
        }
    }
}


void MainWindow::on_bookmarkWidget_customContextMenuRequested(QPoint pos)
{
    int row = ui->bookmarkWidget->currentRow();

    QMenu menu(ui->treeWidget);

    QAction *edit = new QAction(tr("Edit bookmark title..."), &menu);
    edit->setIcon(QIcon(":/Icons/edit.png"));

    QAction *del = new QAction(tr("Delete bookmark"), &menu);
    del->setIcon(QIcon(":/Icons/edit-delete.png"));

    //Im to lazy to do this the right way...
    QObject::connect(edit, SIGNAL(triggered()), this , SLOT(on_bookmarkEdit_clicked()));
    QObject::connect(del, SIGNAL(triggered()), this , SLOT(on_removeBookmark_clicked()));

    menu.addAction(edit);
    menu.addAction(del);

    menu.setLayoutDirection(Qt::RightToLeft);

    //Open the menu to the left of the cursor's position
    QPoint point = QPoint(QCursor::pos().x() - menu.width(), QCursor::pos().y());
    menu.exec(point);
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
            int ind = gBookList.FindBookByTWI(ui->treeWidget->selectedItems()[0]);
            if (ind != -1)
            {
                if ( gBookList[ind]->IsDir() )
                {
                    if ( ui->treeWidget->selectedItems()[0]->isExpanded() ) ui->treeWidget->collapseItem(ui->treeWidget->selectedItems()[0]);
                    else ui->treeWidget->expandItem(ui->treeWidget->selectedItems()[0]);
                }
                else openBook(ind);
            }
        }
        if (ui->bookmarkWidget->hasFocus())
        {
            //Act as if the current item was clicked
            on_bookmarkWidget_itemDoubleClicked(ui->bookmarkWidget->currentItem());
        }
    }

    ////TODO: probably useless now
    //Ctrl was pressed when this key event happend
    if (keyEvent->modifiers() == Qt::CTRL)
    {
        //If this key is "C" or "X":
        if ( keyEvent->key() == Qt::Key_C || keyEvent->key() == Qt::Key_X)
        {
#ifndef KHTML
            //Call the copy function of the current webView
            gWebViewList[ui->viewTab->currentIndex()]->triggerPageAction(QWebPage::Copy);
#endif
        }
    }
}

//Print the current book (all pages I think...)
void MainWindow::printBook()
{
#ifndef KHTML
    QPrinter printer;

    QPrintDialog *dialog = new QPrintDialog(&printer, this);
    dialog->setWindowTitle("הדפס ספר");
    if (dialog->exec() != QDialog::Accepted)
        return;
    else
    {
        gWebViewList[CURRENT_TAB]->print(&printer);
    }
#else
    gWebViewList[CURRENT_TAB]->view()->print();
#endif
}

QString MainWindow::bookMarkTitle(QString lnk)
{
    //Format position for human display
    int p = lnk.indexOf(":");
    QString lnkdisplay = lnk.mid(p+1);
    lnkdisplay = unescapeFromBase32(lnkdisplay).replace("_", " ").replace("-", " ").replace("{", "").replace("}", "").replace(" 0", "");

    int id;
    ToNum(lnk.mid(0,p), &id);

    //Find and add the book's name
    int index = gBookList.FindBookById(id);
    if (index == -1)
    {
        print ("Invalid link!");
        return "";
    }
    else lnkdisplay = gBookList[index]->getNormallDisplayName() + " " + lnkdisplay;

    //Remove extra spaces that appear for some reason:
    lnkdisplay.replace(QRegExp("[ ][ ]*"), " ");

    return lnkdisplay;
}

//Opens the bookmark dialog
void MainWindow::bookMarkPosition(QString lnk)
{
    QString linkdisplay = bookMarkTitle(lnk);

    BookMarkTitle *t = new BookMarkTitle(this, lnk, linkdisplay);
    t->show();

    connect (t, SIGNAL(OK(QString,QString)), this, SLOT(addBookMark(QString,QString)));
}

//Adds the given book position to the bookmark list and file
void MainWindow::addBookMark(QString link, QString title)
{
    QString linkdisplay = bookMarkTitle(link);
    if ( linkdisplay == title) title = "";

    addBookMarkToList(link, title);

    //Write to bookmark file
    writetofile(USERPATH + "BookMarkList.txt", link + "|" + title + "\n", "UTF-8", false);
}


//Adds a bookmark to the bookmark list, (giving it's encoded link)
void MainWindow::addBookMarkToList(QString lnk, QString title)
{
    QListWidgetItem *item = new QListWidgetItem(ui->bookmarkWidget);

    QString linkdisplay = bookMarkTitle(lnk);

    if ( title == "")
    {
        item->setText( linkdisplay);
        title = linkdisplay;
    }
    else item->setText( title + " (" + linkdisplay + ")" );

    //This is clearly not the right way to do this, but hey, it works.
    item->setData(Qt::StatusTipRole, lnk);
    item->setData(Qt::WhatsThisRole, title);

    item->setIcon(QIcon(":Icons/bookmarks.png"));
}

//Build the bookmark list from the bookmark file
void MainWindow::buildBookMarkList()
{
    vector <QString> text;
    ReadFileToVector(USERPATH + "BookMarkList.txt", text, "UTF-8");

    for ( int i=0; i<text.size(); i++)
    {
        QString title = "";
        //Split title and link
        int p = text[i].indexOf("|"); //TODO: escape this guy
        if ( p != -1) title = text[i].mid(p+1);
        addBookMarkToList(text[i].mid(0,p), title);
    }
}

//Open the clicked bookmark
void MainWindow::on_bookmarkWidget_itemDoubleClicked(QListWidgetItem* item)
{
    QString lnk = item->data(Qt::StatusTipRole).toString();

    vector<QString> parts;
    splittotwo(lnk, parts, ":");
    int id;
    if(ToNum(parts[0], &id))
    {
        int index = gBookList.FindBookById(id);
        if( index != -1)
        {
            gInternalLocationInHtml = "#" + parts[1];

            //Add a new tab and open the link there
            addViewTab(false);
            ui->viewTab->setTabText(CURRENT_TAB, tr("Orayta"));
            openBook(index);
        }
    }
}

//Opens a dialog allowing to edit the selected bookmark's title
void MainWindow::on_bookmarkEdit_clicked()
{
    int ind = ui->bookmarkWidget->currentIndex().row();

    QString otitle = ui->bookmarkWidget->item(ind)->data(Qt::WhatsThisRole).toString();

    //Open the bookmark title giving dialog with this index and title
    BookMarkTitle *t = new BookMarkTitle(this, ind, otitle);
    t->show();

    //The dialog will call the editBookMarkTitle function when it's done
    connect (t, SIGNAL(RenameOK(int, QString)), this, SLOT(editBookMarkTitle(int, QString)));
}

//Changes the title of the given bookmark (by it's index) to the given one
void MainWindow::editBookMarkTitle(int ind, QString newtitle)
{
    QString link = ui->bookmarkWidget->item(ind)->data(Qt::StatusTipRole).toString();

    QString otitle = ui->bookmarkWidget->item(ind)->data(Qt::WhatsThisRole).toString();
    if (otitle == bookMarkTitle(link)) otitle = "";


    //Edit the item title data (it's saved here...) to the new one
    ui->bookmarkWidget->item(ind)->setData(Qt::WhatsThisRole, newtitle);

    //Update file:
    vector <QString> text;
    ReadFileToVector(USERPATH + "BookMarkList.txt", text, "UTF-8");

    QString str = "";
    for (int i=0; i<text.size(); i++)
    {
        if ( i != ind) str += text[i] + "\n";
        else str += link + "|" + newtitle + "\n";
    }

    writetofile(USERPATH + "BookMarkList.txt", str, "UTF-8", true);

    //Uodate the new title in the list
    if (newtitle != "")
    {
        //Replace the titles
        if (otitle != "") ui->bookmarkWidget->item(ind)->setText( ui->bookmarkWidget->item(ind)->text().replace(otitle, newtitle) );
        //If there was an empty title before, add the newtitle and "("-s
        else ui->bookmarkWidget->item(ind)->setText( newtitle + " (" + bookMarkTitle(link) + ")");
    }
    //Empty title given
    else
    {
        //Remove the title and "("-s
        if (otitle != "") ui->bookmarkWidget->item(ind)->setText( ui->bookmarkWidget->item(ind)->text().replace(otitle + " (", "").replace(QRegExp("[)]$"), ""));
    }
}


//Remove the selected bookmark from the list and file
void MainWindow::on_removeBookmark_clicked()
{
    int ind = ui->bookmarkWidget->currentIndex().row();

    vector <QString> text;
    ReadFileToVector(USERPATH + "BookMarkList.txt", text, "UTF-8");

    QString str = "";
    for (int i=0; i<text.size(); i++)
    {
        if ( i != ind) str += text[i] + "\n";
    }

    writetofile(USERPATH + "BookMarkList.txt", str, "UTF-8", true);

    ui->bookmarkWidget->takeItem( ind );
}


//Move the selected bookmark up in the list by one position
void MainWindow::on_bookmarkUp_clicked()
{
    int ind = ui->bookmarkWidget->currentIndex().row();

    if (ind > 0)
    {
        vector <QString> text;
        ReadFileToVector(USERPATH + "BookMarkList.txt", text, "UTF-8");

        swap(text[ind], text[ind-1]);

        QString str = "";
        for (int i=0; i<text.size(); i++) str += text[i] + "\n";

        writetofile(USERPATH + "BookMarkList.txt", str, "UTF-8", true);

        //Swap the list elements:
        QListWidgetItem *tmp = ui->bookmarkWidget->currentItem();
        ui->bookmarkWidget->takeItem( ind );
        ui->bookmarkWidget->insertItem( ind - 1 ,tmp);
        ui->bookmarkWidget->setCurrentRow( ind - 1 );
    }
}

//Move the selected bookmark down in the list by one position
void MainWindow::on_bookmarkDown_clicked()
{
    int ind = ui->bookmarkWidget->currentIndex().row();

    if (ind < ui->bookmarkWidget->count() - 1 )
    {
        vector <QString> text;
        ReadFileToVector(USERPATH + "BookMarkList.txt", text, "UTF-8");

        swap(text[ind], text[ind+1]);

        QString str = "";
        for (int i=0; i<text.size(); i++) str += text[i] + "\n";

        writetofile(USERPATH + "BookMarkList.txt", str, "UTF-8", true);

        //Swap the list elements:
        QListWidgetItem *tmp = ui->bookmarkWidget->currentItem();
        ui->bookmarkWidget->takeItem( ind );
        ui->bookmarkWidget->insertItem( ind + 1 ,tmp);
        ui->bookmarkWidget->setCurrentRow( ind + 1 );
    }
}

void MainWindow::menuBookMark()
{
    QString link = gWebViewList[CURRENT_TAB]->activeLink().replace("$","");

    //Find book's id and add it to the link
    int id = gWebViewList[CURRENT_TAB]->book()->getUniqueId();
    link = stringify(id) + ":" + link;

    bookMarkPosition( link );
}





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
        for (int i=0; i<comment_titles.size(); i++)
        {
            str += comment_titles[i] + "\n" + comment_texts[i] + "\n";
        }
        writetofile(USERPATH + "CommentList.txt", str, "UTF-8", true);
    }

    //In any case, add the new comment (if it isn't empty) to the file
    if ( comment != "") writetofile(USERPATH + "CommentList.txt", link + "\n" + text + "\n", "UTF-8", false);

    //Force the webview to clear [seems to be needed only for empty comments]
    if ( comment == "") gWebViewList[CURRENT_TAB]->setHtml("");

    //Re-load the page at the comment's position
    // NOTE: this isn't perfect, but I have nothing better
    int p = link.indexOf(":");
    int uid;
    ToNum(link.mid(0,p), &uid);
    gInternalLocationInHtml = "#" + link.mid(p+1);

    int id = gBookList.FindBookById(uid);
    openBook(id);

}

void MainWindow::removeComment(QString link)
{
    //Adding an emtpy comment removes the existing one
    addCommentAtPosition(link, "");
}

void MainWindow::menuComment()
{
    QString link = gWebViewList[CURRENT_TAB]->activeLink().replace("$","");

    //Find book's id and add it to the link
    int id = gWebViewList[CURRENT_TAB]->book()->getUniqueId();
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
    gWebViewList[CURRENT_TAB]->ShowNikud = showNikud;
    LoadBook( gWebViewList[CURRENT_TAB]->book() );
}

void MainWindow::toggleTeamim(bool showTeamim)
{
    gWebViewList[CURRENT_TAB]->ShowTeamim = showTeamim;
    LoadBook( gWebViewList[CURRENT_TAB]->book() );
}

void MainWindow::on_viewTab_currentChanged(int index)
{
    //Show / hide menu options depending on the book:
    if ( gWebViewList.size() > index)
    {
        if ( gWebViewList[index]->book() != NULL )
        {
            //Show / Hide nikud and teamim buttons
            ui->showNikudAction->setVisible( gWebViewList[index]->book()->hasNikud );
            ui->showTeamimAction->setVisible( gWebViewList[index]->book()->hasTeamim );
        }

        //Disable / Enable them by what's displayed
        ui->showNikudAction->setChecked( gWebViewList[index]->ShowNikud );
        ui->showTeamimAction->setChecked( gWebViewList[index]->ShowTeamim );
    }

}

void MainWindow::menuErrorReport()
{
    QString link = gWebViewList[CURRENT_TAB]->activeLink().replace("$","");

    if (link != "")
    {
        int id = gWebViewList[CURRENT_TAB]->book()->getUniqueId();
        int index = gBookList.FindBookById(id);

        errorReport *err = new errorReport( this, link, gBookList[index]->getNormallDisplayName() );
        err->show();
    }
}


void MainWindow::on_treeWidget_clicked(QModelIndex index)
{
    QTreeWidgetItem * current = ui->treeWidget->currentItem();

    int bookid = gBookList.FindBookByTWI(current);

    if (current->checkState(0) == Qt::Checked) addToSearch();
    if (current->checkState(0) == Qt::Unchecked) removeFromSearch();

    if ( gBookList[bookid]->mIconState == BLUE )
    {
        ui->addToSearchAction->setEnabled(false);
        ui->removeFromSearchAction->setEnabled(true);
    }
    else if ( gBookList[bookid]->mIconState == GREY )
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

QList <QCheckBox *> weavedList;

void MainWindow::treeWidgetSelectionChanged(QTreeWidgetItem* current, QTreeWidgetItem* old)
{
    int bookid = gBookList.FindBookByTWI(current);

    //Show / Hide mixed display stuff
    if (gBookList[bookid]->isMixed())
    {
        ui->mixedGroup->show();

        //Clear old entries
        for (int i=0; i<weavedList.size(); i++)
        {
            weavedList[i]->hide();
        }
        weavedList.clear();

        //Create new entries
        bool anychecked = false;
        for(int i=gBookList[bookid]->mWeavedSources.size()-1; i>0; i--)
        {
            QCheckBox *chk = new QCheckBox(gBookList[bookid]->mWeavedSources[i].Title, ui->mixedFrame);
            ui->verticalLayout_11->addWidget(chk);

            if (gBookList[bookid]->mWeavedSources[i].show == true)
            {
                chk->setChecked(true);
                anychecked = true;
            }

            chk->setWhatsThis(stringify(gBookList[bookid]->mWeavedSources[i].id));

            weavedList << chk;

            QSignalMapper *signalMapper = new QSignalMapper(this);

            connect(chk, SIGNAL(clicked()), signalMapper, SLOT(map()));
            signalMapper->setMapping(chk, weavedList.size() - 1);
            connect(signalMapper, SIGNAL(mapped(int)), this, SLOT(weavedCheckBoxClicked(int)));
        }
        if (!anychecked) ui->aloneCheckBox->setChecked(true);

    }
    else ui->mixedGroup->hide();
}

void MainWindow::weavedCheckBoxClicked(int btnIndex)
{
    //Still a bit ugly

    QTreeWidgetItem* current = ui->treeWidget->currentItem();
    int bookid = gBookList.FindBookByTWI(current);


    QString id = weavedList[btnIndex]->whatsThis();

    for(int i=1; i<gBookList[bookid]->mWeavedSources.size(); i++)
    {
        if (stringify(gBookList[bookid]->mWeavedSources[i].id) == id)
        {
            gBookList[bookid]->mWeavedSources[i].show = weavedList[btnIndex]->checkState();
        }
    }
}

void MainWindow::on_saveConf_clicked()
{
    gFontFamily = ui->fontComboBox->currentFont().family();
    gFontSize = ui->fonSizeSpinBox->value();

    ui->saveConf->setEnabled(false);
}

void MainWindow::on_fonSizeSpinBox_valueChanged(int val)
{
    ui->saveConf->setEnabled(true);

    ui->fontPreview->setFont(QFont(ui->fontComboBox->currentFont().family(), ui->fonSizeSpinBox->value()));
}

void MainWindow::on_fontComboBox_currentIndexChanged(QString f)
{
    ui->saveConf->setEnabled(true);

    ui->fontPreview->setFont(QFont(f, ui->fonSizeSpinBox->value()));
}

void MainWindow::findBookForm()
{
    bookfind * bf = new bookfind(this, gBookList);
    bf->show();

    connect(bf, SIGNAL(openBook(int)), this, SLOT(menuOpenBook(int)));
}

void MainWindow::menuOpenBook(int uid)
{
    int index = gBookList.FindBookById(uid);

    if (index != -1)
    {
        addViewTab(false);
        openBook(index);
    }

}


void MainWindow::on_viewTab_tabCloseRequested(int index)
{
    if (ui->viewTab->count() > 1)
    {
        //Destroy tab
        ui->viewTab->removeTab(index);
        //Erase it's web view and hbox
        gWebViewList.erase(gWebViewList.begin() + index);
        gVboxList.erase(gVboxList.begin() + index);
    }
    else
    {
        //Yes, this is an ugly hack. It just wouldn't clear without it...

        //Erase the web view and hbox
        gWebViewList.erase(gWebViewList.begin() + index);
        gVboxList.erase(gVboxList.begin() + index);

        //Rebuild them:
        //Create new webview
#ifdef KHTML
        KHTMLWidget *newview = new KHTMLWidget();
#else
        myWebView *newview = new myWebView();
#endif
        gWebViewList.push_back(newview);

        //Connect the signalls sent from the new widgets to their slots
        QObject::connect(newview, SIGNAL(loadFinished(bool)), this , SLOT(webView_loadFinished(bool)));
        QObject::connect(newview, SIGNAL(linkClicked(QUrl)), this , SLOT(webView_linkClicked(QUrl)));
        QObject::connect(newview, SIGNAL(loadStarted()), this , SLOT(webView_loadStarted()));
        QObject::connect(newview, SIGNAL(loadProgress (int)), this , SLOT(webView_loadProgress(int)));

#ifndef KHTML
        newview->setContextMenuPolicy(Qt::PreventContextMenu);
#endif
        //Set all externall links to be handeled by the LickClick signal instead of automatically
        //  (All links I care about are marked as externall)
#ifndef KHTML
        newview->page()->setLinkDelegationPolicy(QWebPage::DelegateExternalLinks);
#endif
        delete ui->viewTab->widget(CURRENT_TAB)->layout();
        QVBoxLayout *vbox = new QVBoxLayout(ui->viewTab->widget(index));
        gVboxList.push_back(vbox);
#ifdef KHTML
        vbox->addWidget(newview->getQWidget());
#else
        vbox->addWidget(newview);
#endif

        vbox->setMargin(0);
        newview->setHtml("<html></html>");
    }

    //Update the stupid tab widget
    on_viewTab_currentChanged(index);
}

void MainWindow::on_changeLangButton_clicked()
{
    QSettings settings("Orayta", "SingleUser");
    settings.beginGroup("Confs");
    int i = gLangsDisplay.indexOf(ui->langComboBox->currentText());
    if (i != -1) settings.setValue("lang", gLangs[i]);
    settings.endGroup();

    QApplication::processEvents();

    //Reboot the program
    QProcess::startDetached("\"" + QApplication::applicationFilePath() + "\"");
    exit(0);
}

void MainWindow::on_aloneCheckBox_toggled(bool checked)
{
    ui->mixedFrame->setEnabled(!checked);
}

void MainWindow::on_openMixed_clicked()
{
    int ind = gBookList.FindBookByTWI(ui->treeWidget->currentItem());
    if (ind != -1) openBook(ind);
}
