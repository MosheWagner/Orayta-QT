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

#include "booklist.h"

#include <QSet>
#include <QDir>
#include <QFileInfoList>
#include <QFileInfoListIterator>
#include <QDebug>

BookList::BookList() {}

BookList::BookList(BookList * other_list)
{
    for(unsigned int i=0; i<other_list->size(); i++)
    {
        this->at(i) = other_list->at(i);
    }

}

BookList::~BookList() {   }

//Builds the booklist from all fles within the given folder
// (Simply calls "addAllBooks")
void BookList::BuildFromFolder(QString dirpath, bool isUserBooks)
{    addAllBooks (dirpath, isUserBooks);     }

// Recursivly add all book files to the list
void BookList::addAllBooks (QString dirpath, bool isUserBooks, int parentindex)
{
    QDir cdir(absPath(dirpath));

    //Get all .folder or .obk files in the directory
    QStringList filter; filter << "*.folder" << "*.obk" << "*.pdf" << "*.link" << "*.html" << "*.htm";
    QFileInfoList list = cdir.entryInfoList(filter, QDir::AllEntries | QDir::NoSymLinks | QDir::NoDotAndDotDot, QDir::Name);

    //TODO: Add pdf, html, etc'

    for (int i=0; i<list.size(); i++)
    {
        Book::Filetype ft;
        if (list[i].fileName().endsWith(".folder", Qt::CaseInsensitive))
            ft = Book::Dir;
        else if (list[i].fileName().endsWith(".obk", Qt::CaseInsensitive))
            ft = Book::Normal;
        else if (list[i].fileName().endsWith(".html", Qt::CaseInsensitive) || list[i].fileName().endsWith(".htm", Qt::CaseInsensitive))
            ft = Book::Html;
        else if (list[i].fileName().endsWith(".pdf", Qt::CaseInsensitive))
            ft = Book::Pdf;
        else if (list[i].fileName().endsWith(".link", Qt::CaseInsensitive))
            ft = Book::Link;
        else
            ft = Book::Unkown;

        if ( ft != Book::Unkown )
        {
            if ( list[i].fileName().indexOf("Pics") != -1 )
                continue;

            QString Name = list[i].fileName();
            if (!isUserBooks)
                Name.replace(QRegExp("^[0-9 ]*"), "").replace("_", " ").replace(".obk", "");

            //Create BookListItem
            Book *b = new Book(parentindex != -1 ? (*this)[parentindex] : NULL,
                                   list[i].absoluteFilePath(), list[i].absoluteFilePath(), Name, ft, isUserBooks);

            //Tell the parent it has a new child
            if (b->getParent() != NULL)
                b->getParent()->add_child(b);

            //Add this book to the list
            push_back(b);

            //Add folder entry, and all files in that folder
            if (ft == Book::Dir)
            {
                //Do the whole thing again for any dir, sending it's index in the list as the
                // Parents index of all it's children
                addAllBooks(list[i].absoluteFilePath().replace(".folder", ""), isUserBooks, this->size() - 1);

                //Add confs for this directory
                QList <QString> t;
                t.clear();
                if (ReadFileToList(list[i].absoluteFilePath(), t , "UTF-8")) AddBookConfs(b, t);
            }

            //Add orayta-book
            else if ( ft == Book::Normal )
            {
                //Add confs for this book
                QList <QString> t;
                t.clear();
                if (ReadZipComment(list[i].absoluteFilePath(), t, "UTF-8")) AddBookConfs(b, t);
            }

            else if ( ft == Book::Link )
            {
                //Add confs for this book
                AddBookConfs(b, readfile(b->getPath(), "UTF-8").split("\n"));
            }

            else if ( ft == Book::Html )
            {
                //Add confs for this book
                AddBookConfs(b, readfile(b->getPath().replace(QRegExp("\\.\\w{3,4}$"),".conf"), "UTF-8").split("\n"));
            }

        }
    }

    /*
    for (int i=0; i<list.size(); i++)
    {
        Book::Filetype ft;

        // set the file type
        if (list[i].isDir())
            ft = Book::Dir;
        else if (list[i].fileName().endsWith(".txt", Qt::CaseInsensitive))
            ft = Book::Normal;
        else if (list[i].fileName().endsWith(".html", Qt::CaseInsensitive) || list[i].fileName().endsWith(".htm", Qt::CaseInsensitive))
            ft = Book::Html;
        else if (list[i].fileName().endsWith(".pdf", Qt::CaseInsensitive))
            ft = Book::Pdf;
        else if (list[i].fileName().endsWith(".link", Qt::CaseInsensitive))
            ft = Book::Link;
        else
            ft = Book::Unkown;

        if ( ft != Book::Unkown )
        {
            if ( list[i].fileName().indexOf("Pics") != -1 )
                continue;

            QString Name = list[i].fileName();
            if (!isUserBooks)
                Name.replace(QRegExp("^[0-9 ]*"), "").replace("_", " ");

            //Create BookListItem
            Book *b = new Book(parentindex != -1 ? (*this)[parentindex] : NULL,
                                   list[i].absoluteFilePath(), list[i].absoluteFilePath(), Name, ft, isUserBooks);

            //Tell the parent it has a new child
            if (b->getParent() != NULL)
                b->getParent()->add_child(b);

            //Add this book to the list
            push_back(b);

            if ( ft == Book::Normal )
            {
                //Add confs for this book
                AddBookConfs(b, b->getPath().replace(".txt",".conf"));
            }

            if ( ft == Book::Html )
            {
                //Add confs for this book
                AddBookConfs(b, b->getPath().replace(QRegExp("\\.\\w{3,4}$"),".conf"));
            }

            if ( ft == Book::Link )
            {
                //Add confs for this book
                AddBookConfs(b, b->getPath());
            }

            if ( isUserBooks )
            {
                //b->setIsInSearch(false);
            }
            else
            {
                // call this after AddBookConfs
                b->loadFont();
            }

            if (ft == Book::Dir)
            {
                //Do the whole thing again for any dir, sending it's index in the list as the
                // Parents index of all it's children
                addAllBooks(list[i].absoluteFilePath(), isUserBooks, this->size() - 1);

                //Add confs for this directory
                AddBookConfs(b, b->getPath().append(".conf"));

            }
        }
    }
    */
}

//Add the book's confs, from it's conf file
void BookList::AddBookConfs(Book *book, QList<QString> text)
{
    vector<QString> linesplit;

    for(int i=0; i<text.size() ; i++)
    {
        if (text[i].indexOf("Comments") != -1)
        {
            book->Comments = text[i].mid(9);
        }

        else if (text[i].indexOf("GroupId") != -1)
        {
            book->GroupId = text[i].mid(8);
        }

        else if (text[i].indexOf("DisplayName") != -1)
        {
            book->setNormallDisplayName(text[i].mid(12).replace(QRegExp("^ *"), ""));
        }

        //field for the display name of the directory
        else if (text[i].indexOf("BranchName") != -1)
        {
            linesplit.clear();
            splittotwo(text[i],linesplit ,"=");
            if (linesplit[1]!="")
            {
                book->setTreeDisplayName(linesplit[1]);
            }
        }

        else if (text[i].indexOf("LastLevelIndex") != -1)
        {
            int n;
            if(ToNum(text[i].mid(15),&n))
                book->LastLevelIndex = n;
        }

        else if (text[i].indexOf("PutNewLinesAsIs") != -1)
        {
            if(text[i].mid(16) != "1")
                book->PutNewLinesAsIs=false;
            else
                book->PutNewLinesAsIs=true;
        }

        else if (text[i].indexOf("TextSource") != -1)
        {
            book->setCopyrightInfo(text[i].mid(11));
        }

        else if (text[i].indexOf("ForcedBookName") != -1)
        {
            linesplit.clear();
            splittotwo(text[i],linesplit ,"=");
            if (linesplit[1]!="")
                book->setTreeDisplayName(linesplit[1]);
        }

        else if (text[i].indexOf("HiddenFromIndex") != -1)
        {
            int t;
            GetIntValue(text[i], &t);
            if (t==1)
            {
                book->setIsHidden(true);
                book->setIsInSearch(false);
            }
        }

        else if (text[i].indexOf("WeaveLevel") != -1)
        {
            QString wl = text[i].mid(text[i].size()-1);
            book->WeaveLevel = wl;
        }

        else if (text[i].indexOf("AddSource") != -1)
        {
            if ( book->mWeavedSources.size() == 0)
            {
                //Add the book it self as the first source
                weavedSource base;

                base.FilePath = book->getPath();

                base.Zoom = 0;

                base.id = 0;

                base.show = true;

                book->mWeavedSources.append(base);
            }

            QStringList p = text[i].mid(10).split(":");
            if (p.size() > 1)
            {
                weavedSource src;
                src.FilePath = BOOKPATH + (BOOKPATH.endsWith("/") ? "" : "/") + p[0];
                src.Title = p[1];

                src.Zoom = 0;

                src.id = book->mWeavedSources.size();

                src.show = false;

                book->mWeavedSources.append(src);
            }
            else
            {
                qDebug() << "Invalid weaved source in:" << book->getPath();
                qDebug() << "Bad path:" << text[i];
            }
        }

        // code from yoch to add new commentaries by id
        else if (text[i].indexOf("MixedSources") != -1)
        {
            QString srcLst = text[i].mid(12);
            srcLst = srcLst.mid(srcLst.indexOf("(") + 1);
            srcLst = srcLst.mid(0, srcLst.indexOf(")"));
            QList<int> wsrc;
            QStringList srcId = srcLst.split(',', QString::SkipEmptyParts);

            for (QStringList::const_iterator it = srcId.begin(); it != srcId.end(); ++it)
            {
                int id;
                if(ToNum(*it, &id))
                {
                    weavedSource src;
                    src.show = false;
                    src.id = id;
                    //                    mWeavedSources.push_back(src);
                    book->mWeavedSources.append(src);
                }
            }
        }

        else if (text[i].indexOf("CosmeticsType") != -1)
        {
            book->setCosmetics(text[i]);
        }

        else if (text[i].indexOf("UniqueId") != -1)
        {
            int id;
            if(ToNum(text[i].mid(9), &id) == true)
                book->setUniqueId(id);
        }

        else if (text[i].indexOf("Nikud") != -1)
        {
            book->hasNikud=true;
        }

        else if (text[i].indexOf("Teamim") != -1)
        {
            book->hasTeamim=true;
        }
    }
}

//Return the index of the book with the given UniqueId - in the book list
//  (Returns -1 if it dosen't exist)
int BookList::FindBookById(int ID)
{
    for(unsigned int i=0; i<size(); i++)
    {
        if((*this)[i]->getUniqueId() == ID)
            return i;
    }
    return -1;
}

//Return pointer to the book with the given name - in the book list
//  (Returns NULL if it dosen't exist)
//  this function is unsafe, because two books can have the same name
Book* BookList::FindBookByName (QString name)
{
    for(unsigned int i=0; i<size(); i++)
    {
        if( (*this)[i]->getNormallDisplayName() == name )
            return (*this)[i];
    }
    return NULL;
}

//Return pointer to the book with the given filepath - in the book list
//  (Returns NULL if it dosen't exist)
Book* BookList::FindBookByPath (QString path)
{
    for(unsigned int i=0; i<size(); i++)
    {
        QString p = (*this)[i]->getPath().replace("\\", "/").replace("//","/");
        if( p == path.replace("\\", "/").replace("//","/") )
            return (*this)[i];
    }
    return NULL;
}

//Return pointer to the book with the given UniqueId - in the book list
//  (Returns NULL if it dosen't exist)
Book* BookList::findBookById(int id)
{
    for(unsigned int i=0; i<size(); i++)
    {
        if((*this)[i]->getUniqueId() == id)
            return (*this)[i];
    }
    return NULL;
}

//Return the index in the book list of the book with the given TreeWidgetItem
//  (Returns -1 if it dosen't exist)
int BookList::FindBookByTWI(QTreeWidgetItem *TWI)
{
    for(unsigned int i=0; i<size(); i++)
    {
        if((*this)[i]->getTreeItemPtr() == TWI)
            return i;
    }
    for(unsigned int i=0; i<size(); i++)
    {
        if((*this)[i]->getSearchTreeItemPtr() == TWI)
            return i;
    }
    return -1;
}

//Return the pointer to the book with the given TreeWidgetItem
//  (Returns NULL if it dosen't exist)
Book* BookList::findBookByTWI(QTreeWidgetItem *TWI)
{
    for(unsigned int i=0; i<size(); i++)
    {
        if((*this)[i]->getTreeItemPtr() == TWI)
            return (*this)[i];
    }
    for(unsigned int i=0; i<size(); i++)
    {
        if((*this)[i]->getSearchTreeItemPtr() == TWI)
            return (*this)[i];
    }
    qDebug()<< "can't find book from TWI : " << TWI->text(0);
    return NULL;
}

vector<Book*> BookList::BooksInSearch (void)
{
    vector<Book*> ret;

    for ( vector<Book*>::iterator it=this->begin() ; it < this->end(); it++ )
        if (!(*it)->IsDir() && (*it)->IsInSearch())
            ret.push_back(*it);

    return ret;
}

vector<Book*> BookList::Children ( Book* book )
{
    vector<Book*> ret;

    if ( book->IsDir() )
    {
        vector<Book*> children = book->getChildren();

        for ( vector<Book*>::iterator it=children.begin() ; it < children.end(); ++it )
        {
            if ( (*it)->IsDir() )
            {
                vector<Book*> ch = Children( *it );
                ret.insert( ret.end(), ch.begin(), ch.end() );
            }
            else
            {
                ret.push_back(*it);
            }
        }
    }

    return ret;
}


void BookList::CheckUid()
{
    QSet <int> existingId;
    QList <Book*> withoutId;

    for (unsigned i=0; i < this->size(); i++)
    {
        int id = (*this)[i]->getUniqueId();
        if ( id != -1 )
        {
            if (existingId.contains(id))
            {
                qDebug()<< "duplicate id: " << id;
                qDebug()<< "in: " << (*this)[i]->getPath() << " and: " << this->findBookById(id)->getPath();
            }
            existingId.insert(id);
        }
        else if ( (*this)[i]->fileType() == Book::Normal || (*this)[i]->fileType() == Book::Html )
            withoutId.push_back((*this)[i]);
    }

    //Set random id for books without id
    for (int i=0; i < withoutId.size(); i++)
    {
        qDebug() << "no uniqueId found for : " << withoutId[i]->getName();

        int randomId;
        do {
            randomId = rand();
        } while ( existingId.contains( randomId ) );

        withoutId[i]->setUniqueId( randomId );
        withoutId[i]->hasRandomId = true;
        existingId.insert( randomId );
    }
}

//Display the booklist in the given QTreeWidget
void BookList::displayInTree(QTreeWidget *tree, bool showCheck, bool isSearchTree)
{
    //Add treeItems for each book to the treeWidget
    for(unsigned int i=0;i<size();i++)
    {
        if (this->at(i)->IsHidden() != true)
        {
            QTreeWidgetItem *twi;
            if(this->at(i)->getParent() == NULL)
                twi = new QTreeWidgetItem(tree);
            else
                if(isSearchTree)
                    twi = new QTreeWidgetItem(this->at(i)->getParent()->getSearchTreeItemPtr());
                else
                    twi = new QTreeWidgetItem(this->at(i)->getParent()->getTreeItemPtr());

            if(isSearchTree)
                this->at(i)->setSearchTreeItemPtr(twi);
            else
                this->at(i)->setTreeItemPtr(twi);

            QString dn;

            if(!this->at(i)->getTreeDisplayName().simplified().isEmpty())
                dn = this->at(i)->getTreeDisplayName().simplified();
            else if (!this->at(i)->getNormallDisplayName().simplified().isEmpty())
                dn = this->at(i)->getNormallDisplayName().simplified();
            else
            {
                vector<QString> name_parts;
                splittotwo(this->at(i)->getName(), name_parts, "_");
                dn = name_parts[1];
            }
            dn.replace("שס", "ש\"ס");

            twi->setText(0, dn);

            //set the icon:
            QIcon *icon = bookIcon(this->at(i), this->at(i)->mIconState);
            twi->setIcon(0, *icon);
            delete icon;

            twi->setToolTip(1, this->at(i)->getName());

            if ( showCheck && ( this->at(i)->fileType() == Book::Dir || this->at(i)->fileType() == Book::Normal || this->at(i)->fileType() == Book::Html ) )
            {
                twi->setFlags( Qt::ItemIsSelectable | Qt::ItemIsEnabled | Qt::ItemIsUserCheckable );
                twi->setCheckState(0,Qt::Unchecked);
            }
            else
            {
                twi->setFlags( Qt::ItemIsSelectable | Qt::ItemIsEnabled );
            }
        }
    }
}
