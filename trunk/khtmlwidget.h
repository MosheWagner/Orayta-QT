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

#ifndef KHTMLWidget_H
#define KHTMLWidget_H


#include "khtml_part.h"
#include "khtmlview.h"
#include <khtml_events.h>
#include "book.h"
#include "functions.h"
#include <QApplication>
#include <QDir>
#include <QMenu>
#include <QAction>
#include <QPoint>
#include <QScrollBar>
#include <QScrollArea>
#include <QClipboard>
#include <dom/html_document.h>


/*
  Simple KHTML based html display widget.
  Inherits KHTMLPart, with all the added quirkes needed for this program.

  Emitts:
    docCreated - When page is (almost) done.
    LinkClicked - when internal link or menu right cliked, or external link clicked
*/


class KHTMLWidget : public KHTMLPart
{
Q_OBJECT
public:
    //Constructor
    explicit KHTMLWidget(QWidget *parent = 0);

    //Loads the given Html page.
    // BEWARE: If called twice too quickly, it will not empty the old contents!
    void load(QUrl url);


    void setBook( Book * );
    Book * book();
    bool ShowNikud;
    bool ShowTeamim;

    QString activeLink();
    QString title();

    qreal textSizeMultiplier();

    QUrl url();

    //Execute the given JS snippet
    void execScript(QString script);

    //Returns the value of given JS variable
    QString getJSVar(QString var);

    void setHtml(QString html);
    QWidget * getQWidget();

    //Call this before any unique search. only after that use "searchText"
    void buildSearch(QString text);
    //Searches for the text given to "buildSearch"
    void searchText( bool back);

    //Zooms the text by the given factor (not percent!)
    void setTextSizeMultiplier(qreal factor);
signals:
    // NOTE: "LinkClicked" is emitted twice for in some cases, so ignore the second time
    void LinkClicked(QUrl);

private slots:
    //Catches mouse events
    void khtmlMousePressEvent(khtml::MousePressEvent * event);
    //Copies the selceted text to the clipboard
    void copyText();

private:
    qreal m_Multiplier;
    Book * myBook;

};

#endif // KHTMLWidget_H
