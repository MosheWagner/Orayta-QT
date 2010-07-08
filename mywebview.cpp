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

myWebView::myWebView(QWidget * parent)
{
    setParent(parent);

    //Holds currently selected link
    mActiveLink = "";

    connect(this, SIGNAL(statusBarMessage(QString)), this, SLOT(rememberActiveLink(QString)));

}

void myWebView::execScript(QString script)
{
    page()->mainFrame()->evaluateJavaScript(script);
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

            QMouseEvent *e=new QMouseEvent(QEvent::MouseButtonRelease, event->pos() , Qt::LeftButton, Qt::LeftButton, Qt::NoModifier);
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
    if ( keyEvent->key() == Qt::Key_Left || keyEvent->key() == Qt::Key_Down)
    {
        page()->mainFrame()->evaluateJavaScript("paintNext();");
        return;
    }
    if ( keyEvent->key() == Qt::Key_Right || keyEvent->key() == Qt::Key_Up)
    {
        page()->mainFrame()->evaluateJavaScript("paintPrevious();");
        return;
    }

    //Ignore backspace clicks
    if ( keyEvent->key() != Qt::Key_Backspace) QWebView::keyPressEvent(keyEvent);

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

QString myWebView::documentSource()
{
    return page()->mainFrame()->toPlainText();
}

//Returns the value of given JS variable
QString myWebView::getJSVar(QString var)
{
    QVariant val = page()->mainFrame()->evaluateJavaScript("ReturnValue(" + var + ");");

    //Return the requested var
    return val.toString();
}
