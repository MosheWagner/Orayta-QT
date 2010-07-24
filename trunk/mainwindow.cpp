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

//TODO: Finish GUI changes
//TODO: Build search DB in the backgorund?

//TODO: BUG: Mark string fails! (in all books, but differently in gmarot)
//TODO: Program slows down after search. Is the search fully stoped?

/*
  Roadmap for 0.03:

  - Put the settings tab in a form instead

  - Put search-in-books in a tab, and leave on top a button allowing to search in current book (like firefox?)

  - Improve all search stuff (including better result page. using side bar?)
  
  - Solve all book issues (wierd nikud, a few books that render incorrectly, better book names in tree)

  - Improve GUI. (Remove search bar? Change in and out of search icons)

  - Add import books menu, including a "get from hebrewbooks" option
  
  - PDF support
*/

//TODO: FIXME: KHTML search starts from begining when focus lost

//TODO: BUG!!! When קרי וכתיב, none are found by searches. But both should...

//TODO: Improve current position detection

//TODO: Allow adding custom html books through the program

//TODO: dictionary

//TODO: allow search to be stopped or canceled

//TODO: Fix the way ספר המידות looks (compare to the original TE)

//BUG: תומר דבורה Crashes!!!
//BUG: בראשית Crashes!!

//TODO: Translatable welcome page
//(TODO: create help page for linux version)
//TODO: hide welcomepage from search

//TODO: Speed up search. (Better progress bar too?)

//TODO: Add rav kook books

//TODO: Remove wierd nikud

//TODO: see if printing dialog really appears in hebrew on hebrew locals

//TODO: document & clean code

//TODO: more of the search - two word search, etc'

//TODO: Requested Feature: parallel tab (like in the originall)

//TODO: see what else to do ;-)




// This is just a very simple define. every place in the code,
//  "CURRENT_TAB" simply represents "ui->viewTab->currentIndex()".
//
//  It just made the code much more readable, I couldn't resist
#define CURRENT_TAB ui->viewTab->currentIndex()

//Another define becuase I'm lasy. "CurrentBook" simply represents
// the currently visible "bookdisplayer"
#define CurrentBook bookDisplayerList[CURRENT_TAB]



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
    if (LANG == "Hebrew") toRTL();

    //Force the viewTab to take 80 percent of the window
    ui->verticalLayout_12->setStretchFactor(ui->viewTab, 80);
    ui->verticalLayout_12->setStretchFactor(ui->verticalLayout_9, 20);

    //Close the default tab:
    ui->viewTab->removeTab(0);
    //Now add a good one:
    addViewTab(false);
    //Set window title:
    setWindowTitle(tr("Orayta"));

    //Generate the BookList(s), and put it in the tree
    bookList.BuildFromFolder(BOOKPATH);

    if (bookList.size() == 0)
    {
        QMessageBox msgBox;
        msgBox.setText(tr("No books found! \nCheck your installation, or contact the developer."));
        msgBox.exec();

        //No books found
        exit(2);
    }

    //Insert all the books into the booktree
    BuildBookTree();

    //Load mixed display confs
    restoreBookConfs();

    //Sort the book tree by the second column ( that holds the whole name, including the number before it)
    ui->treeWidget->setColumnCount(2);  //Small hack to allow sorting the right way...
    ui->treeWidget->sortItems(1, Qt::AscendingOrder);
    ui->treeWidget->setColumnCount(1);

    //Load start page. Assuming it's book[0] of course.
    if (!bookList[0]->IsDir())
    {
        LoadBook(bookList[0]);
    }
    else
    {
        bookDisplayerList[0]->setHtml(simpleHtmlPage(tr("Orayta"), tr("Jewish books")));
        ui->viewTab->setTabText(CURRENT_TAB, tr("Orayta"));
    }

    ui->btnBox->addButton((QAbstractButton *)ui->newTabButton, QDialogButtonBox::ActionRole);
    ui->btnBox->addButton((QAbstractButton *)ui->topButton, QDialogButtonBox::ActionRole);
    ui->btnBox->addButton((QAbstractButton *)ui->zoominButton, QDialogButtonBox::ActionRole);
    ui->btnBox->addButton((QAbstractButton *)ui->zoomoutButton, QDialogButtonBox::ActionRole);
    ui->btnBox->addButton((QAbstractButton *)ui->showSearchBarButton, QDialogButtonBox::ActionRole);

    ui->searchGroupBX->hide();
    ui->showSearchBarButton->setChecked(false);

    ui->progressBar->hide();

    ui->mixedGroup->hide();

    ui->viewTab->setCornerWidget(ui->btnBox, Qt::TopRightCorner);

    ui->treeWidget->setHeaderHidden(true);
    //Hack for windows:
    ui->treeWidget->setColumnWidth(0,1000);

    //Load the bookmarks into thier list:
    buildBookMarkList();

    connectMenuActions();

    //Set available languages
    langs << "Hebrew" << "English" << "French";
    langsDisplay << "עברית" << "English" << "Français";;

    //Show available languages in the language combobox
    for (int i=0; i<langs.size(); i++)
    {
        ui->langComboBox->addItem(langsDisplay[i]);
    }

    //Show current language
    int is = -1;
    for (int i=0; i<langs.size(); i++) if (LANG == langs[i]) is = i;
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

    ui->progressBar->setLayoutDirection(Qt::RightToLeft);

    ui->searchGroupBX->setLayoutDirection(Qt::RightToLeft);

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

    connect(ui->advancedSearchAction, SIGNAL(triggered()), this, SLOT(showSearchTab()));
    connect(ui->searchForwardAction, SIGNAL(triggered()), this, SLOT(on_searchForward_clicked()));
    connect(ui->searchBackwardsAction, SIGNAL(triggered()), this, SLOT(on_searchBackwords_clicked()));

    connect(ui->addAllToSearchAction, SIGNAL(triggered()), this, SLOT(on_addAllToSearchButton_clicked()));
    connect(ui->removeAllFromSearchAction, SIGNAL(triggered()), this, SLOT(on_removeAllFromSearchButton_clicked()));
    connect(ui->addToSearchAction, SIGNAL(triggered()), this, SLOT(addToSearch()));
    connect(ui->removeFromSearchAction, SIGNAL(triggered()), this, SLOT(removeFromSearch()));

}

MainWindow::~MainWindow()
{
    ClearTmp();

    //TODO: Don't I need to free all the items?
    for (int i=0; i<bookDisplayerList.size(); i++) { delete bookDisplayerList[i]; }
    bookDisplayerList.clear();

    delete ui;
}

//Remove all temporary html files the program created
void MainWindow::ClearTmp()
{
    QDir dir(TMPPATH);
    QStringList list = dir.entryList(QStringList("*.html"));

    QFile f;
    for (int i=0; i<list.size(); i++)
    {
        f.remove(dir.absoluteFilePath(list[i]));
    }
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


    //Save books' settings
    for(int i=0; i<bookList.size(); i++)
    {
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


    QFont f (gFontFamily, gFontSize);
    ui->fontPreview->setFont(f);
    ui->fontComboBox->setCurrentFont(f);
    ui->fonSizeSpinBox->setValue(gFontSize);
    settings.endGroup();
}

void MainWindow::restoreBookConfs()
{
    QSettings settings("Orayta", "SingleUser");

    //Load books settings
    for(int i=0; i<bookList.size(); i++)
    {
        settings.beginGroup("Book" + stringify(bookList[i]->getUniqueId()));
            int n = settings.value("MixedDisplayes", 0).toInt();
            bookList[i]->showAlone = settings.value("ShowAlone", false).toBool();
            for (int j=0; j<n; j++)
            {
                bookList[i]->mWeavedSources[j].show = settings.value("Shown" + stringify(j), true).toBool();
            }
            bool inSearch = settings.value("InSearch", false).toBool();
            if (inSearch) bookList[i]->select();
            else bookList[i]->unselect();
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

            //Just for the sorting...
            twi->setText(1, bookList[i]->getName());


            //Show a checkbox next to the item
            twi->setCheckState(0,Qt::Checked);


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
            dn = dn.replace("שס", "ש\"ס");
            twi->setText(0, dn);

            //set the icon:
            QIcon *icon = bookIcon(bookList[i]->IsDir(), bookList[i]->isMixed(), bookList[i]->mIconState);

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
    bool shownikud = CurrentBook->isNikudShown();
    bool showteamim = CurrentBook->areTeamimShown();

    ui->viewTab->setTabText(CURRENT_TAB, tr("Loading..."));

    //Generate filename representing this file and commentreis that should open
    //  This way the file is rendered again only if it needs to be shown differently (if other commenteries were requested)
    QString htmlfilename = book->HTMLFileName();

    //Check if file already exists. If not, make sure it renders ok.
    QFile f(htmlfilename);
    bool renderedOK = true;
    if (!f.exists()) renderedOK = book->htmlrender(htmlfilename, shownikud, showteamim, markString);

    if (renderedOK == true)
    {
        QString p =  absPath(htmlfilename);

        CurrentBook->load(QUrl(p));

        CurrentBook->setBook(book);
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

    //Show / Hide nikud and teamim buttons
    ui->showNikudAction->setVisible( book->hasNikud );
    ui->showTeamimAction->setVisible( book->hasTeamim );
}

//Called when a TreeItem is double clicked
void MainWindow::on_treeWidget_itemDoubleClicked(QTreeWidgetItem* item, int column)
{
    int ind = bookList.FindBookByTWI(item);

    //Open the selected book
    openBook(ind);
}

//Open the given book (by it's id in the booklist)
void MainWindow::openBook( int ind )
{
    if (ind >= 0 && ind < bookList.size())
    {
        if(bookList[ind]->IsDir() == false)
        {
            //If the filename ends with ".html", load it as is, without rendering
            if((bookList[ind]->getPath().endsWith(".html")) || (bookList[ind]->getPath().endsWith(".htm")))
            {
                CurrentBook->load( QUrl( bookList[ind]->getPath()));
            }
            else
            {
                LoadBook(bookList[ind]);
            }
        }
    }
}

//Calls "openBook", but uses the currently selected book in the tree
void MainWindow::openSelectedBook( )
{
    int ind = bookList.FindBookByTWI(ui->treeWidget->selectedItems()[0]);

    openBook(ind);
}

//Opens a new tab and Calls "openBook" (using the currently selected book in the tree).
void MainWindow::openSelectedBookInNewTab()
{
    addViewTab(false);

    int ind = bookList.FindBookByTWI(ui->treeWidget->selectedItems()[0]);
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
    bookDisplayer * bd = new bookDisplayer(this, ui->viewTab);
    bookDisplayerList << bd;
    ui->viewTab->addTab(bd, tr("Orayta"));

    if (empty == true) bd->setHtml(simpleHtmlPage(tr("Orayta"), ""));

    ui->viewTab->setTabText(CURRENT_TAB, tr("Orayta"));

    //Switch to the new tab
    ui->viewTab->setCurrentIndex(ui->viewTab->count()-1);

    connect(bd, SIGNAL(externalLink(QString)), this, SLOT(openExternalLink(QString)));
}

void MainWindow::on_lineEdit_returnPressed()
{
    on_searchForward_clicked();
}


void MainWindow::addToSearch()
{
    int ind = bookList.FindBookByTWI(ui->treeWidget->currentItem());
    if (ind != -1)
        bookList[ind]->select();

    ui->addToSearchAction->setEnabled(false);
    ui->removeFromSearchAction->setEnabled(true);
}

void MainWindow::on_addAllToSearchButton_clicked()
{
    for(unsigned int i=0; i<bookList.size(); i++)
    {
        bookList[i]->select();
    }

    ui->addToSearchAction->setEnabled(false);
    ui->removeFromSearchAction->setEnabled(true);
}

void MainWindow::removeFromSearch()
{
    int ind = bookList.FindBookByTWI(ui->treeWidget->currentItem());
    if (ind != -1)
        bookList[ind]->unselect();

    ui->addToSearchAction->setEnabled(true);
    ui->removeFromSearchAction->setEnabled(false);
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
    CurrentBook->ZoomIn();
}

void MainWindow::on_zoomoutButton_clicked()
{
    CurrentBook->ZoomOut();
}


void MainWindow::on_topButton_clicked()
{
    //Jump to the top of the page using javascript
    CurrentBook->execScript("window.scrollTo(0, 0)");
}

void MainWindow::on_SearchInBooksBTN_clicked()
{
    //TODO: כתיב מלא / חסר

    QString otxt = ui->searchInBooksLine->text();
    QString stxt = otxt;

    //Search as regexp entered
    if (ui->regexpCheckBox->isChecked())
    {
        //Prevent greedy searches by mistake
        QRegExp regexp(stxt);
        regexp.setMinimal(true);
        SearchInBooks(regexp, tr("RegExp: ") + otxt);
    }
    else
    {
        //Find any of the words
        if ( ui->radioButton->isChecked() )
        {
            QString pat = "";

            if (ui->fuzzyCheckBox->isChecked()) stxt = AllowKtivMaleh(stxt);

            QStringList words = stxt.split(" ");
            for (int i=0; i<words.size(); i++)
            {
                if (words[i] != "")
                {
                    //See if to force full words
                    // (This is OK because the search DB has forced spaces at it's beginning and end too)
                    if ( ui->fullCheckBox->isChecked()) pat += " " + words[i] + " ";
                    else pat += words[i]; //Part of word will be found too
                }
                if ( i != words.size()-1 ) pat+= "|";
            }
            SearchInBooks(QRegExp(pat), otxt);
        }
        //Search for all of the words
        else
        {
            if ( ui->fullCheckBox->isChecked()) stxt = " " + otxt + " ";

            if (ui->fuzzyCheckBox->isChecked()) stxt = AllowKtivMaleh(stxt);

            SearchInBooks(QRegExp(stxt), otxt);
        }
    }
}


#define RESULTS_MAX 200
void MainWindow::SearchInBooks (QRegExp regexp, QString disp)
{
    //TODO: make preview look nice

    QString title, Html="",Htmlhead="", HtmlTopLinks="";

    int results = 0;
    QList <QString> text;

    if ( regexp.pattern() != "" )
    {
        CurrentBook->ShowWaitPage();

        ui->progressBar->show();
        ui->progressBar->setValue(5);
        float percentPerBook = (90.0 / bookList.size());

        //Make a new tab if the current tab has something in it
        if (ui->viewTab->tabText(CURRENT_TAB).indexOf(tr("Orayta")) == -1)
        {
            addViewTab(false);
            CurrentBook->normalZoom();
            CurrentBook->setHtml(simpleHtmlPage(tr("Orayta"), ""));
            ui->viewTab->setTabText(CURRENT_TAB, (tr("Orayta")));
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

        int results = 0;
        for (int i=0; i<bookList.size() && (results <= RESULTS_MAX); i++)
        {
            ui->progressBar->setValue( 5 + (i + 1) * percentPerBook ) ;

            if (!bookList[i]->IsDir() && bookList[i]->IsInSearch())
            {
                QList <SerachResult> searchResults = bookList[i]->findInBook(regexp);

                //Let the animation move...
                QApplication::processEvents();

                for (int j=0; j<searchResults.size(); j++)
                {
                    results ++;
                    //Get the text best to show for this reult's description
                    QString linkdisplay = "";

                    //Regular books:
                    if (!bookList[i]->getPath().contains("תלמוד") && !bookList[i]->getPath().contains("שס"))
                    {

                        //This is an overkill, but I couldn't resist:
                        //linkdisplay += BookSearchDisplay (gBookList[i]->getNormallDisplayName() ,itr.toHumanString());

                        linkdisplay += bookList[i]->getNormallDisplayName() + " " + searchResults[j].itr.toHumanString();
                    }
                    //Gmarot:
                    else
                    {
                        linkdisplay += bookList[i]->getNormallDisplayName() + " ";
                        linkdisplay += searchResults[j].itr.toGmaraString();
                    }


                    //Add a small link (at the index) to the full result
                    HtmlTopLinks += reddot() + "&nbsp&nbsp&nbsp<a href=\"#" + stringify(results) + "\">" + linkdisplay  + "</a><BR>";

                    //Add the full result to the page
                    Html += "<span style=\"font-size:23px\">";
                    Html += "<a name=\"" + stringify(results) + "\"></a>";

                    Html += "<a href=!" + stringify(bookList[i]->getUniqueId()) + ":";
                    Html += searchResults[j].itr.toStringForLinks();
                    Html += ":" + escapeToBase32(regexp.pattern()) + ">";

                    Html += linkdisplay;
                    Html += "</a><BR></span>\n";

                    //Show result
                    Html += searchResults[j].preview;

                    Html += "<br><br><br>\n";
                }

            }
        }
        if(results == 0)
        {
            //TODO: write better explenation
            Htmlhead +="<BR><BR>"; Htmlhead += tr("No search results found:"); Htmlhead += "\"" + disp + "\"";
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

        //TODO: Serch results are special books
        writetofile(TMPPATH + "Search" + ".html", Html, "UTF-8");

        CurrentBook->HideWaitPage();
        CurrentBook->load(QUrl(TMPPATH + "Search" + ".html"));
    }
}

QString lastSearch = "";

void MainWindow::on_searchForward_clicked()
{
    if (ui->lineEdit->text().replace(" ","") == "") return;

    QRegExp regexp = withNikudAndTeamim(ui->lineEdit->text());

    //BEWARE: Amaizingly ugly hack! Watch out!

    //Mark current cursor position using JS, grab the Html, and hide the marking
    CurrentBook->execScript("markCursorPos();");

    QString text = CurrentBook->htmlSource();

    int ppp = text.indexOf("**|*|**");

    //Hide the marking in the html
    CurrentBook->execScript("unMarkCursorPos();");

    //Search only in the text after the cursor's position
    if (ppp != -1) text = text.mid(ppp + 7 + 1);

    //Find the closest occurence of the RegExp
    QString next = "";

    int pos = regexp.indexIn(text);
    if (pos > -1)
    {
        next = regexp.cap(0);
    }

    CurrentBook->searchText(next, false);
}

void MainWindow::on_searchBackwords_clicked()
{
    if (ui->lineEdit->text().replace(" ","") == "") return;

    //BEWARE: Amaizingly ugly hack! Watch out!

    //Mark current cursor position using JS, grab the Html, and hide the marking
    CurrentBook->execScript("markCursorPos();");

    QString text = CurrentBook->htmlSource();


    int ppp = text.indexOf("**|*|**");
    int ppb = text.indexOf("</SCRIPT");

    //Hide the marking in the html
    CurrentBook->execScript("unMarkCursorPos();");

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

    CurrentBook->searchText(last, true);
}

void MainWindow::on_treeWidget_customContextMenuRequested(QPoint pos)
{
    QTreeWidgetItem * item = 0 ;
    item = ui->treeWidget->itemAt(pos);

    int ind = -1;
    if ( item != 0 )
    {
        ind = bookList.FindBookByTWI(item);
    }

    if( ind != -1)
    {
        if ( bookList[ind]->IsDir() == false)
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
            int ind = bookList.FindBookByTWI(ui->treeWidget->selectedItems()[0]);
            if (ind != -1)
            {
                if ( bookList[ind]->IsDir() )
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
    //Ctrl
    if (keyEvent->modifiers() == Qt::CTRL)
    {
        //If this key is "F", show "search in books" bar
        if ( keyEvent->key() == Qt::Key_F)
        {
            ui->showSearchBarButton->click();
        }
    }
}

/*
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
        CurrentBook->print(&printer);
    }
#else
    CurrentBook->view()->print();
#endif
}
*/

QString MainWindow::bookMarkTitle(QString lnk)
{
    //Format position for human display
    int p = lnk.indexOf(":");
    QString lnkdisplay = lnk.mid(p+1);
    lnkdisplay = unescapeFromBase32(lnkdisplay).replace("_", " ").replace("-", " ").replace("{", "").replace("}", "").replace(" 0", "");

    int id;
    ToNum(lnk.mid(0,p), &id);

    //Find and add the book's name
    int index = bookList.FindBookById(id);
    if (index == -1)
    {
        print ("Invalid link!");
        return "";
    }
    else lnkdisplay = bookList[index]->getNormallDisplayName() + " " + lnkdisplay;

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
    QList <QString> text;
    ReadFileToList(USERPATH + "BookMarkList.txt", text, "UTF-8");

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
        int index = bookList.FindBookById(id);
        if( index != -1)
        {
            //Add a new tab and open the link there
            addViewTab(false);
            ui->viewTab->setTabText(CURRENT_TAB, tr("Orayta"));

            CurrentBook->setInternalLocation("#" + parts[1]);
            openBook(index);
        }
    }
}

//Opens a dialog allowing to edit the selected bookmark's title
void MainWindow::on_bookmarkEdit_clicked()
{
    if (ui->bookmarkWidget->currentIndex().isValid())
    {
        int ind = ui->bookmarkWidget->currentIndex().row();

        QString otitle = ui->bookmarkWidget->item(ind)->data(Qt::WhatsThisRole).toString();

        //Open the bookmark title giving dialog with this index and title
        BookMarkTitle *t = new BookMarkTitle(this, ind, otitle);
        t->show();

        //The dialog will call the editBookMarkTitle function when it's done
        connect (t, SIGNAL(RenameOK(int, QString)), this, SLOT(editBookMarkTitle(int, QString)));
    }
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
    QList <QString> text;
    ReadFileToList(USERPATH + "BookMarkList.txt", text, "UTF-8");

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

    QList <QString> text;
    ReadFileToList(USERPATH + "BookMarkList.txt", text, "UTF-8");

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
        QList <QString> text;
        ReadFileToList(USERPATH + "BookMarkList.txt", text, "UTF-8");

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
        QList <QString> text;
        ReadFileToList(USERPATH + "BookMarkList.txt", text, "UTF-8");

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
    QString link = CurrentBook->activeLink().replace("$","");

    //Find book's id and add it to the link
    int id = CurrentBook->book()->getUniqueId();
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

    //Re-load the page at the comment's position
    // NOTE: this isn't perfect, but I have nothing better
    int p = link.indexOf(":");
    int uid;
    ToNum(link.mid(0,p), &uid);
    CurrentBook->setInternalLocation("#" + link.mid(p+1));

    int id = bookList.FindBookById(uid);

    //Force a new render of the book
    bookList[id]->htmlrender(bookList[id]->HTMLFileName(), CurrentBook->isNikudShown(), CurrentBook->areTeamimShown(), "");

    //Force the webview to clear [seems to be needed only for empty comments]
    CurrentBook->setHtml("");

    openBook(id);
}

void MainWindow::removeComment(QString link)
{
    //Adding an emtpy comment removes the existing one
    addCommentAtPosition(link, "");
}

void MainWindow::menuComment()
{
    QString link = CurrentBook->activeLink().replace("$","");

    //Find book's id and add it to the link
    int id = CurrentBook->book()->getUniqueId();
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
    CurrentBook->showNikud(showNikud);
    LoadBook( CurrentBook->book() );
}

void MainWindow::toggleTeamim(bool showTeamim)
{
    CurrentBook->showTeamim(showTeamim);
    LoadBook( CurrentBook->book() );
}

void MainWindow::on_viewTab_currentChanged(int index)
{
    if (index != -1)
    {
        //Show / hide menu options depending on the book:
        if ( bookDisplayerList.size() > index && bookDisplayerList.size() > 0)
        {
            if ( bookDisplayerList[index]->book() != NULL )
            {
                //Show / Hide nikud and teamim buttons
                ui->showNikudAction->setVisible( bookDisplayerList[index]->book()->hasNikud );
                ui->showTeamimAction->setVisible( bookDisplayerList[index]->book()->hasTeamim );
            }

            //Disable / Enable them by what's displayed
            ui->showNikudAction->setChecked( bookDisplayerList[index]->isNikudShown() );
            ui->showTeamimAction->setChecked( bookDisplayerList[index]->areTeamimShown() );
        }
    }
}

void MainWindow::menuErrorReport()
{
    QString link = CurrentBook->activeLink().replace("$","");

    if (link != "")
    {
        int id = CurrentBook->book()->getUniqueId();
        int index = bookList.FindBookById(id);

        errorReport *err = new errorReport( this, link, bookList[index]->getNormallDisplayName() );
        err->show();
    }
}


void MainWindow::on_treeWidget_clicked(QModelIndex index)
{
    QTreeWidgetItem * current = ui->treeWidget->currentItem();

    int bookid = bookList.FindBookByTWI(current);

    if (current->checkState(0) == Qt::Checked) addToSearch();
    if (current->checkState(0) == Qt::Unchecked) removeFromSearch();

    if ( bookList[bookid]->mIconState == BLUE )
    {
        ui->addToSearchAction->setEnabled(false);
        ui->removeFromSearchAction->setEnabled(true);
    }
    else if ( bookList[bookid]->mIconState == GREY )
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
    int bookid = bookList.FindBookByTWI(current);

    //Show / Hide mixed display stuff
    if (bookList[bookid]->isMixed())
    {
        ui->mixedGroup->show();

        //Clear old entries
        for (int i=0; i<weavedList.size(); i++)
        {
            weavedList[i]->hide();
        }
        weavedList.clear();

        //Create new entries
        for(int i=1; i<bookList[bookid]->mWeavedSources.size(); i++)
        {
            QCheckBox *chk = new QCheckBox(bookList[bookid]->mWeavedSources[i].Title, ui->mixedFrame);
            ui->verticalLayout_11->addWidget(chk);

            if (bookList[bookid]->mWeavedSources[i].show == true)
            {
                chk->setChecked(true);
            }

            chk->setWhatsThis(stringify(bookList[bookid]->mWeavedSources[i].id));

            weavedList << chk;

            QSignalMapper *signalMapper = new QSignalMapper(this);

            connect(chk, SIGNAL(clicked()), signalMapper, SLOT(map()));
            signalMapper->setMapping(chk, weavedList.size() - 1);
            connect(signalMapper, SIGNAL(mapped(int)), this, SLOT(weavedCheckBoxClicked(int)));
        }

        ui->showaloneCBX->setChecked(bookList[bookid]->showAlone);
        ui->mixedFrame->setEnabled(!ui->showaloneCBX->isChecked());
    }
    else ui->mixedGroup->hide();
}

void MainWindow::weavedCheckBoxClicked(int btnIndex)
{
    //Still a bit ugly

    QTreeWidgetItem* current = ui->treeWidget->currentItem();
    int bookid = bookList.FindBookByTWI(current);

    if (bookid == -1) return;

    QString id = weavedList[btnIndex]->whatsThis();

    for(int i=1; i<bookList[bookid]->mWeavedSources.size(); i++)
    {
        if (stringify(bookList[bookid]->mWeavedSources[i].id) == id)
        {
            bookList[bookid]->mWeavedSources[i].show = weavedList[btnIndex]->checkState();
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
    bookfind * bf = new bookfind(this, bookList);
    bf->show();

    connect(bf, SIGNAL(openBook(int)), this, SLOT(menuOpenBook(int)));
}

void MainWindow::menuOpenBook(int uid)
{
    int index = bookList.FindBookById(uid);

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
        bookDisplayerList[index]->deleteLater();
        bookDisplayerList.removeAt(index);

        //Destroy tab
        ui->viewTab->widget(index)->layout()->deleteLater();
        ui->viewTab->widget(index)->deleteLater();
        ui->viewTab->removeTab(index);
    }
    else
    {
        bookDisplayerList[0]->setHtml(simpleHtmlPage(tr("Orayta - Jewish books"), ""));
        ui->viewTab->setTabText(0, tr("Orayta - Jewish books"));
    }

    //Update the stupid tab widget
    on_viewTab_currentChanged(index - 1);
}

void MainWindow::on_changeLangButton_clicked()
{
    QSettings settings("Orayta", "SingleUser");
    settings.beginGroup("Confs");
    int i = langsDisplay.indexOf(ui->langComboBox->currentText());
    if (i != -1) settings.setValue("lang", langs[i]);
    settings.endGroup();

    QApplication::processEvents();

    //Reboot the program
    QProcess::startDetached("\"" + QApplication::applicationFilePath() + "\"");
    exit(0);
}

void MainWindow::openExternalLink(QString lnk)
{
    QStringList parts = lnk.split(":");

    int id;
    if(ToNum(parts[0], &id))
    {
        int index = bookList.FindBookById(id);
        if( index != -1)
        {
            //Add a new tab and open the link there
            addViewTab(false);
            ui->viewTab->setTabText(CURRENT_TAB, tr("Orayta"));

            CurrentBook->setInternalLocation("#" + parts[1]);

            if (parts.size() == 3)
            {
                //Mark the requested text in the new book
                LoadBook(bookList[index], unescapeFromBase32(parts[2]));
            }
            else LoadBook(bookList[index]);
        }
    }
}

void MainWindow::on_openMixed_clicked()
{
    int ind = bookList.FindBookByTWI(ui->treeWidget->currentItem());
    if (ind != -1)
    {
        //Reopen at current position, if exists

        //TODO: If active part is in view, use it instead

        //If it's the same book:
        if (bookList.FindBookById(CurrentBook->book()->getUniqueId()) == ind)
        {
            QString script = "var obj = ClosestElementToView();";
            CurrentBook->execScript(script);
            QString closest = CurrentBook->getJSVar("obj.href");

            QString link = closest.mid(closest.indexOf("$")+1);

            if (link != "") CurrentBook->setInternalLocation("#" + link);
        }

        openBook(ind);
    }
}

void MainWindow::on_showaloneCBX_clicked(bool checked)
{
    ui->mixedFrame->setEnabled(!checked);

    int ind = bookList.FindBookByTWI(ui->treeWidget->currentItem());
    if (ind != -1) bookList[ind]->showAlone = checked;
}

void MainWindow::on_showSearchBarButton_clicked(bool checked)
{
    ui->searchGroupBX->setVisible(checked);
}

void MainWindow::on_hideSearchButton_clicked()
{
    ui->showSearchBarButton->click();
}

void MainWindow::showSearchTab()
{
    ui->treeTab->setCurrentIndex(1);
}
