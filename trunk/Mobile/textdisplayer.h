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

#ifndef TEXTDISPLAYER_H
#define TEXTDISPLAYER_H

#include <QTextBrowser>
#include "../OraytaBase/booklist.h"
#include "../OraytaBase/book.h"

class textDisplayer : public QTextBrowser
{
    Q_OBJECT

public:
    explicit textDisplayer(QWidget *, BookList *);

    //Show the index of the book
    void display(Book * book);

    //Show the given chapter of the book
    // (Loads the chapter, and the tries to jump to the exact itr position)
    void display(Book * book, BookIter itr);

    //Jump to index
    void goToIndex();

    Book * getCurrentBook();
    BookIter getCurrentIter();

    //Zoomin
    void increaseSize();

    //Zoomout
    void decreaseSize();

    void reloadBook();

    void setSource(const QUrl &name);

    //Returns true if last page was a search result page
    bool isLastSearch();

    //Scrolls to the given y value when the document is fully loaded
    //scrollTo(int y);

private:

    //A pointer to the public booklist. We need this for weaved display
    BookList *booklist;

    //Holds a pointer to the currently loaded book
    Book * currentBook;

    BookIter currentIter;

    //Control whether nikud/teamim are displayed
    bool showNikud;
    bool showTeamim;

    QPoint _startPoint;
    QPoint _endPoint;

    bool inSwipe;

signals:
    void loadStart();
    void loadEnd(QUrl, Book*,  BookIter);

public slots:
    //Called when anchor clicked
    void processAnchor(const QUrl &);

    void leftSwipe();
    void rightSwipe();
    void swipeDone();
protected:
    //virtual void mousePressEvent(QMouseEvent *ev);
    //virtual void mouseReleaseEvent(QMouseEvent *ev);
};

#endif // TEXTDISPLAYER_H
