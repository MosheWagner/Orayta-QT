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

#include "textdisplayer.h"

#include <QDebug>
#include <QFile>
#include <QDesktopServices>

textDisplayer::textDisplayer(QWidget *p, BookList *bl) : QTextBrowser(p)
{
    //setOpenLinks(false);

    setTextInteractionFlags(Qt::LinksAccessibleByKeyboard | Qt::LinksAccessibleByMouse);

    booklist = bl;

    showNikud = true;
    showTeamim = true;

    currentBook = NULL;

    connect(this, SIGNAL(anchorClicked(QUrl)), SLOT(processAnchor(QUrl)));
}



void textDisplayer::processAnchor(const QUrl &url)
{
    QString link = QString(url.toString());

    qDebug() << "url clicked: " << url;

    /*
    if(link.indexOf("#") != -1 )
    {
        int pos = link.indexOf("#");
        QString lnk = link.mid(pos+1);

        scrollToAnchor("\"$" + lnk + "\"");
    }
    */

    //External book link
    if(link.indexOf("!") != -1 )
    {
        int pos = link.indexOf("!");

        QString lnk = link.mid(pos+1);

        //Open the link
        QStringList parts = lnk.split(":");

        int id;
        if(ToNum(parts[0], &id))
        {
            Book* book = booklist->findBookById(id);
            if( book )
            {
                BookIter itr = BookIter::fromEncodedString(parts[1]);

                qDebug() << itr.toString();
                /*
                if (parts.size() == 3)
                    CurrentBookdisplayer()->setSearchMarker( QRegExp(unescapeFromBase32(parts[2])) );
                */

                display(book, itr);
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

    //Link to chapter in book
    else if(link.indexOf("@") != -1 )
    {
        int pos = link.indexOf("@");
        QString lnk = link.mid(pos+1);

        BookIter itr = BookIter::fromEncodedString(lnk);
        currentIter = itr;

        QString html = currentBook->getChapterHtml(&itr, booklist, true, true);
        setHtml(html);
    }
}

//Show the index of the book
void textDisplayer::display(Book * book)
{  
    currentBook = book;
    currentMode = BookDisplay;
    currentIter = BookIter();

    //@@@
    book->readBook(1);

    setHtml(currentBook->getBookIndexHtml());
}


//Show the given chapter of the book
// (Loads the chapter, and the tries to jump to the exact itr position)
void textDisplayer::display(Book * book, BookIter itr)
{
    currentBook = book;
    currentMode = BookDisplay;
    currentIter = itr;

    //@@@
    book->readBook(1);

    QString html = currentBook->getChapterHtml(&itr, booklist, showNikud, showTeamim);

    if (html == "") {
        qDebug()<< "empty html";
        return;
    }

    setHtml(html);

    //TODO: Search results need to be improved
    scrollToAnchor(itr.toEncodedString());
}

void textDisplayer::displaySearchResult(QString html)
{
    currentMode = SearchResultDisplay;

    setHtml(html);
}

//Show html book
void textDisplayer::displayHtml(QUrl url)
{
    QFile f(url.toLocalFile());
    currentMode = HtmlFileDisplay;

    setHtml(f.readAll());
}

//Jump to index
void textDisplayer::goToIndex()
{
    //Empty the current iter
    currentIter = BookIter();

    if (currentMode == BookDisplay && currentBook) setHtml(currentBook->getBookIndexHtml());
}

Book * textDisplayer::getCurrentBook() { return currentBook; }

BookIter textDisplayer::getCurrentIter() { return currentIter; }

#include <QMouseEvent>

void textDisplayer::mousePressEvent(QMouseEvent *ev)
{
    _startPoint = ev->pos();

    QTextBrowser::mousePressEvent(ev);
}

// minimal movment to be considered as a swipe and not a press
#define MIN_MOVMENT 50
void textDisplayer::mouseReleaseEvent(QMouseEvent *ev)
{
    _endPoint = ev->pos();

    //process distance and direction
    int xDiff = qAbs(_startPoint.x() - _endPoint.x());
    int yDiff = qAbs(_startPoint.y() - _endPoint.y());
    if( xDiff > MIN_MOVMENT && xDiff > yDiff )
    {
        BookIter it = currentIter;

        // horizontal swipe detected, now find direction
        if( _startPoint.x() > _endPoint.x() ) {
            //Left swipe:
            it = currentBook->prevChap(it);
            display(currentBook, it);

        } else {
            //Right swipe
            it = currentBook->nextChap(it);
            display(currentBook, it);
        }
    }
    else {
        QTextBrowser::mouseReleaseEvent(ev);
    }
}
