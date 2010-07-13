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

#include "bookdisplayer.h"


//////////TODO:
//Auto detection on current node in view?
//Add comment s via script?


bookDisplayer::bookDisplayer(QWidget *parent, QTabWidget * tabviewptr)
{
    //Hold a pointer to the mainwindow, so we can tell it stuff to do
    MW = (MainWindow *) parent;
    TW = tabviewptr;

    shownikud = true;
    showteamim = true;

    myBook = NULL;

    //Holds currently selected link
    mActiveLink = "";

    myurl = "";

    //Create new webview
#ifdef KHTML
    htmlview = new KHTMLWidget(this);
#else
    htmlview = new myWebView(this);
#endif

    //Connect the signalls sent from the new widgets to their slots
    QObject::connect(htmlview, SIGNAL(LinkClicked(QUrl)), this , SLOT(htmlView_linkClicked(QUrl)));
#ifdef KHTML
    QObject::connect(htmlview, SIGNAL(completed()), this , SLOT(htmlView_loadFinished()));
#else
    QObject::connect(htmlview, SIGNAL(loadFinished(bool)), this , SLOT(htmlView_loadFinished(bool)));
    //Prevent context menus. If needed, they will be created manually
    htmlview->setContextMenuPolicy(Qt::PreventContextMenu);
    //The myWebView class will handle all link, but will emit "LinkMenu" when one should be shown
    htmlview->page()->setLinkDelegationPolicy(QWebPage::DontDelegateLinks);
#endif

    //Create new hbox
    vbox = new QVBoxLayout(this);
    vbox->setMargin(0);

    waitView = new KHTMLWidget(this);


    QFileInfo f("Images/Wait.gif");
    QString html = simpleHtmlPage("hello", "<br><br><img src=\"" + f.absoluteFilePath() + "\" ></img>");
    
    waitView->setHtml(html);

    stackedWidget = new QStackedWidget(this);
    stackedWidget->addWidget(htmlQWidget());
    stackedWidget->addWidget(waitView->getQWidget());

    stackedWidget->show();
    vbox->addWidget(stackedWidget);


    setHtml(simpleHtmlPage(tr("Orayta"), ""));

    //htmlview->setCaretMode(true);
}

bookDisplayer::~bookDisplayer()
{
    deleteLater();
}

//Signall omitted when the webView finishes loading a file
#ifdef KHTML
void bookDisplayer::htmlView_loadFinished()
{
    if (myurl.toString() == "") return;
#else
void bookDisplayer::htmlView_loadFinished(bool)
{
#endif
    //change the tabs title from "Loading..." to the book's title
    int index = TW->indexOf(this);

    if (index != -1) TW->setTabText(index, title());

    //Mark location as "active"
    if ( InternalLocationInHtml != "")
    {
        QString script = "paintByHref(\"" + InternalLocationInHtml.replace("#", "$") + "\");";

        //Make sure script was loaded before erasing InternalLocationInHtml.
        //This should only affect KHTML anyway
        if (getJSVar("findByLink").replace(" ","") != "")
        {
            execScript(script);
            InternalLocationInHtml="";
        }
    }

    //Hide "wait" page and show the real page
    stackedWidget->setCurrentIndex(0);
}

// KHTML calls "linkClicked" twice for some stupid reason, so we ignore the second time
QString last = "";

//Omitted when a link was clicked in the webView
void bookDisplayer::htmlView_linkClicked(QUrl url)
{
    QString link = QString(url.path());

#ifdef KHTML
    //Yuck!!! Disgusting!!!

    //Ignore the extra call of this function
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
        int id = myBook->getUniqueId();

        int p = link.indexOf("$");
        link = stringify(id) + ":" + link.mid(p+1);


        QMenu menu(this);

        QAction *mark = new QAction(tr("Add bookmark here..."), &menu);

        mark->setIcon(QIcon(":/Icons/bookmarks.png"));
        menu.addAction(mark);

        QSignalMapper *signalMapper = new QSignalMapper(this);

        signalMapper->setMapping(mark, QString(link));
        connect(mark, SIGNAL(triggered()), signalMapper, SLOT (map()));
        connect(signalMapper, SIGNAL(mapped(const QString &)), MW, SLOT(bookMarkPosition(QString)));

        QAction *comment = new QAction(tr("Add/edit comment..."), &menu);

        menu.addAction(comment);
        comment->setIcon(QIcon(":/Icons/edit.png"));

        QSignalMapper *csignalMapper = new QSignalMapper(this);
        csignalMapper->setMapping(comment, QString(link));
        connect(comment, SIGNAL(triggered()), csignalMapper, SLOT (map()));
        connect(csignalMapper, SIGNAL(mapped(const QString &)), MW, SLOT(openCommentDialog(QString)));

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
        int id = myBook->getUniqueId();

        int p = link.indexOf("*");
        link = stringify(id) + ":" + link.mid(p+1);


        QMenu menu(this);

        QSignalMapper *signalMapper = new QSignalMapper(this);

        QAction *comment = new QAction(tr("Edit comment..."), &menu);

        comment->setIcon(QIcon(":/Icons/edit.png"));
        menu.addAction(comment);

        QSignalMapper *csignalMapper = new QSignalMapper(this);
        csignalMapper->setMapping(comment, QString(link));
        connect(comment, SIGNAL(triggered()), csignalMapper, SLOT (map()));
        connect(csignalMapper, SIGNAL(mapped(const QString &)), MW, SLOT(openCommentDialog(QString)));


        QAction *delcomment = new QAction(tr("Delete comment"), &menu);
        delcomment->setIcon(QIcon(":/Icons/edit-delete.png"));
        menu.addAction(delcomment);

        QSignalMapper *dcsignalMapper = new QSignalMapper(this);
        dcsignalMapper->setMapping(delcomment, QString(link));
        connect(delcomment, SIGNAL(triggered()), dcsignalMapper, SLOT (map()));
        connect(dcsignalMapper, SIGNAL(mapped(const QString &)), MW, SLOT(removeComment(QString)));

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

        //Tell the MainWindow to open this link in a new tab
        emit externalLink(lnk);
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



void bookDisplayer::setHtml(QString html)
{
    myurl = "";

    htmlview->setHtml(html);

    //Set book to null or 0?
}

void bookDisplayer::execScript(QString script)
{
    htmlview->execScript(script);
}

QString bookDisplayer::getJSVar(QString var)
{
    return htmlview->getJSVar(var);
}

QString bookDisplayer::activeLink()
{
    return htmlview->activeLink();
}

void bookDisplayer::ZoomIn()
{
    htmlview->setTextSizeMultiplier(htmlview->textSizeMultiplier() + 0.1);
}

void bookDisplayer::ZoomOut()
{
    if (htmlview->textSizeMultiplier() > 0.1)
    {
        htmlview->setTextSizeMultiplier(htmlview->textSizeMultiplier() - 0.1);
    }
}

void bookDisplayer::normalZoom()
{
    htmlview->setTextSizeMultiplier(1);
}

void bookDisplayer::load(QUrl url)
{ 

    stackedWidget->setCurrentIndex(1);

    url.setUrl(QString(url.toString() + InternalLocationInHtml));

    myurl = url;

    htmlview->load(url);
}

void bookDisplayer::setInternalLocation(QString location)
{
    InternalLocationInHtml = location;
}

QString bookDisplayer::title() { return htmlview->title(); }

bool bookDisplayer::isNikudShown() { return shownikud; }
bool bookDisplayer::areTeamimShown() { return showteamim; }

void bookDisplayer::showNikud(bool show)
{
    shownikud = show;
}

void bookDisplayer::showTeamim(bool show)
{
    showteamim = show;
}

Book * bookDisplayer::book() { return myBook; }

void bookDisplayer::setBook( Book * book)
{
    myBook = book;
}

QWidget * bookDisplayer::htmlQWidget()
{
#ifdef KHTML
    return htmlview->getQWidget();
#else
    return htmlview;
#endif
}

QString bookDisplayer::htmlSource()
{
#ifdef KHTML
    return htmlview->document().toString().string();
#else
    return htmlview->page()->mainFrame()->toPlainText();
#endif
}

void bookDisplayer::searchText(QString text, bool backwards)
{
#ifdef KHTML
    if (text != lastSearch) htmlview->buildSearch(text);
    lastSearch = text;

    htmlview->searchText(backwards);
#else
    if (backwards == true) htmlview->findText(text, QWebPage::FindBackward);
    else htmlview->findText(text, 0);
#endif
}
