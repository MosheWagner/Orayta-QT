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

#include "khtmlwidget.h"

KHTMLWidget::KHTMLWidget(QWidget *parent) : KHTMLPart(parent)
{
    m_Multiplier = 1.0;

    myBook = NULL;

    //Invert scrollview so it's correct for hebrew
    view()->horizontalScrollBar()->setInvertedAppearance(true);
}

//Returns the value of given JS variable
QString KHTMLWidget::getJSVar(QString var)
{
    //Put the "window.status" in a temp variable
    QString str = "";
    str += "var tmp = window.status;";

    //Put the requestet var in the "window.status"
    str += "window.status = " + var + ";";
    executeScript(str);

    //Remember the "window.status"
    QString res = jsStatusBarText();

    //Restore the "window.status"
    executeScript("window.status =  tmp;");

    //Return the requested var
    return res;
}

QString KHTMLWidget::title()
{
    DOM::HTMLDocument document = this->htmlDocument();
    DOM::DOMString title = document.title();

    if (title != "") return title.string();

    if (myBook != NULL) return myBook->getNormallDisplayName();

    return "";
}

//Execute the given JS snippet
void KHTMLWidget::execScript(QString script)
{
    executeScript(script);
}


//Catches mouse events
// NOTE: "LinkClicked" is emitted twice for in some cases, so ignore the second time
void KHTMLWidget::khtmlMousePressEvent(khtml::MousePressEvent * event)
{
    QUrl r = QUrl(event->url().string());

    if (event->qmouseEvent()->button() == Qt::RightButton)
    {
        //Right clicks on text should allow you to copy it
        if (r.toString() == "")
        {
            //Display "copy" menu
            QMenu menu(getQWidget());

            QAction *copy = new QAction(getQWidget());
            copy->setText("העתק טקסט");
            copy->setIcon(QIcon(":/Icons/edit-copy.png"));

            connect(copy, SIGNAL(triggered()), this, SLOT(copyText()));

            menu.addAction(copy);

            menu.setLayoutDirection(Qt::RightToLeft);
            menu.exec(event->qmouseEvent()->globalPos());
        }
        //On links the "LinkClicked" signal in emiited
        else
        {
            //if ((r.toString().indexOf("!") != -1 ) || (r.toString().indexOf("^") != -1 ))
            //{
                emit LinkClicked(r);
           // }
        }
    }
    //Left clicks emit "LinkClicked" if they are on externall links, or handled normally.
    else
    {
        if (r.toString() != "" && r.toString().indexOf("!") != -1) emit LinkClicked(r);

        KHTMLPart::khtmlMousePressEvent(event);
    }
}

//Copies the selceted text to the clipboard
void KHTMLWidget::copyText()
{
    QClipboard *clipboard = QApplication::clipboard();
    clipboard->setText(selectedText());
}

//Catches keyboard events
void KHTMLView::keyPressEvent (QKeyEvent * keyEvent)
{
    if ( keyEvent->key() == Qt::Key_Left || keyEvent->key() == Qt::Key_Down)
    {
        part()->executeScript("paintNext();");
        return;
    }
    if ( keyEvent->key() == Qt::Key_Right || keyEvent->key() == Qt::Key_Up)
    {
        part()->executeScript( "paintPrevious();");
        return;
    }

    if (keyEvent->modifiers() == Qt::CTRL)
    {
        //If this key is "C" or "X":
        if ( keyEvent->key() == Qt::Key_C || keyEvent->key() == Qt::Key_X)
        {
            QClipboard *clipboard = QApplication::clipboard();
            clipboard->setText(part()->selectedText());
        }
    }
}

//Zooms the text by the given factor (not percent!)
//(Sets a busy cursor before actually resizing the text, and restores it by the end)
void KHTMLWidget::setTextSizeMultiplier(qreal factor)
{
    m_Multiplier = factor;

    QApplication::setOverrideCursor(Qt::WaitCursor);

    //Resize the text
    setFontScaleFactor(factor * 100);

    QApplication::restoreOverrideCursor();
}

qreal KHTMLWidget::textSizeMultiplier() {return m_Multiplier; }

void KHTMLWidget::setBook( Book * book)
{
    myBook = book;
}

QString KHTMLWidget::activeLink() { return jsStatusBarText(); }

Book * KHTMLWidget::book() { return myBook; }

QUrl KHTMLWidget::url() { return QUrl(baseURL()); }

//Loads the given Html page.
// BEWARE: If called twice too quickly, it will not empty the old contents!
    //NOTE: Two phase loading will not work here!!!
void KHTMLWidget::load(QUrl url)
{
    QString nurl = url.toString();

    if (!nurl.startsWith("file://")) nurl = "file://" + nurl;

    openUrl(KUrl(nurl));
}

void KHTMLWidget::setHtml(QString html)
{
    begin();
    write (html);
    end();
}

QWidget * KHTMLWidget::getQWidget()
{
     return (QWidget *) view();
}

//Call this before any unique search. only after that use "searchText"
void KHTMLWidget::buildSearch(QString text)
{
     if (text != "") findText(text, FindNoPopups , NULL, NULL);
}
//Searches for the text given to "buildSearch"
void KHTMLWidget::searchText(bool back)
{
    findTextNext(back);
}
