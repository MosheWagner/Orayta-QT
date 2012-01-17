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
* Author: Moshe Wagner. <moshe.wagner@gmail.com>
*/

#include "treeitem_base.h"
#include "treeitem_orayta.h"

#include "bookdisplayer.h"
#include "mainwindow.h"
#include "bookviewinterface.h"
#include "bookview_orayta.h"


//PDF TODOs:
//TODO: Make sure pdf mode is safe.

//Html TODOs:
//Auto detection on current node in view?
//Add comment s via script?



BookDisplayer::BookDisplayer(QWidget * parent, QTabWidget * tabviewptr) :
    MW(qobject_cast<MainWindow *>(parent)),
    TW(tabviewptr),
    myBook(0),
    mBookView(0)
{
    //Create new vbox
    vbox = new QVBoxLayout(this);
    vbox->setContentsMargins(0,0,0,0);
}

BookDisplayer::~BookDisplayer()
{
    deleteLater();
}

void BookDisplayer::resetBookView()
{
    if (mBookView)
    {
        // le dynamic_cast est employé ici car BookView est une classe interface qui n'est pas un QWidget.
        QWidget* w = dynamic_cast<QWidget*>(mBookView);
        vbox->removeWidget(w);
        w->deleteLater();
        mBookView = 0;
    }
}

// Be carefull !!!
void BookDisplayer::loadBook( const NodeBook* book )
{
    if (!book)  // sanity check
        return;

    if (mBookView && mBookView->booktype() != book->booktype())
        resetBookView();

    if (!mBookView)
    {
        mBookView = BookViewFactory(this, book->booktype());
        vbox->addWidget(dynamic_cast<QWidget*>(mBookView));
    }

    // erase tabWidget associed with previous book
    if (myBook != NULL) myBook->setTabWidget( 0 );
    myBook = (NodeBook*)book;

    if (book->booktype() == NodeBook::Pdf)
    {
        setTitle(book->getTreeDisplayName());
    }

    MW->adjustVtabCornerLayout(this);
    mBookView->loadBook(book);

    // ############## en fait, il faudrait vérifier que tout s'est bien passé
    myBook->setTabWidget(this);
}

void BookDisplayer::loadSearchPage(const QUrl& url)
{
    resetBookView();
    mBookView = BookViewFactory(this, NodeBook::Orayta);
    OraytaBookView* oryView = dynamic_cast<OraytaBookView*>(mBookView);
    vbox->addWidget(oryView);
    oryView->load(url);
}

BookViewInterface* BookDisplayer::bookView() const
{  return mBookView;  }

/////////////////////////////////////////////////////////
//               wrappers functions

void BookDisplayer::ZoomIn() const
{  if (mBookView) mBookView->ZoomIn();  }

void BookDisplayer::ZoomOut() const
{  if (mBookView) mBookView->ZoomOut();  }

void BookDisplayer::normalZoom() const
{  if (mBookView) mBookView->normalZoom();  }

void BookDisplayer::jumpToTop() const
{  if (mBookView) mBookView->jumpToTop();  }

// ################ ce serait mieux de renvoyer un bool pour dire si on a trouvé ou non,
// ca permettrait de modifier la couleur du lineEdit en amont en fonction
void BookDisplayer::searchText(const QString& text, bool backwards) const
{  if (mBookView) mBookView->searchText(text, backwards);  }

void BookDisplayer::setInternalLocation(const QString& location) const
{
    if (!mBookView) return;
    mBookView->setInternalLocation(location);
}

void BookDisplayer::reload()
{
    if (mBookView)  // sanity check
        mBookView->reload();
}

NodeBook * BookDisplayer::book() const
{   return myBook;  }

MainWindow * BookDisplayer::mainWnd() const
{  return MW;  }

bool BookDisplayer::hasNikud() const
{
    OraytaBookView* oview = dynamic_cast<OraytaBookView*>(mBookView);
    return oview ? oview->hasNikud() : false;
}

bool BookDisplayer::hasTeamim() const
{
    OraytaBookView* oview = dynamic_cast<OraytaBookView*>(mBookView);
    return oview ? oview->hasTeamim() : false;
}

QWidget* BookDisplayer::additionalButtons() const
{  return mBookView ? mBookView->additionalButtons() : NULL;  }

void BookDisplayer::setTitle(QString title)
{   //qDebug() << " set title : " << title;
    if (title == "" && myBook)
        title = myBook->getTreeDisplayName();

    int index = TW->indexOf(this);
    if (index != -1)
    {
        TW->setTabText(index, title);
    }
    else qDebug() << "index not found";
}

NodeBook::Booktype BookDisplayer::displayedBookType()
{  return mBookView ? mBookView->booktype() : NodeBook::Unkown;  }
