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

#ifndef TREEITEM_BASE_BOOK_H
#define TREEITEM_BASE_BOOK_H


#include <QPointer>

#include "treeitem_base.h"
//#include "bookdisplayer.h"

//class NodeBook;


class NodeBook : public BaseNodeItem
{
public:
    enum Booktype {
        Orayta,
        Html,
        Pdf,
        Link,
        Unkown
    };

    NodeBook(BaseNodeItem *parent, QString path, QString name, bool isUserBook);

    //getters
    Nodetype nodetype() const;
    BookDisplayer* tabWidget() const;
    int getUniqueId() const;
    int getRealUniqueId() const;

    virtual bool isFontModifiable() const;
    virtual bool isSearchable() const;
    virtual Booktype booktype() const = 0;
    virtual QList<QAction*> menuActions() const;
    virtual QString getLoadableFile() const;

    // setters
    void setTabWidget(BookDisplayer*);
    void setRandomUniqueId(int id);

    static NodeBook* BookFactory(BaseNodeItem *parent, QString path, QString name, Booktype ft, bool isUserBook);


protected:
    virtual void AddConfs();

    //QWidget* of current viewTab (0 if not exists)
    QPointer<BookDisplayer> mTabWidget;

    //Unique id used for linking to a specific book
    int mUniqueId;

    int mRandomId;
};


typedef QList<NodeBook*> BookList;


#endif // TREEITEM_BASE_BOOK_H
