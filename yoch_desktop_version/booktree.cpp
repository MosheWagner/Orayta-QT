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
* First Author: Moshe Wagner. <moshe.wagner@gmail.com>
* Contributor: Yoch Melka. <yoch.melka@gmail.com>
*/

#include "booktree.h"
#include "treeitem_directory.h"
#include "treeitem_orayta.h"
#include "settings.h"
#include "functions.h"


#include <QObject>
#include <QtCore>
#include <QDir>
#include <QFileInfoList>
#include <QFileInfoListIterator>
#include <QMessageBox>
#include <QMenu>
#include <QFontDialog>
#include <QKeyEvent>
#include <QDebug>


static void recBuild( BaseNodeItem* parentNode )
{
    parentNode->setText(0, parentNode->getTreeDisplayName());
    if (parentNode->isSearchable())
    {
        //qDebug() << parentNode->getTreeDisplayName() << " is searchable";
        parentNode->setFlags( Qt::ItemIsSelectable | Qt::ItemIsEnabled | Qt::ItemIsUserCheckable );
    }
    else
    {
        //qDebug() << parentNode->getTreeDisplayName() << " isn't' searchable";
        parentNode->setFlags( Qt::ItemIsSelectable | Qt::ItemIsEnabled );
    }
    parentNode->setCheckState(parentNode->getIconState());      //set the checkstate and icon

    for (QList<BaseNodeItem*>::const_iterator it = parentNode->getChildren().begin();
                                              it != parentNode->getChildren().end();
                                            ++it)
    {
        if ((*it)->IsHidden())
            continue;
        parentNode->addChild( *it );
        recBuild( *it );
    }
}

void BookTree::init()
{
    // build the book tree
    addAllBooks (m_root);
    addAllBooks (m_userRoot);

    // Only after alls books loaded, check alls uids
    CheckUid();

    recBuild(m_root);
    recBuild(m_userRoot);

    invisibleRootItem()->addChild(m_root);       //addTopLevelItem(root());
    invisibleRootItem()->addChild(m_userRoot);   //addTopLevelItem(userRoot());

    root()->setExpanded(true);
}

BookTree::BookTree( QWidget * parent ) :
    QTreeWidget(parent),
    m_root(new NodeDirectory(NULL, tr("Orayta Library"), BOOKPATH)),
    m_userRoot(new NodeDirectory(NULL, tr("User Library"), USERPATH + "Books", true)),
    m_tanachRoot(NULL),
    mBookList(BookList()),
    mMapId(QMap<int, NodeBook*>())
{
    init();
/*
    openbook = new QAction(QIcon(":/Icons/book-blue.png"), tr("Open book"), this);
    openbooknewtab = new QAction(QIcon(":/Icons/tab-new.png"), tr("Open in new tab"), this);
    deleteBook = new QAction(QIcon(":/Icons/edit-delete.png"), tr("Delete book"), this);
    changefont = new QAction(QIcon(":/Icons/font.png"), tr("Change font"), this);

    // connect context menu for treeView
    connect(openbook, SIGNAL(triggered()), this , SLOT(openSelectedBook()));
    connect(openbooknewtab, SIGNAL(triggered()), this , SLOT(openSelectedBookInNewTab()));
    connect(deleteBook, SIGNAL(triggered()), this, SLOT(deleteSelectedBook()));
    connect(changefont, SIGNAL(triggered()), this , SLOT(changeFont()));
*/
    connect(this, SIGNAL(customContextMenuRequested(QPoint)), this, SLOT(displayContextMenu(QPoint)));
}

BookTree::~BookTree()
{
    delete m_root;
    delete m_userRoot;
}


// Recursivly add all book files to the list
void BookTree::addAllBooks (BaseNodeItem* parent)
{
    bool isUserBooks = parent->IsUserBook();
    QString dirpath = parent->getPath();

    QDir cdir(absPath(dirpath));

    //Get all files in this dir
    //QStringList filter; filter << "*.obk" << "*.pdf" << "*.link" << "*.html" << "*.htm";
    QFileInfoList list = cdir.entryInfoList(QDir::AllEntries | QDir::NoSymLinks | QDir::NoDotAndDotDot,
                                            QDir::Name);

    for (int i=0; i < list.size(); i++)
    {
        NodeBook::Booktype ft = NodeBook::Unkown;
        bool is_dir = false;

        if ( list[i].isDir() && list[i].fileName().indexOf("Pics") == -1 )
            is_dir = true;
        else if (list[i].fileName().endsWith(".obk", Qt::CaseInsensitive))
            ft = NodeBook::Orayta;
        else if (list[i].fileName().endsWith(".html", Qt::CaseInsensitive) || list[i].fileName().endsWith(".htm", Qt::CaseInsensitive))
            ft = NodeBook::Html;
#ifdef POPPLER
        else if (list[i].fileName().endsWith(".pdf", Qt::CaseInsensitive))
            ft = NodeBook::Pdf;
#endif
        else if (list[i].fileName().endsWith(".link", Qt::CaseInsensitive))
            ft = NodeBook::Link;
        else
            ft = NodeBook::Unkown;

        if ( is_dir || ft != NodeBook::Unkown )
        {
            QString Name = list[i].fileName();

            //Create BookListItem
            BaseNodeItem* b;
            if ( is_dir )
            {
                if (!isUserBooks)
                    Name = Name.replace(QRegExp("^[0-9 ]*"), "").replace("_", " ").trimmed();

                b = new NodeDirectory(parent, Name, list[i].absoluteFilePath(), isUserBooks);

                //if (Name == "מקרא" && !m_tanachRoot)
                if (Name == "mkra" && !m_tanachRoot)
                    m_tanachRoot = b;

                // recursively add children
                addAllBooks(b);
            }
            else
            {
                NodeBook* _b = NodeBook::BookFactory(parent, list[i].absoluteFilePath(), Name, ft, isUserBooks);

                if (ft != NodeBook::Link)
                {
                    //Add this book to the booklist
                    mBookList.push_back(_b);

                    // after that, set uniqueId in map
                    addNewBookid(_b);
                }

                b = _b;
            }

            //Tell the parent it has a new child
            parent->add_child(b);
        }
    }
}

void BookTree::addNewBookid (NodeBook* book)
{
    int id = book->getUniqueId();
    if (id == -1)
        return;

    if ( mMapId.contains(id) )
        qWarning() << "  same id(" << id << ") for " << book->getTreeDisplayName();
    else
        mMapId[id] = book;
}

//Return pointer to the book with the given UniqueId - in the book list
//  (Returns NULL if it dosen't exist)
NodeBook* BookTree::findBookById(int id)
{
    if ( mMapId.contains(id) )
        return mMapId[id];
    qDebug() << " id " << id << "not found !";
    return NULL;
}

// ###### utiliser l'arbre ?
BookList BookTree::BooksInSearch (void)
{
    BookList ret;

    for ( BookList::const_iterator it = mBookList.begin() ; it < mBookList.end(); ++it )
        if ((*it)->IsInSearch())
            ret.push_back(*it);

    return ret;
}


void BookTree::CheckUid()
{
    for ( BookList::const_iterator it = mBookList.begin(); it != mBookList.end(); ++it )
    {
        int id = (*it)->getUniqueId();
        if ( id == -1 && (*it)->isSearchable() )
        {
            int randomId;
            do {
                randomId = rand();
            } while ( mMapId.contains( randomId ) );

            (*it)->setRandomUniqueId( randomId );
            addNewBookid ( *it );
        }
    }
}

BaseNodeItem* BookTree::root()
{  return m_root;  }

BaseNodeItem* BookTree::userRoot()
{  return m_userRoot;  }

BaseNodeItem* BookTree::tanachRoot()
{  return m_tanachRoot;  }

BookList& BookTree::booklist()
{  return mBookList;  }


void BookTree::saveBooksConfs()
{
    //Save books' settings
    QList<OraytaConfs> confs_lst;

    for (BookList::const_iterator it = mBookList.begin(); it != mBookList.end(); ++it)
    {
        if ((*it)->booktype() == NodeBook::Orayta && (*it)->getRealUniqueId() != -1)
        {
            confs_lst << dynamic_cast<OraytaBookItem*>(*it)->publicConfs();

            //renew_confs(dynamic_cast<OraytaBookItem*>(*it));
        }
    }

    QFile file(USERPATH + "orayta.conf");
    file.open(QIODevice::WriteOnly);
    QDataStream out(&file);
    out << confs_lst;
}

void BookTree::restoreBooksConfs()
{
    QList<OraytaConfs> confs_lst;

    QFile file(USERPATH + "orayta.conf");
    file.open(QIODevice::ReadOnly);
    QDataStream in(&file);
    in >> confs_lst;

    for (QList<OraytaConfs>::const_iterator it = confs_lst.begin(); it != confs_lst.end(); ++it)
    {
        OraytaConfs confs = *it;
        OraytaBookItem* obook = dynamic_cast<OraytaBookItem*>(findBookById(confs.bookid));

        if (!obook) continue;

        if (!confs.hasDefaultFont) obook->setFont(confs.font);
        if (confs.checked) obook->setSelected(confs.checked);
        if (confs.isNikudShown) obook->showNikud(true);
        if (confs.isTeamimShown) obook->showTeamim(true);

        obook->setShowAlone(confs.showAlone);

        for (int i=0; i < confs.weavedSrcShown.size(); ++i)
            obook->setWeavedSourceState(i, confs.weavedSrcShown[i]);
    }
}

void BookTree::keyPressEvent( QKeyEvent *keyEvent )
{
    if ( keyEvent->key() == Qt::Key_Return || keyEvent->key() == Qt::Key_Enter )
    {
        BaseNodeItem* node = dynamic_cast<BaseNodeItem*>(currentItem());
        if ( node)
        {
            if ( node->nodetype() == NodeBook::Node )  // it's directory
            {
                if ( node->isExpanded() )
                    collapseItem(node);
                else
                    expandItem(node);
            }
            else
            {
                emit openBookRequested(dynamic_cast<NodeBook*>(node), true);
            }
        }
    }
    QTreeWidget::keyPressEvent(keyEvent);
}

//////////////////////////////////////////////////
///                  public slots
//////////////////////////////////////////////////

void BookTree::updateTree()
{
    saveBooksConfs();

    mMapId.clear();
    mBookList.clear();

    QList<QTreeWidgetItem*> children = m_userRoot->takeChildren();
    qDeleteAll(children.begin(), children.end());

    children = m_root->takeChildren();
    qDeleteAll(children.begin(), children.end());

    init();

    restoreBooksConfs();
}

void BookTree::openSelectedBook()
{
    NodeBook* book = dynamic_cast<NodeBook*>(currentItem());
    if (book) emit openBookRequested(book, false);
}

void BookTree::openSelectedBookInNewTab()
{
    NodeBook* book = dynamic_cast<NodeBook*>(currentItem());
    if (book) emit openBookRequested(book, true);
}

// delete user book and associated node
void BookTree::deleteSelectedBook()
{
    QTreeWidgetItem * selectedItem = currentItem();

    BaseNodeItem* book = dynamic_cast<BaseNodeItem*>( selectedItem );
    QString path = book->getPath();

    QString displayText = tr("Are you sure you want to remove this ")
              + ( book->nodetype() == BaseNodeItem::Node ? tr("books directory ?\nThis will remove all the books in this directory.")
                                                         : tr("book ?") );

    int ret = QMessageBox::warning(this, tr("Deleting book"),
                                   displayText,
                                   QMessageBox::Ok | QMessageBox::Cancel,
                                   QMessageBox::Cancel);
    switch (ret)
    {
    case QMessageBox::Ok:
        if ( book->nodetype() == BaseNodeItem::Node )
        {
            // deleteBooksFolder( path );
            QtConcurrent::run( deleteBooksFolder, path );
        }
        else
        {
            if ( dynamic_cast<OraytaBookItem*>(book) != 0 )
                deleteOraytaBook(path);
            else if ( !QFile::remove( path ) )
                qDebug() << "Couldn't remove file: " << path;
        }
        updateTree();
        break;

    default:
        break;
    }
}

void BookTree::changeFont()
{
    BaseNodeItem* node = dynamic_cast<BaseNodeItem*>(currentItem());

    QFont defaultFont = gFont;

    // Ugly...
    if (node->nodetype() == BaseNodeItem::Leaf)
    {
        OraytaBookItem* obook = dynamic_cast<OraytaBookItem*>(node);
        if (obook) defaultFont = obook->getFont();
    }

    bool ok;
    QFont font = QFontDialog::getFont(&ok, defaultFont, this);
    if (ok) node->changeFont(font);
}

void BookTree::displayContextMenu(QPoint pos)
{
    QTreeWidgetItem * item = itemAt(pos);
    if ( !item ) return;

    BaseNodeItem* node = dynamic_cast<BaseNodeItem*>(item);
    if ( !node ) return;

    QList<QAction*> menuActions = node->menuActions();

    if ( !menuActions.empty() )
    {
        QMenu menu(this);

        for (int i = 0; i < menuActions.size(); ++i)
            menu.addAction(menuActions[i]);

        menu.setLayoutDirection(Qt::RightToLeft);
        //Open the menu to the left of the cursor's position
        QPoint point = QPoint(QCursor::pos().x() - menu.width(), QCursor::pos().y());

        menu.exec(point);
    }
}

/* // helpers function for make list of mixed displays by id

NodeBook* BookTree::getBoookByPath(const QString& path)
{
    for (BookList::const_iterator it = mBookList.begin(); it != mBookList.end(); ++it)
    {
        if ( (*it)->getPath() == path ) return *it;
    }
    return 0;
}

template <typename T>
QTextStream & operator<< ( QTextStream & out, const QList<T> & list )
{
    out << "(";
    for (int i=0; i < list.size(); ++i)
    {
        out << list[i];
        if (i != list.size() - 1)
            out << ", ";
    }
    out << ")";

    return out;
}

void BookTree::saveMixedInfos()
{
    QList < QPair <QString, QList<int> > > save;
    for (BookList::const_iterator it = mBookList.begin(); it != mBookList.end(); ++it)
    {
        OraytaBookItem* obook = dynamic_cast<OraytaBookItem*>(*it);
        if ( !obook ) continue;

        const QList<weavedSource> sources = obook->weavedSources();
        if (sources.empty()) continue;

        QString path = obook->getPath();
        QList<int> mixed;
        for (int i=1; i < sources.size(); i++)
        {
            weavedSource src = sources[i];
            qDebug() << src.FileName;
            NodeBook* book = getBoookByPath(src.FileName);
            mixed.push_back( (book ? book->getRealUniqueId() : -1) );
        }
        save.push_back( qMakePair(path, mixed) );
    }

    QFile data(BOOKPATH + "mixedInfos.txt");
    if (!data.open(QFile::WriteOnly)) return;

    QTextStream out(&data);

    QListIterator< QPair <QString, QList<int> > > i(save);
    while(i.hasNext())
    {
        QPair <QString, QList<int> > val = i.next();

        out << val.first << " : " << val.second  << "\n";
    }
}
*/


/* // save confs in distinct file

#include <quazip/quazip.h>
#include <quazip/quazipfile.h>

QTextStream & operator<< ( QTextStream & out, const QStringList & list )
{
    for (QStringList::const_iterator it = list.begin(); it != list.end(); ++it)
        out << *it << "\n";
    return out;
}

void renew_confs(OraytaBookItem* ory_book)
{
    QString path = ory_book->getPath();
    QStringList confs;
    ReadZipComment(path, confs, "UTF-8");

    QuaZip zip(path);
    if (!zip.open(QuaZip::mdAdd)) qDebug() << "Error on renew_confs!";

    QuaZipFile zfile(&zip);
    zfile.open(QIODevice::WriteOnly, QuaZipNewInfo("Confs"));
    QTextStream out(&zfile);

    out.setCodec( QTextCodec::codecForName("UTF-8") );

    out << confs;
}
*/
