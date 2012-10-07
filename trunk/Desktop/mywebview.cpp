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

#include "mywebview.h"

#include <QClipboard>
#include <QMenu>
#include <QDebug>

myWebView::myWebView(QWidget * parent)
{
    setParent(parent);

    //Holds currently selected link
    mActiveLink = "";

    //if this is mobile app we don't use bookDisplayer class.
#ifndef MOBILE
    mBookdisp = qobject_cast<bookDisplayer*>(parent);
#else
    // in mobile version ignore mouse moves
    setMouseTracking(false);
#endif

    /* // don't work
    pageAction(QWebPage::Copy)->setShortcut(QKeySequence::Copy);
    */
    connect(this, SIGNAL(statusBarMessage(QString)), this, SLOT(rememberActiveLink(QString)));

    copyNoSigns = new QAction(QIcon(":/Icons/copy-clean.png"), tr("Copy text only"), this); //TODO: is the ':' sign in ":/Icons/copy-clean.png" currect? or should it be '.' ?
    connect(copyNoSigns, SIGNAL(triggered()), this, SLOT(copyClean()));

    this->page()->setLinkDelegationPolicy(QWebPage::DelegateAllLinks);
}

void myWebView::copyClean()
{
    QString text = removeSigns(page()->selectedText());
    if (!text.isEmpty())
    {
        //Copy to clipboard:
        QClipboard *clipboard = QApplication::clipboard();
        clipboard->setText(text, QClipboard::Clipboard);
    }
}

QVariant myWebView::execScript(QString script)
{
    return page()->mainFrame()->evaluateJavaScript(script);
}

 #ifdef MOBILE
void myWebView::mouseMoveEvent(QMouseEvent *event)
{

        event->ignore();

}
#endif

//void myWebView::mousePressEvent(QMouseEvent *event) {
////    qDebug() << "mouse press: " << event->button() << "type: " << event->type();
//}

//{
//    //Ignore right clicks and emulate a left one instead
//    if (event->button() == Qt::RightButton)
//    {
//        QWebHitTestResult r = page()->mainFrame()->hitTestContent(event->pos());

//        //Click on link
//        if (!r.linkUrl().isEmpty())
//        {
//            //Emulate left click
//            QMouseEvent *e=new QMouseEvent(QEvent::MouseButtonPress, event->pos(), Qt::LeftButton, Qt::LeftButton, Qt::NoModifier);
//            QWebView::mousePressEvent(e);
//        }
//        else
//        {
//            //Display "copy" menu
//            QMenu menu(this);

//            QAction *copy = pageAction(QWebPage::Copy);
//            copy->setText(tr("Copy text"));
//            copy->setIcon(QIcon(":/Icons/edit-copy.png"));

//            menu.addAction(copy);

//            menu.addAction(copyNoSigns);

//            menu.setLayoutDirection(Qt::RightToLeft);
//            menu.exec(mapToGlobal(event->pos()));
//        }
//    }
//    else QWebView::mousePressEvent(event);
//}

//void myWebView::mouseReleaseEvent(QMouseEvent *event)
//{
//    QWebHitTestResult r = page()->mainFrame()->hitTestContent(event->pos());

//    //Ignore right clicks and emulate a left one instead
//    if (event->button() == Qt::RightButton)
//    {
//        //Click on link
//        if (!r.linkUrl().isEmpty())
//        {
//            emit LinkClicked(r.linkUrl());

//            QMouseEvent *e=new QMouseEvent(QEvent::MouseButtonRelease, event->pos(), Qt::LeftButton, Qt::LeftButton, Qt::NoModifier);
//            QWebView::mouseReleaseEvent(e);
//        }
//    }

//    else if (event->button() == Qt::LeftButton)
//    {
//        if (!r.linkUrl().isEmpty())
//        {
//            if ((r.linkUrl().toString().indexOf("!") != -1 ) || (r.linkUrl().toString().indexOf("^") != -1 ))
//            {
//                emit LinkClicked(r.linkUrl());
//            }
//        }

//        QWebView::mouseReleaseEvent(event);
//    }

//}

QString myWebView::activeLink() { return mActiveLink; }


void myWebView::keyPressEvent( QKeyEvent *keyEvent )
{
    //if this is mobile app we don't use bookDisplayer class.
#ifdef MOBILE

//    qDebug() << "cauget key event: " << keyEvent->key();
    switch ( keyEvent->key() )
    {
    case Qt::Key_MediaPrevious:
    case Qt::Key_Explorer:
    case Qt::Key_Meta:
    case Qt::Key_Backspace:
        qDebug()<<"caught back or media";
        keyEvent->ignore();
        return;

    case Qt::Key_Down:
            //scroll down
        this->page()->currentFrame()->scroll(0, 40);
        return;
    case Qt::Key_Up:
        this->page()->currentFrame()->scroll(0, -40);
        return;

   default:
//        qDebug() << "key pressed: " << keyEvent->key();
    QWebView::keyPressEvent(keyEvent);
    return;

    }

#else
qDebug() << "mobile not defined! ";

    //if this is normal book
    if ( mBookdisp && (mBookdisp->book() != NULL) )
    {
        if ( keyEvent->modifiers() == Qt::NoModifier )
        {
            if ( mBookdisp->book()->fileType() == Book::Normal )
            {
                switch ( keyEvent->key() )
                {
                case Qt::Key_Left:
                case Qt::Key_Down:
                    page()->mainFrame()->evaluateJavaScript("paintNext();");
                    return;

                case Qt::Key_Right:
                case Qt::Key_Up:
                    page()->mainFrame()->evaluateJavaScript("paintPrevious();");
                    return;

                case Qt::Key_Backspace:  //Ignore backspace clicks
                    return;
                }
            }
        }
        else if ( keyEvent->modifiers() == Qt::AltModifier )
        {
            if ( mBookdisp->book()->fileType() == Book::Html )
            {
                switch ( keyEvent->key() )
                {
                case Qt::Key_Left:
                    back();
                    return;

                case Qt::Key_Right:
                    forward();
                    return;
                }
            }
        }
        else if ( keyEvent->modifiers() == Qt::CTRL )
        {
            switch ( keyEvent->key() )
            {
            case Qt::Key_C:
                copyClean();
                return;
            }
        }
    }


    // default
    QWebView::keyPressEvent(keyEvent);
#endif //#ifdef MOBILE
}

#ifdef MOBILE
void myWebView::keyReleaseEvent(QKeyEvent *keyEvent ){

//    qDebug() << "cauget key event: " << keyEvent->key();
    switch ( keyEvent->key() )
    {
    case Qt::Key_MediaPrevious:
    case Qt::Key_Explorer:
    case Qt::Key_Meta:
    case Qt::Key_Backspace:
        qDebug()<<"caught back or media";
        keyEvent->ignore();
        return;

   default:
    QWebView::keyReleaseEvent(keyEvent);
    return;

    }
}
#endif

//Set a busy cursor before actually resizing the text, and restores it by the end
void myWebView::setTextSizeMultiplier(qreal factor)
{
    QApplication::setOverrideCursor(Qt::WaitCursor);

    //Resize the text
    QWebView::setTextSizeMultiplier(factor);

    QApplication::restoreOverrideCursor();
}

void myWebView::rememberActiveLink(QString message)
{
    mActiveLink = message;
    //?
    mActiveLink = url().toString();
    qDebug() << "url: " << mActiveLink;

}

//Returns the value of given JS variable
QString myWebView::getJSVar(QString var)
{
    QVariant val = page()->mainFrame()->evaluateJavaScript("ReturnValue(" + var + ");");

    //Return the requested var
    return val.toString();
}
