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
* Author: Izar <izar00@gmail.com>
*/

#ifndef MINIBMARK_H
#define MINIBMARK_H

//#include <QWidget>
#include <QListWidgetItem>
#include <QListWidget>
#include <QObject>
#include "../OraytaBase/book.h"
#include "../OraytaBase/bookiter.h"

class BMarkList;

class MiniBMark : public QObject, public QListWidgetItem
{
    Q_OBJECT

public:
    explicit MiniBMark(Book *b, BookIter iter , BMarkList *parent, int viewPos =0);
    int viewPosition;

signals:

public slots:
    Book* getBook();
    BookIter getBookIter();
    bool isConstant();
    void setBook(Book *b);
    void setBookIter(BookIter iter);
    void setConstant(bool);
    void setTitle(QString title);
    QString title();
    int getUid();
    BMarkList * getParentList();

private:
    //id for this bookmark
    int id;
    Book* mBook;
    BookIter mBIter;
    BMarkList * parentList;

    //true if user wants this bookmark to remain in place;
    bool constant;

};

#endif // MINIBMARK_H
