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

#ifndef BookList_H
#define BookList_H

#include "book.h"

//A BookList, basically a vector of books, but with functions associated with it
class BookList : public vector<Book *>
{
public:
    BookList();
    ~BookList();

    //Return the index of the book with the given UniqueId - in the book list
    //  (Returns -1 if it doesn't exist)
    int FindBookById(int id);
    Book* FindBookByName (QString name);
    Book* findBookById(int id);

    //Return the index of the book with the given TreeWidgetItem - in the book list
    //  (Returns -1 if it doesn't exist)
    int FindBookByTWI(QTreeWidgetItem *);
    Book* findBookByTWI(QTreeWidgetItem *);


    //Builds the booklist from all fles within the given folder
    // (Simply calls "addAllBooks")
    void BuildFromFolder(QString dirpath, bool userBooks = false);
    // Recursivly add all book files to the list
    void addAllBooks (QString dirpath, bool userBooks = false, int parentindex = -1);
    //Add the book's confs, from it's conf file
    void AddBookConfs(Book *book, QString filename);

    vector<Book*> BooksInSearch (void);
    vector<Book*> Children ( Book* book );

    void CheckUid();

private:
};

#endif // BookList_H
