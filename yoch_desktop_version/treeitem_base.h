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

#ifndef TREEITEM_BASE_H
#define TREEITEM_BASE_H


#include <QTreeWidgetItem>
#include <QString>
#include <QList>
#include <QFont>

//#include "bookdisplayer.h"


class BookDisplayer;
class NodeBook;
class BookTree;

//Class containing information about the books and folders.
class BaseNodeItem : public QTreeWidgetItem
{
public:
    enum Nodetype {
        Node,
        Leaf
    };

    enum IconState {
        BLUE,
        HALF,
        GREY
    };

// Name ?
    BaseNodeItem (BaseNodeItem *parent, QString name = "", QString path = "", bool isUserBook = false);
    virtual ~BaseNodeItem();

    virtual Nodetype nodetype() const = 0;
    virtual bool isFontModifiable() const = 0;
    virtual bool isSearchable() const = 0;

    virtual QList<QAction*> menuActions() const;

    virtual bool IsInSearch() const;
    virtual bool IsHidden() const;
    virtual bool IsUserCheckable() const;
    virtual bool IsUserBook() const;

    virtual QString getPath() const;

    virtual QString getTreeDisplayName() const;

    virtual void setName(QString name);
    virtual void changeFont( const QFont& );

    virtual void setCheckState(IconState state);

    IconState getIconState() const;

    BaseNodeItem* getParent();
    QList<BaseNodeItem*>& getChildren();

    NodeBook* getBookPtrFromId(int) const;

    //Add the pointed book as a child to this folder
    void add_child(BaseNodeItem* child);

    void setSelected(bool);

protected:

    virtual void setIcon();

    //Repaints the icon of this folder, by the state of it's children
    // (Full blue - all selected, Half - some selected and some not, Full grey - all deselected)
    void repaintIcon();

    // recursive function
    void recSetSelection(bool);

    //Pointer to book's parent
    BaseNodeItem* mpParent;

    //List containing pointers to the folders children
    QList<BaseNodeItem*> mvChildren;

    QString mTreeDisplayName;

    //Path of the book's source file
    QString mPath;

    //Icon state of this book (or folder)
    IconState mIconState;

    bool mInSearch;
    bool mIsHidden;
    bool mUserBook;
};



#endif // TREEITEM_BASE_H
