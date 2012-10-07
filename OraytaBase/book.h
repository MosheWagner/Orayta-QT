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

#include "bookiter.h"
#include "guematria.h"

class BookList;

#include <QDateTime>
#include <QPointer>
#include <QtGui/QTreeWidgetItem>

extern QString gFontFamily;
extern int gFontSize;

//A simple struct holding all information needed for every source in the weaved display mode
struct weavedSource
{
    QString Title;
    QString FilePath;
    int Zoom;
    int id;
    bool show;
};

struct weavedSourceData : weavedSource
{
    QList <QString> text;
    QString str;
    QString Prefix;
    QString Suffix;
    BookIter itr;
    int line;
};

//Helper struct and functions for generating the html of the book

//Struct holding information about a link to a position in the page
struct IndexItem
{
    QString displayText;
    QString linkPoint;
    //The level of the link.
    // (Used for deciding how the links should be formatted at the top of the page)
    int level;
};


//Simplest struct holding results for a search in book.
// Includes an iter of the result's position, and a preview of results
//  (some puretext before and after it).
struct SearchResult
{
    BookIter itr;
    QString preview;
};

//Class containing information about the books and folders.
class Book
{

public:
    enum Filetype
    {
        Dir,
        Normal,
        Html,
        Pdf,
        Link,
        Unkown
    };

    //Constructor of book class. gets parent book (a dir, of course), path to it's source file,
    //  it's names (see their documentation), and if it is a dir or not.
    Book(Book * parent, QString path, QString name, QString displayname, Filetype ft, bool isUserBook = false);
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
    bool IsMixed();
    bool ShowAlone();
    bool ShowMixed();
    bool IsUserBook();
    bool IsUserCheckable(QTreeWidgetItem *it);
    Book * getParent();
    vector<Book*>& getChildren();
    QTreeWidgetItem * getTreeItemPtr();
    QTreeWidgetItem * getSearchTreeItemPtr();
    QFont getFont();
    Filetype fileType();
    //Returns a conf entry representing this book
    QString confEntry();
    //Returns the filename that should be used for the book, depending on the shown commentaries
    QString HTMLFileName();


    QWidget* tabWidget();


    //set functions:
    void setPath(QString path);
    void setName(QString name);
    void setCopyrightInfo(QString info);
    void setNormallDisplayName(QString displayname);
    void setTreeDisplayName(QString treedisplayname);
    void setParent(Book * parent);
    void setIcon(QTreeWidgetItem *TreeItem, IconState iconstate);
    void setUniqueId(int id);
    void setTreeItemPtr(QTreeWidgetItem * treeitem);
    void setSearchTreeItemPtr(QTreeWidgetItem * treeitem);
    void setIsInSearch(bool);
    void setIsHidden(bool);
    void setFont(const QFont& font);
    void loadFont();

    void setTabWidget(QWidget*);

    //Sets the cosmetic types of the book, by the given conf line
    void setCosmetics(QString);

    //Renders a html file from the given Orayta file.
    // Generates a full file into the given outfile name, and a header file into outfile + ".header.html"
    // (Returns false on failure, true on success)
    // Implemented in htmlgen.cpp
    //bool htmlrender(QString outfile, bool showNikud = true, bool showTeamim = true, QString mark = "");

    //Add the pointed book as a child to this folder
    void add_child(Book * child);

    // Unselects this book (or folder) from the search.
    //  Applies to all children, and changes icon state and color
    void unselect();

    // Selects this book (or folder) for searching.
    //  Applies to all children, and changes icon state and color
    void select();

    bool showAlone;

    bool hasRandomId;

    //Holds a level for special weaving.
    // If this has a value, when a source is weaved with this book, it will weave all the text in this level even if it doesn't perfectly match.
    QString WeaveLevel;

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

    //Icon state of this book (or folder)
    IconState mIconState;

    vector <GuematriaDb> guematriaDb;
    bool guematriaDbExists;

    void BuildGuematriaDb ();
    const vector <GuematriaDb>& getGuematriaDb();

    
    //Rrturn a list of bookiters to all occurrences of the reqested phrase in the book
    QList <SearchResult> findInBook(const QString& phrase);
    //Overloaded function of the above
    QList <SearchResult> findInBook(const QRegExp& regexp);

    void restoreIconeState();
    //Repaints the icon of this book or folder, by the state of it's children
    // (Full blue - all selected, Half - some selected and some not, Full grey - all deselected)
    void repainticon();

    //inline QString DBFilePath();
    //inline QString LMFilePath();

    //Fills "pureText" and "levelMap" with their values
    //void BuildSearchTextDB();


    //This function reads the book's text, split up to 'Lowest Index Levels',
    // into the 'chapterText' list, while the equivilant Iters are stored in the 'chapterIter' list.
    void readBook(int LIL);

    //Builds the list of indexItems that represent the index (תוכן) of the book
    void buildIndex(QList <QString> text);

    //Returns a QUrl to the rendered html file of the chapter given by the iter.
    QUrl renderChapterHtml(BookIter iter, BookList * bl, bool shownikud, bool showteamim, QRegExp mark = QRegExp());

    //Returns a QUrl to the rendered html file of the index (תוכן) of this book
    QUrl renderBookIndex();

    //Returns a bookiter representing the chapter coming before/after the given chapter.
    BookIter nextChap(BookIter iter);
    BookIter prevChap(BookIter iter);

    //Returns True if readbook was called on the book, and closeBook() wasn't
    bool isBookOpen();

    QDateTime modificationDate();

protected:

    //
    //QString chapterIndexHtml;

    //This list holds all the text of the book,
    // split up by the lowest level shown in the index ('Lowest Index Level').
    //This list is built by 'readBook', and the parts are accessed via 'getChapterHtml'
    QList <QStringList> chapterText;

    //This list holds the BookItr of each chapter in the 'chapterText' list.
    QList <BookIter> chapterIter;

    //Lowest Index Level. This value is used for splitting up the book into chapters.
    int LIL;

    QList <IndexItem> indexitemlist;


    // recursives function
    void _unselect();
    void _select();

    BookIter itrFromOffset(QString posString);

    //Returns the book's text as pure as possible. No nikud, punctuation marks, no level signs.
    // This should be used for searching in the book.
    // The results ahould later be mapped to BookIters using the "levelMap"
    QString pureText();

    //Sets the book's puretext
    //void setPureText(QString);

    //Returns true if a DB file for this book already exists.
    //bool hasDBFile();

    //Build the levelMap from a string representing it
    void setLevelMap(QString);
    //Return a QString representing the levelMap
    QString levelMapString();

    //Holds the offsets of each BookIters of book in the "pureText". Use this to map search results in positions in the book
    QMap<int, BookIter> levelMap;

    //Returns a preview of the search result  (by it's search regexp and offset in puretext)
    QString resultPreview(const QRegExp& exp, int offset);

    QTreeWidgetItem *mpTreeItem;
    QTreeWidgetItem *mpSearchTreeItem;

    //Tell if the book should be shown in the book tree or ignored
    bool mIsHidden;

    //Unique id used for linking to a specific book
    int mUniqueId;

    //Tells if the book should be searched trough.
    //  Has no affect on folders
    bool mInSearch;

    bool mUserBook;

    QString mPureText;

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

    Filetype mFiletype;

    //Pointer to book's parent
    Book * mpParent;

    //Level of the short index (index to index)
    int mShortIndexLevel;

    //QWidget* of current viewTab (0 if not exists)
    QPointer<QWidget> mTabWidget;

    // Suffix to remove from the name of the links in it's level
    // Take Notice: this is allways one lower than the link level -
    //      I.E., rus2 is entered to mRemoveSuffix[1], and it's for level 2.
    QString mRemoveSuffix[5];

    //TODO: use these :-)
    QString mIndexSuffix[5];
    QString mIndexPrefix[5];

    QList <QString> replaceFrom;
    QList <QString> replaceTo;


    int mIndexTextSize[5];
    int mTitleEmptyLines[5];

private:

    //font to display (if it's directory, applied on all children)
    // IZAR:
    // this shoud be NULL if it wasn't defined in conf, in which case gFontSize and gFontFamlily will be used instead.
    // the reason is that we want to be able to change the font size during runtime and not only during book constructor.
    QFont *mFont;

};

QIcon* bookIcon(Book* book, IconState state);

#endif // BOOK_H
