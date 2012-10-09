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

#include "treeitem_directory.h"
#include "functions.h"

#include "booktree.h"

#include <QAction>
#include <QDebug>

NodeDirectory::NodeDirectory(BaseNodeItem* parent, QString name, QString path, bool isUserBook) :
    BaseNodeItem(parent, name, path, isUserBook)
{
    QString filename = path + ".folder";

    QStringList lines;

    //Read the book's conf file file:
    if(!ReadFileToList(filename, lines, "UTF-8"))
    {
        //qDebug() << "missing .folder for " << name;
        return;
    }

    for (QStringList::const_iterator it = lines.begin(); it != lines.end(); ++it)
    {
        if ( it->indexOf("BranchName") != -1)
        {
            mTreeDisplayName = it->mid(11);
        }
    }
}

// recursive (call it after tree is complete)
bool NodeDirectory::isSearchable() const
{
    for (QList<BaseNodeItem*>::const_iterator it = mvChildren.begin(); it != mvChildren.end(); ++it)
        if ( (*it)->isSearchable() ) return true;

    return false;
}

// recursive (call it after tree is complete)
bool NodeDirectory::isFontModifiable() const
{
    for (QList<BaseNodeItem*>::const_iterator it = mvChildren.begin(); it != mvChildren.end(); ++it)
        if ( (*it)->isFontModifiable() ) return true;

    return false;
}

BaseNodeItem::Nodetype NodeDirectory::nodetype() const
{  return Node;  }

QList<QAction*> NodeDirectory::menuActions() const
{
    QList<QAction*> ret = BaseNodeItem::menuActions();

    if (isFontModifiable())
    {
        QAction* changefont = new QAction(QIcon(":/Icons/font.png"), QObject::tr("Change font"), treeWidget());
        QObject::connect(changefont, SIGNAL(triggered()), treeWidget(), SLOT(changeFont()));

        ret << changefont;
    }

    return ret;
}

void NodeDirectory::setIcon()
{
    if (mUserBook)
        QTreeWidgetItem::setIcon(0, QIcon(":/Icons/folder-user.png"));
    else if (mIconState == BLUE)
        QTreeWidgetItem::setIcon(0, QIcon(":/Icons/folder-blue.png"));
    else if (mIconState == GREY)
        QTreeWidgetItem::setIcon(0, QIcon(":/Icons/folder-grey.png"));
    else if (mIconState == HALF)
        QTreeWidgetItem::setIcon(0, QIcon(":/Icons/folder-blue-grey.png"));
}
