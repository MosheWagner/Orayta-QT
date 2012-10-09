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

#include "treeitem_base_book.h"
#include "treeitem_html.h"
#include "treeitem_orayta.h"
#include "treeitem_link.h"
#ifdef POPPLER
    #include "treeitem_pdf.h"
#endif

#include "bookdisplayer.h"
#include "booktree.h"

//////////////////////////////////////////////////////////////
//                       Node Book
//////////////////////////////////////////////////////////////


// static member function
NodeBook* NodeBook::BookFactory(BaseNodeItem* parent, QString path, QString name, NodeBook::Booktype ft, bool isUserBook)
{
    switch (ft)
    {
    case NodeBook::Orayta :
        return new OraytaBookItem(parent, path, name, isUserBook);
    case NodeBook::Html :
        return new HtmlBookItem(parent, path, name, isUserBook);
#ifdef POPPLER
    case NodeBook::Pdf :
        return new PdfBookItem(parent, path, name, isUserBook);
#endif
    case Link:
        return new LinkItem(parent, path, name, isUserBook);
    default:
        qFatal("Unable to create Book for this booktype");
    }
}



NodeBook::NodeBook( BaseNodeItem* parent, QString path, QString name, bool isUserBook ) :
    BaseNodeItem(parent, name, path, isUserBook),
    mTabWidget(0),
    mUniqueId(-1),
    mRandomId(-1)
{}

/*
NodeBook::~NodeBook()
{
}
*/

QList<QAction*> NodeBook::menuActions() const
{
    QList<QAction*> ret = BaseNodeItem::menuActions();

    QAction* openbook = new QAction(QIcon(":/Icons/book-blue.png"), QObject::tr("Open book"), treeWidget());
    QAction* openbooknewtab = new QAction(QIcon(":/Icons/tab-new.png"), QObject::tr("Open in new tab"), treeWidget());

    QObject::connect(openbook, SIGNAL(triggered()), treeWidget() , SLOT(openSelectedBook()));
    QObject::connect(openbooknewtab, SIGNAL(triggered()), treeWidget() , SLOT(openSelectedBookInNewTab()));

    ret << openbook << openbooknewtab;

    if (mUserBook)
    {
        QAction* deleteBook = new QAction(QIcon(":/Icons/edit-delete.png"), QObject::tr("Delete book"), treeWidget());
        QObject::connect(deleteBook, SIGNAL(triggered()), treeWidget(), SLOT(deleteSelectedBook()));
        ret << deleteBook;
    }

    return ret;
}

BaseNodeItem::Nodetype NodeBook::nodetype() const
{  return Leaf;  }

// implementation by default
bool NodeBook::isSearchable() const
{  return false;  }

// implementation by default
bool NodeBook::isFontModifiable() const
{  return false;  }

BookDisplayer* NodeBook::tabWidget() const
{  return mTabWidget;  }

// default implementation
QString NodeBook::getLoadableFile() const
{  return mPath;  }

int NodeBook::getUniqueId() const
{  return mUniqueId != -1 ? mUniqueId : mRandomId; }

int NodeBook::getRealUniqueId() const
{  return mUniqueId;  }

void NodeBook::setRandomUniqueId(int id)
{  mRandomId = id;  }

void NodeBook::setTabWidget(BookDisplayer* w)
{  mTabWidget = w;  }

void NodeBook::AddConfs()
{}

