#include "bookdisplayer.h"


//////////TODO:
//Auto detection on current node in view?
//Add comment s via script?


bookDisplayer::bookDisplayer(QWidget *parent, QTabWidget * tabviewptr)
{
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
    QObject::connect(htmlview, SIGNAL(loadStarted()), this , SLOT(htmlView_loadStarted()));
    QObject::connect(htmlview, SIGNAL(loadProgress (int)), this , SLOT(htmlView_loadProgress(int)));

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

    stackedWidget->setCurrentIndex(1);
    


    setHtml(simpleHtmlPage(tr("Orayta"), ""));
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

    //Hide the progressbar
    //ui->progressBar->hide();

    if ( InternalLocationInHtml != "")
    {
        //Mark location as "active"
        QString script = "paintByHref(\"" + InternalLocationInHtml.replace("#", "$") + "\");";

        //Make sure script was loaded before erasing InternalLocationInHtml.
        //This should only affect KHTML anyway
        if (getJSVar("findByLink").replace(" ","") != "")
        {
            execScript(script);
            InternalLocationInHtml="";
        }

    }

    //stackedWidget->setCurrentIndex(0);

    //Return cusror to normal
    QApplication::restoreOverrideCursor();
}


QString last = "";

//Omitted when a link was clicked in the webView
void bookDisplayer::htmlView_linkClicked(QUrl url)
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

        QStringList parts = lnk.split(":");

        //Emmit about this
        /*
        int id;
        if(ToNum(parts[0], &id))
        {
            int index = gBookList.FindBookById(id);
            if( index != -1)
            {
                //Add a new tab and open the link there
                addViewTab(false);
                ui->viewTab->setTabText(CURRENT_TAB, tr("Orayta"));

                CurrentBook->setInternalLocation("#" + parts[1]);

                ///%%%%%%%%%
                if (parts.size() == 3)
                {
                    LoadBook(gBookList[index], parts[2]);
                }
                else
                    LoadBook(gBookList[index]);
            }
        }
        */
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

void bookDisplayer::setTextSizeMultiplier(qreal factor)
{
    htmlview->setTextSizeMultiplier(factor);
}

qreal bookDisplayer::textSizeMultiplier() {return htmlview->textSizeMultiplier(); }

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
