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


/*
This is a slightly modified webview widget, allowing me to take control over it's mouseEvents.

Not extremly important, but it isn't too complicated either.
*/

#ifndef MYWEBVIEW_H
#define MYWEBVIEW_H

#include "functions.h"
#include "mainwindow.h"

class myWebView : public QWebView
{
    //Very important. Without it, it won't create signalls or slots.
    Q_OBJECT
    
public:
    myWebView(QWidget * parent);

    QString activeLink();

    void execScript(QString script);
    QString getJSVar(QString var);

public slots:
    void setTextSizeMultiplier(qreal factor);

private slots:
    void mousePressEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);

    void keyPressEvent( QKeyEvent *keyEvent );

    void rememberActiveLink(QString);

signals:
    void LinkClicked(QUrl);
    void ToggleSearchBar();

private:
    QString mActiveLink;

};

#endif // MYWEBVIEW_H
