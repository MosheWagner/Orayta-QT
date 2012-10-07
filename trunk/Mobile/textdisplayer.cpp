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
#include <QApplication>

textDisplayer::textDisplayer(QWidget *p, BookList *bl) : QTextBrowser(p)
{
    setTextInteractionFlags(Qt::LinksAccessibleByKeyboard | Qt::LinksAccessibleByMouse);

    booklist = bl;

    showNikud = true;
    showTeamim = true;

    currentBook = NULL;

    //setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    connect(this, SIGNAL(anchorClicked(QUrl)), SLOT(processAnchor(QUrl)));

    //remove arrows from scrollBars
    QString noArrows = "QScrollBar { margin: 0px ; width: 5px;}";
         noArrows +=    "QScrollBar::add-line, QScrollBar::sub-line{border: 0px;  height: 0px; }";
         noArrows +=   "*::right-arrow{image: none; } *::left-arrow{image: none;} *::up-arrow{image: none;} *::down-arrow{image: none;}";
    this->setStyleSheet(noArrows);

    inSwipe = false;
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

        QUrl u = currentBook->renderChapterHtml(&itr, booklist, true, true);
        setSource(u);
    }
}

//Show the index of the book
void textDisplayer::display(Book * book)
{  
    currentBook = book;
    currentIter = BookIter();

    //@@@
    book->readBook(1);

    QUrl u = currentBook->renderBookIndex();
    setSource(u);
}

#include <QScrollBar>

//Show the given chapter of the book
// (Loads the chapter, and the tries to jump to the exact itr position)
void textDisplayer::display(Book * book, BookIter itr)
{
    //Show index
    if (itr == BookIter()) display(book);

    currentBook = book;
    currentIter = itr;

    //@@@
    book->readBook(1);

    QUrl u = currentBook->renderChapterHtml(&itr, booklist, showNikud, showTeamim);

    if (u != QUrl()) setSource(u);
    else
    {
        qDebug()<< "Render error!";
        return;
    }

    scrollToAnchor(itr.toEncodedString());
}

//Jump to index
void textDisplayer::goToIndex()
{
    //Empty the current iter
    currentIter = BookIter();

    if (currentBook)
    {
        QUrl u = currentBook->renderBookIndex();
        setSource(u);
    }
}

Book * textDisplayer::getCurrentBook() { return currentBook; }

BookIter textDisplayer::getCurrentIter() { return currentIter; }

#include <QMouseEvent>
#include <QScrollBar>

/*
void textDisplayer::mousePressEvent(QMouseEvent *ev)
{
    qDebug() << "A";

    _startPoint = ev->pos();

    QTextBrowser::mousePressEvent(ev);
}

// minimal movment to be considered as a swipe and not a press
#define MIN_MOVMENT 40
void textDisplayer::mouseReleaseEvent(QMouseEvent *ev)
{
        qDebug() << "B";

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
            if (it != BookIter()) display(currentBook, it);

        } else {
            //Right swipe
            it = currentBook->nextChap(it);
            if (it != BookIter()) display(currentBook, it);
        }
    }
    else {
        QTextBrowser::mouseReleaseEvent(ev);
    }
}
*/

void textDisplayer::rightSwipe()
{
    if (!inSwipe)
    {
        inSwipe = true;
        BookIter it = currentIter;
        it = currentBook->nextChap(it);
        if (it != BookIter()) display(currentBook, it);
    }
}

void textDisplayer::leftSwipe()
{
    if (!inSwipe)
    {
        inSwipe = true;
        BookIter it = currentIter;
        it = currentBook->prevChap(it);
        if (it != BookIter()) display(currentBook, it);
    }
}

void textDisplayer::swipeDone()
{
    inSwipe = false;
}

#include <QFont>

void textDisplayer::increaseSize()
{
    QFont f = currentBook->getFont();
    f.setPointSize(f.pointSize() + 2);
    currentBook->setFont(f);

    reloadBook();
}

void textDisplayer::decreaseSize()
{
    QFont f = currentBook->getFont();
    f.setPointSize(f.pointSize() - 2);
    currentBook->setFont(f);

    reloadBook();
}

void textDisplayer::reloadBook()
{
    display(currentBook, currentIter);
}


void textDisplayer::setSource(const QUrl &name)
{
    //Cause ui to show wait status
    emit loadStart();
//    QApplication::processEvents();

    QTextBrowser::setSource(name);

    QApplication::processEvents();
    emit loadEnd(name, currentBook, currentIter);
}

bool textDisplayer::isLastSearch()
{
    if (historyUrl(-1).toString().indexOf("SEARCH") != -1) return true;

    return false;
}
