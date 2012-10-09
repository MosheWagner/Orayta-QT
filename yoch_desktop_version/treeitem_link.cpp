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

#include "treeitem_link.h"
#include "functions.h"

#include <QDebug>



LinkItem::LinkItem ( BaseNodeItem* parent, QString path, QString name, bool isUserBook ) :
    NodeBook(parent, path, name, isUserBook)
{
    QStringList confs;

    if (!ReadFileToList(path, confs, "UTF-8", false))
    {
        qDebug() << "unable to read confs for " << path;
        return;
    }

    for (QStringList::const_iterator it = confs.begin(); it != confs.end(); ++it)
    {
        if ( it->indexOf("DisplayName") != -1 )
        {
            mTreeDisplayName = it->mid(12);
        }
        else if ( it->indexOf("Link") != -1 )
        {
            int id;
            if(ToNum(it->mid(5), &id))
                mUniqueId = id;
        }
    }
}

NodeBook::Booktype LinkItem::booktype() const
{  return NodeBook::Link;  }

QList<QAction*> LinkItem::menuActions() const
{
    NodeBook* p = bookPtr();
    return ( p ? p->menuActions() : QList<QAction*>() );
}

bool LinkItem::isSearchable() const
{
    NodeBook* p = bookPtr();
    return ( p ? p->isSearchable() : false );
}

bool LinkItem::isFontModifiable() const
{
    NodeBook* p = bookPtr();
    return ( p ? p->isFontModifiable() : false );
}

bool LinkItem::IsInSearch() const
{
    NodeBook* p = bookPtr();
    return ( p ? p->IsInSearch() : false );
}

bool LinkItem::IsHidden() const
{
    NodeBook* p = bookPtr();
    return ( p ? p->IsHidden() : true );
}

bool LinkItem::IsUserCheckable() const
{
    NodeBook* p = bookPtr();
    return ( p ? p->IsUserCheckable() : false );
}

bool LinkItem::IsUserBook() const
{
    return mUserBook;
}

QString LinkItem::getPath() const
{
    NodeBook* p = bookPtr();
    return ( p ? p->getPath() : "" );
}

void LinkItem::changeFont( const QFont& font )
{
    NodeBook* p = bookPtr();
    if (p) p->changeFont(font);
}

QString LinkItem::getLoadableFile() const
{
    NodeBook* p = bookPtr();
    return ( p ? p->getLoadableFile() : "" );
}

NodeBook* LinkItem::bookPtr() const
{  return getBookPtrFromId(getUniqueId());  }
