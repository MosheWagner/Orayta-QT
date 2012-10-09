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

#ifndef TREEITEM_LINK_H
#define TREEITEM_LINK_H


#include "treeitem_base_book.h"

class LinkItem : public NodeBook
{
public:
    LinkItem ( BaseNodeItem *parent, QString path, QString name, bool isUserBook );

    Booktype booktype() const;
    virtual bool isSearchable() const;
    virtual bool isFontModifiable() const;

    virtual bool IsInSearch() const;
    virtual bool IsHidden() const;
    virtual bool IsUserCheckable() const;
    virtual bool IsUserBook() const;

    virtual QString getPath() const;

    //virtual void setCheckState(IconState state);
    virtual QList<QAction*> menuActions() const;

    virtual void changeFont( const QFont& );

    virtual QString getLoadableFile() const;

    NodeBook* bookPtr() const;

private:

};

#endif // TREEITEM_LINK_H
