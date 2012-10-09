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
* Original Author: Moshe Wagner. <moshe.wagner@gmail.com>
*/

#include "treeitem_base.h"
#include "treeitem_orayta.h"
#include "treeitem_link.h"

#include "bookdisplayer.h"
#include "mainwindow.h"
#include "bookviewinterface.h"
#include "bookview_orayta.h"


//PDF TODOs:
//TODO: Make sure pdf mode is safe.

//Html TODOs:
//Auto detection on current node in view?
//Add comment s via script?

#include <QDebug>

BookDisplayer::BookDisplayer(QWidget * parent, QTabWidget * tabviewptr) :
    MW(qobject_cast<MainWindow *>(parent)),
    TW(tabviewptr),
    vbox(new QVBoxLayout(this)),
/*
    vbox(new QStackedLayout(this)),
    waitLbl(new QLabel(this)),
    waitAnimation(new QMovie(":/Images/Wait.gif")),
*/
    myBook(0),
    mBookView(0)
{
    //Create new vbox
    vbox->setContentsMargins(0,0,0,0);

//    vbox->setAlignment(Qt::AlignCenter);
//    vbox->setStackingMode(QStackedLayout::StackAll);

//    waitAnimation->setCacheMode(QMovie::CacheAll);
//    /*
//    QTimer* timer = new QTimer;
//    timer->start(100);
//    QObject::connect(timer, SIGNAL(timeout()), waitAnimation, SLOT(jumpToNextFrame()), Qt::DirectConnection);
//    */
//    waitLbl->setAlignment(Qt::AlignCenter);
//    waitLbl->setMovie(waitAnimation);

//    vbox->addWidget(waitLbl);
}

BookDisplayer::~BookDisplayer()
{
//    waitAnimation->stop();
//    waitAnimation->deleteLater();

    deleteLater();
}

void BookDisplayer::resetBookView()
{
    if (mBookView)
    {
        vbox->removeWidget(mBookView->widget());
        mBookView->widget()->deleteLater();
        mBookView = 0;
    }
}

// Be carefull !!!
void BookDisplayer::loadBook( NodeBook* book )
{
    if (book->booktype() == NodeBook::Link)
        book = dynamic_cast<LinkItem*>(book)->bookPtr();

    if (!book) return; // sanity check

    if (mBookView && mBookView->booktype() != book->booktype())
        resetBookView();

    if (!mBookView)
    {
        mBookView = BookViewFactory(this, book->booktype());
        vbox->addWidget(mBookView->widget());
//        vbox->setCurrentWidget(mBookView->widget());
    }

    MW->adjustVtabCornerLayout(this);
    mBookView->loadBook(book);

    // ########## must ensure all is clean...
    // erase tabWidget associed with previous book
    if (myBook != NULL) myBook->setTabWidget( 0 );
    myBook = book;
    myBook->setTabWidget(this);
}

void BookDisplayer::loadSearchPage(const QUrl& url)
{
    resetBookView();
    // erase tabWidget associed with previous book
    if (myBook != NULL) myBook->setTabWidget( 0 );

    mBookView = BookViewFactory(this, NodeBook::Orayta);
    vbox->addWidget(mBookView->widget());
    MW->adjustVtabCornerLayout(this);

    OraytaBookView* oryView = dynamic_cast<OraytaBookView*>(mBookView);
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
{  if (mBookView) mBookView->setInternalLocation(location);  }

void BookDisplayer::print() const
{  if (mBookView) mBookView->print();  }

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


//void BookDisplayer::showWaitAnimation()
//{
//    /*
//    vbox->activate();
//    vbox->update();
//    */
//    vbox->setCurrentWidget(waitLbl);
//    //waitLbl->activateWindow();

//    //if (mBookView) mBookView->widget()->hide();

//    //waitAnimation->start();

//    qDebug() << "entry in show anim" << vbox->currentIndex();
//}

//void BookDisplayer::hideWaitAnimation()
//{
//    //waitAnimation->stop();

//    if (mBookView)
//    {
//        vbox->setCurrentWidget(mBookView->widget());
//        //mBookView->widget()->show();
//    }

//    qDebug() << "entry in hide anim" << vbox->currentIndex();
//}
