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

myWebView::myWebView(QWidget * parent)
{
    setParent(parent);

    //Holds currently selected link
    mActiveLink = "";

    mBookdisp = qobject_cast<bookDisplayer*>(parent);

    /* // don't work
    pageAction(QWebPage::Copy)->setShortcut(QKeySequence::Copy);
    */
    connect(this, SIGNAL(statusBarMessage(QString)), this, SLOT(rememberActiveLink(QString)));

    copyNoSigns = new QAction(QIcon(":/Icons/copy-clean.png"), tr("Copy text only"), this);
    connect(copyNoSigns, SIGNAL(triggered()), this, SLOT(copyClean()));
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

void myWebView::mousePressEvent(QMouseEvent *event)
{
    //Ignore right clicks and emulate a left one instead
    if (event->button() == Qt::RightButton)
    {
        QWebHitTestResult r = page()->mainFrame()->hitTestContent(event->pos());

        //Click on link
        if (!r.linkUrl().isEmpty())
        {
            //Emulate left click
            QMouseEvent *e=new QMouseEvent(QEvent::MouseButtonPress, event->pos(), Qt::LeftButton, Qt::LeftButton, Qt::NoModifier);
            QWebView::mousePressEvent(e);
        }
        else
        {
            //Display "copy" menu
            QMenu menu(this);

            QAction *copy = pageAction(QWebPage::Copy);
            copy->setText(tr("Copy text"));
            copy->setIcon(QIcon(":/Icons/edit-copy.png"));

            menu.addAction(copy);

            menu.addAction(copyNoSigns);

            menu.setLayoutDirection(Qt::RightToLeft);
            menu.exec(mapToGlobal(event->pos()));
        }
    }
    else QWebView::mousePressEvent(event);
}

void myWebView::mouseReleaseEvent(QMouseEvent *event)
{
    QWebHitTestResult r = page()->mainFrame()->hitTestContent(event->pos());

    //Ignore right clicks and emulate a left one instead
    if (event->button() == Qt::RightButton)
    {
        //Click on link
        if (!r.linkUrl().isEmpty())
        {
            emit LinkClicked(r.linkUrl());

            QMouseEvent *e=new QMouseEvent(QEvent::MouseButtonRelease, event->pos(), Qt::LeftButton, Qt::LeftButton, Qt::NoModifier);
            QWebView::mouseReleaseEvent(e);
        }
    }

    else if (event->button() == Qt::LeftButton)
    {
        if (!r.linkUrl().isEmpty())
        {
            if ((r.linkUrl().toString().indexOf("!") != -1 ) || (r.linkUrl().toString().indexOf("^") != -1 ))
            {
                emit LinkClicked(r.linkUrl());
            }
        }

        QWebView::mouseReleaseEvent(event);
    }

}

QString myWebView::activeLink() { return mActiveLink; }


void myWebView::keyPressEvent( QKeyEvent *keyEvent )
{
    //if this is normal book
    if ( mBookdisp->book() != NULL )
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
}


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
}

//Returns the value of given JS variable
QString myWebView::getJSVar(QString var)
{
    QVariant val = page()->mainFrame()->evaluateJavaScript("ReturnValue(" + var + ");");

    //Return the requested var
    return val.toString();
}
