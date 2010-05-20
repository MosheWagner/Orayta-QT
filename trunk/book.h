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

#ifndef BOOK_H
#define BOOK_H

#include "functions.h"
#include "bookiter.h"
#include <QtGui/QTreeWidgetItem>


//A simple struct holding all information needed for every source in the weaved display mode
struct weavedSource
{
    vector <QString> text;
    QString Title;
    QString FileName;
    QString Prefix;
    QString Suffix;
    int Zoom;

    int id;

    bool show;

    BookIter itr;
    QString str;
};

//Class containing information about the books and folders.
class Book
{

public:
    //Constructor of book class. gets parent book (a dir, of course), path to it's source file,
    //  it's names (see their documentation), and if it is a dir or not.
    Book(Book * parent, QString path, QString name, QString displayname, bool is_dir);
    ~Book();

    //get functions:
    QString getPath();
    QString getName();
    QString getNormallDisplayName();
    QString getTreeDisplayName();
    int getUniqueId();
    bool IsDir();
    bool IsInSearch();
    bool IsHidden();
    bool IsUnSelectable();
    Book * getParent();
    QTreeWidgetItem * getTreeItemPtr();

    //set functions:
    void setPath(QString path);
    void setName(QString name);
    void setCopyrightInfo(QString info);
    void setNormallDisplayName(QString displayname);
    void setTreeDisplayName(QString treedisplayname);
    void setIsDir(bool is_dir);
    void setParent(Book * parent);
    void setIcon(QTreeWidgetItem *TreeItem, IconState iconstate);
    void setUniqueId(int id);
    void setTreeItemPtr(QTreeWidgetItem * treeitem);
    void setIsInSearch(bool);
    void setIsHidden(bool);
    void setIsUnSelectable(bool);

    //Sets the cosmetic types of the book, by the given conf line
    void setCosmetics(QString);

    //Renders a html file from the given Orayta file.
    // Generates a full file into the given outfile name, and a header file into outfile + ".header.html"
    // (Returns false on failure, true on success)
    // Implemented in htmlgen.cpp
    bool htmlrender(QString outfile, bool showNikud = true, bool showTeamim = true, QString mark = "");

    //Add the pointed book as a child to this folder
    void add_child(Book * child);

    // Unselects this book (or folder) from the search.
    //  Applies to all children, and changes icon state and color
    void unselect();

    // Selects this book (or folder) for searching.
    //  Applies to all children, and changes icon state and color
    void select();

    //Repaints the icon of this book or folder, by the state of it's children
    // (Full blue - all selected, Half - some selected and some not, Full grey - all deselected)
    void repainticon();

    bool isMixed();

    //Variables that don't seem to do much yet; maybe I'll use them later
    bool SearchInTitles;
    bool PutNewLinesAsIs;
    int LastLevelIndex;
    QString GroupId;
    QString TextSource;
    QString Comments;
    QString Cosmetics;

    bool hasNikud;
    bool hasTeamim;

    QList <weavedSource> mWeavedSources;

    //Returns a conf entry representing this book
    QString confEntry();

    //Icon state of this book (or folder)
    IconState mIconState;

protected:

    QTreeWidgetItem *mpTreeItem;

    //Tell if the book should be shown in the book tree or ignored
    bool mIsHidden;

    //Unique id used for linking to a specific book
    int mUniqueId;

    //Tells if the book should be searched trough.
    //  Has no affect on folders
    bool mInSearch;

    //Tells if the book can't be selected at all. I.E., mixed displays and welcome page
    bool mIsUnSelectable;


    //Vector containing pointers to the folders children
    vector<Book *> mvChildren;

    //Information about copyrights of this book
    QString mCopyrightInfo;

    //Path of the book's source file
    QString mPath;

    //Name used for the book title (html title tag, and top of book)
    QString mNameForTitle;

    //Display name for most purposes (search results, etc.)
    QString mNormallDisplayName;

    //Display name for the book tree.
    // Assumes you see it's parents name next to it, so it isn't enough on it's own.
    //  (It's something like "With Nikud", etc.,)
    QString mTreeDisplayName;

    //Is this book a book or a dir
    bool mIsDir;

    //Pointer to book's parent
    Book * mpParent;

    //Level of the short index (index to index)
    int mShortIndexLevel;

    // Suffix to remove from the name of the links in it's level
    // Take Notice: this is allways one lower than the link level -
    //      I.E., rus2 is entered to mRemoveSuffix[1], and it's for level 2.
    QString mRemoveSuffix[5];

    //TODO: use these :-)
    QString mIndexSuffix[5];
    QString mIndexPrefix[5];

    int mIndexTextSize[5];
    int mTitleEmptyLines[5];


    bool normalHtmlRender(QString infile, QString outfile, bool showNikud, bool showTeamim, QRegExp mark);
    bool mixedHtmlRender(QString outfile, bool showNikud, bool showTeamim, QRegExp mark);
};



#endif // BOOK_H
