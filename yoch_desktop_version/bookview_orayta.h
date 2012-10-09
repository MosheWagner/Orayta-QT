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

#ifndef BOOKVIEW_ORAYTA_H
#define BOOKVIEW_ORAYTA_H

#include "bookview_basehtml.h"


#include <QSignalMapper>
#include <QToolButton>
#include <QHBoxLayout>


class OraytaBookItem;

class OraytaBookView : public BaseHtmlView
{
    Q_OBJECT

public:
    OraytaBookView(BookDisplayer * parent);

    virtual NodeBook::Booktype booktype() const;
    virtual void loadBook(const NodeBook*);
    virtual void searchText(const QString& text, bool backwards);
    virtual void jumpToTop();
    virtual void setInternalLocation(const QString& location);

    void reloadAtSameLocation();

    virtual bool hasNikud() const;
    virtual bool hasTeamim() const;
/*
    virtual bool areNikudShown() const;
    virtual bool areTeamimShown() const;
*/
    virtual void showNikud(bool show);
    virtual void showTeamim(bool show);

    virtual void reload();

    virtual QWidget* additionalButtons();

    void highlight( const QRegExp& );
    void unhighlight();

    QString activeLink();

signals:
    void externalLink(QString);

public slots:
    void openCommentDialog(QString);

private slots:
    void on_linkClicked( const QUrl& );
    void copyClean();
    void mousePressEvent( QMouseEvent* );
    void keyPressEvent( QKeyEvent* );
    void removeComment( QString );
    void addCommentAtPosition(QString, QString);
    void reloadSlot();

private:
    OraytaBookItem* mInternalBook;

    int mSearchPos;

    QSignalMapper *msignalMapper;
    QSignalMapper *csignalMapper;
    QSignalMapper *dsignalMapper;

    QAction *copyNoSigns;
    QAction *comment;
    QAction *delcomment;
    QAction *mark;

    QWidget* mAdditionalButtons;
    QHBoxLayout* mAdditionalLayout;
    QToolButton* reloadBtn;

    bool shownikud;
    bool showteamim;
};

#endif // BOOKVIEW_ORAYTA_H
