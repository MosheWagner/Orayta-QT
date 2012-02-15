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
* Author: Yoch Melka. <yoch.melka@gmail.com>
*/

#ifndef BOOKDISP_H
#define BOOKDISP_H


#include <QWidget>
#include <QTabWidget>
#include <QLabel>
#include <QUrl>
#include <QProgressBar>
#include <QStackedLayout>
#include <QStackedWidget>
#include <QMovie>


#include "treeitem_base_book.h"

/*
  Class holding all widgets and objects needed to display a book.
    The main object is a plain QWidget.

    In it, a stackedwidget allows us to switch between two views,
     one for the "loading" image, and one for the book itself
*/

class MainWindow;
class BookViewInterface;



class BookDisplayer : public QWidget
{
    //Very important. Without it, it won't create signalls or slots.
    Q_OBJECT

public:
    BookDisplayer(QWidget * parent, QTabWidget * tabviewptr);
    ~BookDisplayer();

    // wrappers
    void loadBook( NodeBook * );
    void loadSearchPage(const QUrl&);

    // default implementation : nothing
    void ZoomIn() const;
    void ZoomOut() const;
    void normalZoom() const;
    void jumpToTop() const;
    void setInternalLocation(const QString&) const;
    void searchText(const QString& text, bool backwards = false) const;
    void print() const;

    bool hasNikud() const;
    bool hasTeamim() const;

    NodeBook::Booktype displayedBookType();


    NodeBook * book() const;
    BookViewInterface* bookView() const;
    MainWindow * mainWnd() const;
    QWidget* additionalButtons() const;

public slots:
    void setTitle(QString);
//    void showWaitAnimation();
//    void hideWaitAnimation();

private:

    void resetBookView();

    //Html viewer the book is shown within
    MainWindow * MW;
    QTabWidget * TW;

    //Pointer to the book that's displayed here
    NodeBook* myBook;

    BookViewInterface* mBookView;

    //Layout holding the widgets
    QVBoxLayout* vbox;

//    QStackedLayout * vbox;

//    QLabel * waitLbl;
//    QMovie * waitAnimation;

};


#endif // BOOKDISP_H
