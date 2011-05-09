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

BookList::BookList() { }

BookList::~BookList() {   clear();  }

//Builds the booklist from all fles within the given folder
// (Simply calls "addAllBooks")
void BookList::BuildFromFolder(QString dirpath, bool isUserBooks)
{    addAllBooks (dirpath, isUserBooks);     }

// Recursivly add all book files to the list
void BookList::addAllBooks (QString dirpath, bool isUserBooks, int parentindex)
{
    QDir cdir(absPath(dirpath));

    //Get all files in this dir
    QFileInfoList list = cdir.entryInfoList(QDir::AllEntries | QDir::NoSymLinks | QDir::NoDotAndDotDot,
                                            QDir::Name);

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
            }
        }
    }
}

//Add the book's confs, from it's conf file
void BookList::AddBookConfs(Book *book, QString filename)
{
    QList <QString> text;
    vector<QString> linesplit;

    //Read the book's conf file file:
    if(ReadFileToList(filename, text, "UTF-8"))
    {
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
/*
            else if (text[i].indexOf("MixedDisplay") != -1)
            {
                GetIntValue(text[i], &(book->mWeavedSources));

                //All mixed books can't be searched or selected from the tree
                if (book->IsMixed()) book->setIsUnSelectable(true);
            }
*/
            else if (text[i].indexOf("AddSource") != -1)
            {
                if ( book->mWeavedSources.size() == 0)
                {
                    //Add the book it self as the first source
                    weavedSource base;

                    base.FileName = book->getPath();
    
                    base.Zoom = 0;

                    base.id = 0;

                    base.show = true;
    
                    book->mWeavedSources.append(base);
                }

                QStringList p = text[i].mid(10).split(":");
                weavedSource src;
                src.FileName = BOOKPATH + "/" + p[0];
                src.Title = p[1];

                src.Zoom = 0;

                src.id = book->mWeavedSources.size();

                src.show = false;

                book->mWeavedSources.append(src);
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
            existingId.insert(id);
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

