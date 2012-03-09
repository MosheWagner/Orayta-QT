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

    booklist = bl;

    showNikud = true;
    showTeamim = true;

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
        //Level mark is allways 1 digit only
        QString lvls = link.mid(pos+1, 1);
        QString lnk = link.mid(pos+2);

        BookIter itr = BookIter::fromEncodedString(lnk);
        currentIter = itr;

        bool ok;
        int lvl = lvls.toInt(&ok) -1;

        QString html = currentBook->getChapterHtml(itr, booklist, true, true, ok ? lvl : 1);
        setHtml(html);
    }
}

//Show the index of the book
void textDisplayer::display(Book * book)
{  
    currentBook = book;
    currentMode = BookDisplay;

    //@@@
    book->readBook(1);

    setHtml(currentBook->getBookIndexHtml());
}


//Show the given chapter of the book
// (Loads the chapter, and the tries to jump to the exact itr position)
void textDisplayer::display(Book * book, BookIter itr, int splitlevel)
{
    currentBook = book;
    currentMode = BookDisplay;
    currentIter = itr;

    //@@@
    book->readBook(1);

    QString html = currentBook->getChapterHtml(itr, booklist, showNikud, showTeamim, splitlevel);
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
