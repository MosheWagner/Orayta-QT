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

#include "treeitem_html.h"

/////////////////////////////////////////////////////
//           Html Book
/////////////////////////////////////////////////////

HtmlBookItem::HtmlBookItem ( BaseNodeItem* parent, QString path, QString name, bool isUserBook ) :
    NodeBook(parent, path, name, isUserBook)
{
    QTreeWidgetItem::setIcon(0, QIcon(":/Icons/book-html.png"));
}

NodeBook::Booktype HtmlBookItem::booktype() const
{  return Html;  }

// ######### make it searchable ?
bool HtmlBookItem::isSearchable() const
{  return false;  }

