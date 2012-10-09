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


#ifndef BOOKTREE_H
#define BOOKTREE_H


#include "treeitem_base.h"
#include "treeitem_base_book.h"
#include "treeitem_orayta.h"

#include <QTreeWidget>
#include <QAction>
#include <QPointer>
#include <QList>
#include <QMap>

extern QHash<QString, int> TitleToId;

class BookTree : public QTreeWidget
{
    Q_OBJECT

public:
    BookTree( QWidget * parent = 0 );
    ~BookTree();

    BookList& booklist();

    NodeBook* findBookById(int id);

    BookList BooksInSearch();

    void CheckUid();

    void saveBooksConfs();
    void restoreBooksConfs();

    BaseNodeItem* root();
    BaseNodeItem* userRoot();
    BaseNodeItem* tanachRoot();
    //QHash<QString, int> titleToId();

    /* ######### temporary
    void saveMixedInfos();
    NodeBook* getBoookByPath(const QString& path);
    */
    static QList<OraytaBookItem*> getBooksChildren( BaseNodeItem* parent, bool inSearch = false );

public slots:
    void updateTree();
    void displayContextMenu (QPoint pos);
    void openSelectedBook();
    void openSelectedBookInNewTab();
    void deleteSelectedBook();
    void changeFont();

signals:
    void openBookRequested(NodeBook*, bool);

private:
    void init();
    // Recursivly add all book files to the list
    void addAllBooks (BaseNodeItem* parent);

    void addNewBookid (NodeBook* book);

    void keyPressEvent( QKeyEvent *keyEvent );

    BaseNodeItem* m_root;
    BaseNodeItem* m_userRoot;
    BaseNodeItem* m_tanachRoot;
    BaseNodeItem* m_michnaRoot;
    BaseNodeItem* m_chassRoot;

    BookList mBookList;

    QHash<int, NodeBook*> mMapId;

};


#endif // BOOKTREE_H
