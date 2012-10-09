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

#ifndef TREEITEM_DIRECTORY_H
#define TREEITEM_DIRECTORY_H


#include "treeitem_base.h"

class NodeDirectory : public BaseNodeItem
{
public:
    NodeDirectory(BaseNodeItem *parent, QString name, QString path, bool isUserBook = false);

    virtual Nodetype nodetype() const;
    virtual bool isFontModifiable() const;
    virtual bool isSearchable() const;
    virtual QList<QAction*> menuActions() const;

private:
    void setIcon();
};


#endif // TREEITEM_DIRECTORY_H
