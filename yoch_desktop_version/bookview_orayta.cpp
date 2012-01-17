#include "bookview_orayta.h"
#include "bookdisplayer.h"
#include "mainwindow.h"
#include "addcomment.h"
#include "treeitem_orayta.h"


#include <QtCore>
#include <QApplication>
#include <QClipboard>


using namespace std;

OraytaBookView::OraytaBookView( BookDisplayer * parent ) :
    BaseHtmlView(parent),
    mSearchPos(-1),
    mInternalBook(0),
    shownikud(false),
    showteamim(false)
{
    page()->setLinkDelegationPolicy(QWebPage::DelegateAllLinks);

    mark = new QAction(QIcon(":/Icons/bookmarks.png"), tr("Add bookmark here..."), this);
    comment = new QAction(QIcon(":/Icons/edit.png"), tr("Add/edit comment..."), this);
    delcomment = new QAction(QIcon(":/Icons/edit-delete.png"), tr("Delete comment"), this);
    copyNoSigns = new QAction(QIcon(":/Icons/copy-clean.png"), tr("Copy text only"), this);

    msignalMapper = new QSignalMapper(this);
    csignalMapper = new QSignalMapper(this);
    dsignalMapper = new QSignalMapper(this);

    QObject::connect(mark, SIGNAL(triggered()), msignalMapper, SLOT (map()));
    QObject::connect(comment, SIGNAL(triggered()), csignalMapper, SLOT (map()));
    QObject::connect(delcomment, SIGNAL(triggered()), dsignalMapper, SLOT (map()));

    QObject::connect(msignalMapper, SIGNAL(mapped(const QString &)), mBookDisp->mainWnd(), SLOT(bookMarkPosition(QString)));
    QObject::connect(csignalMapper, SIGNAL(mapped(const QString &)), this, SLOT(openCommentDialog(QString)));
    QObject::connect(dsignalMapper, SIGNAL(mapped(const QString &)), this, SLOT(removeComment(QString)));

    QObject::connect(copyNoSigns, SIGNAL(triggered()), this, SLOT(copyClean()));

    QObject::connect(this, SIGNAL(linkClicked(const QUrl&)), this , SLOT(on_linkClicked(const QUrl&)));
    QObject::connect(this, SIGNAL(externalLink(QString)), mBookDisp->mainWnd(), SLOT(openExternalLink(QString)));
}

NodeBook::Booktype OraytaBookView::booktype() const
{  return NodeBook::Orayta;  }

void OraytaBookView::loadBook(const NodeBook* book)
{
    if (book->booktype() != NodeBook::Orayta) return;  // sanity check

    mInternalBook = dynamic_cast<OraytaBookItem*>((NodeBook*)book);
    if (!mInternalBook) return;

    // ########## inconsistent
    shownikud = mInternalBook->areNikudShown();
    showteamim = mInternalBook->areTeamimShown();
    loadUrl(QUrl::fromLocalFile(mInternalBook->getLoadableFile()));
}

void OraytaBookView::reload()
{
    if (!mInternalBook)  //  sanity check
        return;

    // force new render of this book
    mInternalBook->htmlrender();

    page()->triggerAction(QWebPage::Reload);
}

// special function, for reload after html modifications
void OraytaBookView::reloadAtSameLocation()
{
    if (!mInternalBook)  //  sanity check
        return;

    execScript("var obj = ClosestElementToView();");
    QString closest = getJSVar("obj.name");
    QString link = closest.mid(closest.indexOf("$")+1);

    loadUrl(QUrl::fromLocalFile(mInternalBook->getLoadableFile()));
    setInternalLocation(link);
}

void OraytaBookView::jumpToTop()
{  page()->mainFrame()->scrollToAnchor("Top");  }

void OraytaBookView::setInternalLocation(const QString& location)
{
    BaseHtmlView::setInternalLocation(location);
    execScript(QString("paintByName('" + location + "');"));
}

QString OraytaBookView::activeLink()
{
    return getJSVar("currentlyPainted.name");
}

void OraytaBookView::highlight(const QRegExp& rx)
{
    if (loading)
    {
        addDelayedAction(std::tr1::bind(&OraytaBookView::highlight, this, rx));
        return;
    }

    QString txt = page()->mainFrame()->toPlainText();
    QStringList listTerms;
    int pos = 0;

    while ((pos = rx.indexIn(txt, pos)) != -1)
    {
        listTerms << rx.cap(0);
        pos += rx.matchedLength();
    }

    listTerms.removeDuplicates();

    unhighlight();
    for (QStringList::const_iterator it = listTerms.begin(); it != listTerms.end(); ++it)
    {
        findText(*it, QWebPage::HighlightAllOccurrences);
    }
}

void OraytaBookView::unhighlight()
{
    if (loading)
    {
        addDelayedAction(std::tr1::bind(&OraytaBookView::unhighlight, this));
        return;
    }

    findText("", QWebPage::HighlightAllOccurrences);
}

void OraytaBookView::searchText(const QString& text, bool backwards)
{
    if (text.trimmed() == "")   // sanity check
        return;

    QString pageText = page()->mainFrame()->toPlainText();
    QRegExp regexp = withNikudAndTeamim(text);

    int ppp = mSearchPos;

    if (!backwards)
    {
        //Find the closest occurence of the RegExp
        int pos = regexp.indexIn(pageText, ppp + 1);
        if (pos > -1)
        {
            QString next = regexp.cap(0);
            mSearchPos = pos;
            BaseHtmlView::searchText(next, false);
        }
    }
    else
    {
        //Find the closest occurence of the RegExp, backwards
        int pos = regexp.lastIndexIn(pageText, -(pageText.length() - ppp + 1));
        if (pos > -1)
        {
            QString last = regexp.cap(0);
            mSearchPos = pos;
            BaseHtmlView::searchText(last, true);
        }
    }
}

void OraytaBookView::copyClean()
{
    QString text = removeSigns(page()->selectedText());
    if (!text.isEmpty())
        QApplication::clipboard()->setText(text, QClipboard::Clipboard);
}

//when a link was clicked in the OraytaView
void OraytaBookView::on_linkClicked(const QUrl& url)
{
    //qDebug() << "  linkClicked() : " << url.toString();

    QString link = QString(url.toString());

    int pos;
    //Internal link
    if ( (pos = link.indexOf("#")) != -1 )
    {
        QString lnk = link.mid(pos+1);
        page()->currentFrame()->scrollToAnchor(lnk);
    }
    //External book link
    else if ( (pos = link.indexOf("!")) != -1 )
    {
        //Tell the MainWindow to open this link in a new tab
        emit externalLink(link.mid(pos+1));
    }
    //Link to website
    else if( (pos = link.indexOf("^")) != -1 )
    {
        //qDebug() << "website link";

        //Open using browser
        QDesktopServices::openUrl( QUrl("http://" + link.mid(pos+1)) );
    }
}

// Context menus
void OraytaBookView::mousePressEvent(QMouseEvent *event)
{
    //Treat right clicks
    if (event->button() == Qt::RightButton)
    {
        QWebHitTestResult r = page()->mainFrame()->hitTestContent(event->pos());

        if ( r.linkUrl().isEmpty() )
        {
            //Display "copy" menu
            QMenu menu(this);

            menu.addAction(copyAction);
            menu.addAction(copyNoSigns);

            menu.setLayoutDirection(Qt::RightToLeft);
            menu.exec(mapToGlobal(event->pos()));
        }
        else //Click on link
        {
            QString link = r.linkUrl().toString();
            int pos;
            //Not actually a link. A menu should open here
            if ( (pos = link.indexOf("$")) != -1 )
            {
                //Find book's id and add it to the link
                int id = mBookDisp->book()->getUniqueId();

                link = QString::number(id) + ":" + link.mid(pos+1);

                QMenu menu(this);
                menu.addAction(mark);
                menu.addAction(comment);
                menu.setLayoutDirection(Qt::RightToLeft);

                msignalMapper->setMapping(mark, link);
                csignalMapper->setMapping(comment, link);

                //Open the menu to the left of the cursor's position
                QPoint pos = QPoint(QCursor::pos().x() - menu.width(), QCursor::pos().y());
                menu.exec(pos);

                msignalMapper->removeMappings(mark);
                csignalMapper->removeMappings(comment);
            }
            //A comment clicked. A menu should open here too
            else if ( (pos = link.indexOf("*")) != -1 )
            {
                //Find book's id and add it to the link
                int id = mBookDisp->book()->getUniqueId();

                link = QString::number(id) + ":" + link.mid(pos+1);

                QMenu menu(this);
                menu.addAction(comment);
                menu.addAction(delcomment);
                menu.setLayoutDirection(Qt::RightToLeft);

                csignalMapper->setMapping(comment, link);
                dsignalMapper->setMapping(delcomment, link);

                //Open the menu to the left of the cursor's position
                QPoint pos = QPoint(QCursor::pos().x() - menu.width(), QCursor::pos().y());
                menu.exec(pos);

                csignalMapper->removeMappings(comment);
                dsignalMapper->removeMappings(delcomment);
            }
        }
    }
    else QWebView::mousePressEvent(event);
}

void OraytaBookView::keyPressEvent( QKeyEvent *keyEvent )
{
    if ( keyEvent->modifiers() == Qt::NoModifier )
    {
        switch ( keyEvent->key() )
        {
        case Qt::Key_Left:
        case Qt::Key_Down:
            page()->mainFrame()->evaluateJavaScript("paintNext();");
            return;

        case Qt::Key_Right:
        case Qt::Key_Up:
            page()->mainFrame()->evaluateJavaScript("paintPrevious();");
            return;

        case Qt::Key_Backspace:  //Ignore backspace clicks
            return;
        }
    }
    // default
    QWebView::keyPressEvent(keyEvent);
}


////////////////////////////////////////////////////////////
//                nikud/teamim gestion
////////////////////////////////////////////////////////////

bool OraytaBookView::hasNikud() const
{
    return (mInternalBook ? mInternalBook->HasNikud() : false);
}

bool OraytaBookView::hasTeamim() const
{
    return (mInternalBook ? mInternalBook->HasTeamim() : false);
}

/*
bool OraytaBookView::areNikudShown() const
{
    return shownikud;
}

bool OraytaBookView::areTeamimShown() const
{
    return showteamim;  // sanity check
}
*/

void OraytaBookView::showNikud(bool show)
{
    if (hasNikud())
    {
        mInternalBook->showNikud(show);
        shownikud = show;

        if (!show)
        {
            execScript("removeNikud();");
        }
        else
        {
            reload();
        }
    }
}

void OraytaBookView::showTeamim(bool show)
{
    if (hasTeamim())
    {
        mInternalBook->showTeamim(show);
        showteamim = show;

        if (!show)
        {
            execScript("removeTeamim();");
        }
        else
        {
            reload();
        }
    }
}



////////////////////////////////////////////////////////////
//                comments gestion
////////////////////////////////////////////////////////////


//Show the comment adding / editing dialog
void OraytaBookView::openCommentDialog(QString link)
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
void OraytaBookView::addCommentAtPosition(QString link, QString comment)
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
    execScript(script);

    if (!mInternalBook) return;  // sanity check
    //Recreate this page with new comment in other thread (new render of the book)
    // why don't work without dereferencement ?
    QtConcurrent::run( *mInternalBook, &OraytaBookItem::htmlrender );
}

void OraytaBookView::removeComment(QString link)
{
    //Adding an emtpy comment removes the existing one
    addCommentAtPosition(link, "");
}


/*
// ############
QString BookView::pageText()
{
    return page()->mainFrame()->toPlainText();
}

QString BookView::htmlSource()
{
    return page()->mainFrame()->toHtml();
}
*/
