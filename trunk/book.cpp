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

#include "book.h"

#include <QSettings>
#include <QDir>
#include "functions.h"
#include <QDebug>

//Constructor of the Books
Book::Book(Book * parent, QString path, QString name, QString displayname, Filetype ft, bool isUserBook)
{
    mUniqueId = -1;
    mTreeDisplayName = "";

    mNormallDisplayName = displayname;
    mpParent = parent;
    mPath = path;
    //In most cases this will be overriden when rendered
    mNameForTitle = name;

    mFiletype = ft;
    mUserBook = isUserBook;

    mpTreeItem = NULL;

    WeaveLevel = "";

    for(int i=0;i<5;i++)
    {
        mRemoveSuffix[i]="";
        mTitleEmptyLines[i] = 0;
        mIndexSuffix[i] = "";
        mIndexPrefix[i] = "";
    }

    mShortIndexLevel = 0;

    mInSearch = false;
    mIconState = GREY;

    mCopyrightInfo="";
    SearchInTitles = true;
    PutNewLinesAsIs = true;
    LastLevelIndex = 0;

    mIsHidden = false;

    GroupId = "";
    TextSource="";
    Comments = "";
    Cosmetics="";
    
    hasNikud = false;
    hasTeamim = false;

    showAlone = true;

    mTabWidget = 0;

    guematriaDbExists = false;

    hasRandomId = false;

//    mFont = QFont( gFontFamily, gFontSize );
    mFont = NULL;


    //Split level for chapter splitting
    LIL = 1;

    mPureText = "";

    chapterIter.clear();
    chapterText.clear();
    indexitemlist.clear();
}

Book::~Book()
{
    //delete mpTreeItem;
}


// Book Getters

QString Book::getPath()
{   return mPath;  }

int Book::getUniqueId()
{   return mUniqueId; }

QString Book::getNormallDisplayName()
{ return mNormallDisplayName;         }

QString Book::getTreeDisplayName()
{  return mTreeDisplayName;         }

QString Book::getName()
{  return mNameForTitle;  }

bool Book::IsHidden()
{  return mIsHidden; }

bool Book::IsInSearch()
{  return mInSearch; }

bool Book::IsMixed()
{  return (mWeavedSources.size() != 0); }

bool Book::IsDir()
{  return mFiletype == Book::Dir;  }

bool Book::IsUserBook()
{ return mUserBook; }

bool Book::IsUserCheckable()
{
    if (!mpTreeItem)
        return false;
    return mpTreeItem->flags() & Qt::ItemIsUserCheckable;
}

bool Book::ShowMixed()
{
    if (!IsMixed() || showAlone)
        return false;

    unsigned shownMixed = 0;
    for (int i=1; i<mWeavedSources.size(); i++)
        if (mWeavedSources[i].show)
            shownMixed++;
    return shownMixed > 0;
}

bool Book::ShowAlone()
{  return !ShowMixed(); }

QTreeWidgetItem * Book::getTreeItemPtr()
{  return mpTreeItem; }

// Book Setters:
void Book::setPath(QString path)
{   mPath = path;  }

void Book::setName(QString name)
{   mNameForTitle = name;  }

void Book::setNormallDisplayName(QString displayname)
{ mNormallDisplayName = displayname;  }

void Book::setTreeDisplayName(QString treedisplayname)
{ mTreeDisplayName = treedisplayname;  }

void Book::setParent(Book * parent)
{   mpParent = parent;  }

Book *  Book::getParent()
{   return mpParent;          }

vector<Book*>& Book::getChildren()
{   return mvChildren;  }

void Book::setCopyrightInfo(QString info)
{   mCopyrightInfo = info; }

void Book::setUniqueId(int id)
{   mUniqueId = id ;}

void Book::setTreeItemPtr(QTreeWidgetItem * treeitem)
{   mpTreeItem = treeitem; }

void Book::add_child(Book * child)
{
    if(child != NULL)
        mvChildren.push_back(child);
}

void Book::setIsInSearch(bool in_search)
{ mInSearch = in_search; }

void Book::setIsHidden(bool isHidden)
{ mIsHidden = isHidden; }

QWidget* Book::tabWidget()
{ return mTabWidget; }

void Book::setTabWidget(QWidget* w)
{ mTabWidget = w; }

Book::Filetype Book::fileType()
{  return mFiletype;  }

//see header for info
QFont Book::getFont()
{  if (mFont)
        return *mFont;
    else
        return QFont(gFontFamily, gFontSize);
}

void Book::setFont(const QFont& font)
{
    mFont = new QFont(font);

    if ( !mvChildren.empty() )
    {
        for (vector<Book*>::iterator it = mvChildren.begin(); it != mvChildren.end(); ++it)
            (*it)->setFont(font);
    }

    if (mUniqueId != -1)
    {
        QSettings settings("Orayta", "SingleUser");
        settings.beginGroup("Book" + stringify(mUniqueId));
        settings.setValue("Font", font.toString());
        settings.endGroup();
    }
}

void Book::loadFont()
{
    QSettings settings("Orayta", "SingleUser");
    settings.beginGroup("Book" + stringify(mUniqueId));
    QString fontDescription = settings.value("Font", "").toString();
    settings.endGroup();


    // if no font
    if ( fontDescription == "" || !mFont || mFont->fromString(fontDescription) == false )
    {
        if (mpParent != NULL)
        {
            setFont(mpParent->getFont());
        }
    }
}

void Book::repainticon()
{
    //UnSelectable should allways be grey
    //if (mIsUnSelectable) setIcon(mpTreeItem, GREY);

    bool blue_child = false, grey_child = false, mixed_child = false;

    //Check children
    for (unsigned int i=0;i < mvChildren.size(); i++)
    {
        if ( mvChildren[i]->mFiletype != Book::Normal && mvChildren[i]->mFiletype != Book::Dir )
            continue;

        if ( mvChildren[i]->mIsHidden == false)
        {
            if(mvChildren[i]->mIconState == BLUE)
                blue_child = true;
            else if(mvChildren[i]->mIconState == GREY)
                grey_child = true;
            else if(mvChildren[i]->mIconState == HALF)
                mixed_child = true;
        }
    }

    if ( !blue_child && !mixed_child )
    {
        //Dark icon
        setIcon(mpTreeItem, GREY);
    }
    else if ( !grey_child && !mixed_child )
    {
        //Blue icon
        setIcon(mpTreeItem, BLUE);
    }
    else
    {
        //Half icon
        setIcon(mpTreeItem, HALF);
    }

    if(getParent() != NULL)
        getParent()->repainticon();
}

void Book::_unselect()
{
    if ( !IsUserCheckable() )
        return;

    mInSearch = false;

    //Darken icon
    setIcon(mpTreeItem, GREY);

    //Unselect all children
    for (unsigned int i=0;i < mvChildren.size(); i++)
        mvChildren[i]->_unselect();
}

void Book::unselect()
{
    //if (mFiletype == Book::Normal || mFiletype == Book::Dir || mFiletype == Book::Html)
    {
        this->_unselect();

        //Repaint parent (it will tell it's parent to repaint too, and so on...)
        if(mpParent != NULL)
            mpParent->repainticon();
    }
}

void Book::_select()
{
    if ( !IsUserCheckable() )
        return;

    mInSearch = true;

    //Make icon blue
    setIcon(mpTreeItem, BLUE);

    //select all children
    for (unsigned int i=0;i < mvChildren.size(); i++)
        mvChildren[i]->_select();
}

void Book::select()
{
    //if (mFiletype == Book::Normal || mFiletype == Book::Dir || mFiletype == Book::Html)
    {
        this->_select();
        //Repaint parent (it will tell it's parent to repaint too, and so on)
        if(mpParent != NULL)
            mpParent->repainticon();
    }
}

QIcon* bookIcon(Book* book, IconState state)
{
    QIcon * icon;

    switch (book->fileType())
    {
    case (Book::Dir):
        if (book->IsUserBook()) icon = new QIcon(":/Icons/folder-user.png");
        else if (state == BLUE) icon = new QIcon(":/Icons/folder-blue.png");
        else if (state == GREY) icon = new QIcon(":/Icons/folder-grey.png");
        else if (state == HALF) icon = new QIcon(":/Icons/folder-blue-grey.png");
        break;

    case (Book::Normal):
        if ( book->IsUserBook() )
        {
            icon = new QIcon(":/Icons/book-user.png");
        }
        else if ( !book->IsMixed() )
        {
            if (state == BLUE) icon = new QIcon(":/Icons/book-blue.png");
            else if (state == GREY) icon = new QIcon(":/Icons/book-grey.png");
        }
        else
        {
            if (state == BLUE) icon = new QIcon(":/Icons/book-blue-double.png");
            else if (state == GREY) icon = new QIcon(":/Icons/book-grey-double.png");
        }
        break;

    case (Book::Html):
        icon = new QIcon(":/Icons/book-html.png");
        break;

    case (Book::Pdf):
        icon = new QIcon(":/Icons/book-pdf.png");
        break;

    default:
        icon = new QIcon(":/Icons/book-grey.png");
        break;
    }

    return icon;
}

void Book::setIcon(QTreeWidgetItem *TreeItem, IconState newiconstate)
{
    if (TreeItem == NULL) return;

    //set icon state
    mIconState = newiconstate;
    QIcon *icon = bookIcon(this, mIconState);
    TreeItem->setIcon(0, *icon);
    delete icon;

    //if ( !mUserBook )
    {
        //Show state in the checkbox too
        if (mIconState == BLUE)
            TreeItem->setCheckState(0,Qt::Checked);
        else if (mIconState == HALF)
            TreeItem->setCheckState(0,Qt::PartiallyChecked);
        else if (mIconState == GREY)
            TreeItem->setCheckState(0,Qt::Unchecked);
    }
}

void Book::restoreIconeState()
{
    setIcon( mpTreeItem, mIconState);
}

//Sets the cosmetic types of the book, by the given conf line
void Book::setCosmetics(QString confline)
{
    Cosmetics = confline.mid(14);

    QStringList costype;
    int pos = confline.indexOf("=");
    confline = confline.mid(pos + 1);
    costype = confline.split("#");

    //Handle 'CosmeticsType' confs:
    for(int j=0; j<costype.size(); j++)
    {
        vector <QString> costypevals;
        splittotwo(costype[j],costypevals,"=");

        int num, num2;

        //The rus[x] option, says names of the given level links should be trimmed until the given suffix
        if(costypevals[0].startsWith("rus"))
        {
            if ( ToNum((QString(costypevals[0][3])), &num) && num <= 5 && num > 0)
            {
                //In some sources there's a '_' instead of a ' '. this should deal with that.
                for(int k=0;k<costypevals[1].length();k++)
                    if(costypevals[1][k] == '_')
                        costypevals[1][k] = ' ';
                //(num - 1) because the array is from 0 - 4, but the levels it refers to are 1-5
                mRemoveSuffix[num - 1] = costypevals[1];
            }
        }
        //Short index level
        else if(costypevals[0] == "sil")
        {
            if(ToNum(costypevals[1], &num))
                mShortIndexLevel = num;
        }
        //Index prefix code (for the given level)
        else if(costypevals[0].startsWith("ip"))
        {
            if ( ToNum((QString(costypevals[0][2])), &num) && num <= 5 && num > 0)
            {
                //In some sources there's a '_' instead of a '&'. this should deal with that.
                for(int k=0; k<costypevals[1].length(); k++)
                    if(costypevals[1][k] == '_')
                        costypevals[1][k] = '&';
                mIndexPrefix[num - 1] = costypevals[1];
            }
        }
        //Index suffix code (for the given level)
        else if(costypevals[0].startsWith("ix"))
        {
            if ( ToNum((QString(costypevals[0][2])), &num) && num <= 5 && num > 0)
            {
                //In some sources there's a '_' instead of a '&'. this should deal with that.
                for(int k=0; k<costypevals[1].length(); k++)
                    if(costypevals[1][k] == '_')
                        costypevals[1][k] = '&';
                mIndexSuffix[num - 1] = costypevals[1];
            }
        }
        //Index suffix code (for the given level)
        else if(costypevals[0].startsWith("is"))
        {
            if ( ToNum((QString(costypevals[0][2])), &num) && num < 5 && num > 0)
            {
                //In some sources there's a '_' instead of a '&'. this should deal with that.
                if ( ToNum(QString(costypevals[1]), &num2))
                    mIndexTextSize[num - 1] = num2;
            }
        }
        //Index suffix code (for the given level)
        else if(costypevals[0].startsWith("sp"))
        {
            if ( ToNum((QString(costypevals[0][2])), &num) && num <= 5 && num > 0)
            {
                //In some sources there's a '_' instead of a '&'. this should deal with that.
                if ( ToNum(QString(costypevals[1]), &num2))
                    mTitleEmptyLines[num - 1] = num2;
            }
        }

        else if(costypevals[0].startsWith("rep"))
        {
            QString reper = costype[j].mid(costype[j].indexOf("=") + 1);
            QStringList p = reper.split(QString("^^"));
            for (int i=0; i<p.size(); i++)
            {
                vector<QString> pp;
                splittotwo(p[i], pp, "=");

                replaceFrom << pp[0];
                replaceTo << pp[1];
            }
        }

        costypevals.clear();
    }
    costype.clear();
}

//Returns a conf entry representing this book
QString Book::confEntry()
{
    if (mFiletype == Book::Dir) return "";

    QString conf = "";

    //conf += "[" + mPath + "]" + "\n";

    // ?
    conf += "DisplayName=" + mNormallDisplayName + "\n";

    conf += "SearchInTitles=" + QString(SearchInTitles ? "1" : "0") + "\n";
    conf += QString("LastLevelIndex=") + stringify(LastLevelIndex) + "\n";
    if (mPath.indexOf("משנה") != -1 && mPath.indexOf("סדר") != -1) conf += "PutNewLinesAsIs=0\n" + QString("0") + QString("\n");
    else conf += "PutNewLinesAsIs=" + QString(PutNewLinesAsIs ? "1" :  "0") + QString("\n");
    // ?
    conf += QString("PasukIndMode=0") + "\n";
    conf += QString("TextSource=") + mCopyrightInfo + "\n";
    conf += QString("GroupId=") + GroupId + "\n";
    conf += QString("ForcedBookName=") + mTreeDisplayName + "\n";
    // ?
    conf += QString("SpecialTitle=") + "\n";
    conf += QString("Comments=") + Comments + "\n";
    // ?
    conf += QString("ChainFolderName=1") + "\n";

    conf += QString("HiddenFromIndex=") + QString(mIsHidden ? "1" : "0") + QString("\n");

    //TODO: Re-Implement this
    //conf += QString("MixedDisplay=") + stringify(mWeavedSources) + "\n";

    conf += QString("CosmeticsType=") + Cosmetics + "\n";
    conf += QString("UniqueId=") + stringify(mUniqueId) + "\n";

    if (hasNikud) conf += QString("Nikud") + "\n";
    if (hasTeamim) conf += QString("Teamim") + "\n";

    conf += "\n";


    return conf;
}

//Returns the filename that should be used for the book, depending on the shown commentaries
QString Book::HTMLFileName()
{
    QString CommenteriesMagicString = "-"; //QString representing the commenteries state
    for (int i=1; i<mWeavedSources.size(); i++)
        CommenteriesMagicString += stringify(mWeavedSources[i].show);

    if (showAlone || mWeavedSources.size() == 0) CommenteriesMagicString = "";

    QString name = ( mUniqueId == -1 ? getNormallDisplayName() : stringify(mUniqueId) );
    QString htmlfilename = TMPPATH + name + CommenteriesMagicString;

    return htmlfilename;
}

/*
inline QString Book::DBFilePath()
{
    QString p = mPath;
    return p.replace(".txt", ".TDB");
}

inline QString Book::LMFilePath()
{
    QString p = mPath;
    return p.replace(".txt", ".LMP");
}
*/

QString Book::pureText()
{
    if (mPureText.simplified() == "" || mPureText == "Locked!")
    {        
        mPureText = ReadFileFromZip(mPath, "SearchDB", "UTF-8");
        if (mPureText == "Error!")
        {
            qDebug() << "Error! Couldn't read search DB for:" << mPath;
            mPureText = "";
        }
        else
        {
            levelMap.clear();
            setLevelMap(ReadFileFromZip(mPath, "LevelMap", "UTF-8"));
        }
    }

    return mPureText;
}

/*
void Book::setPureText(QString ptxt)
{
    writetofile(DBFilePath(), ptxt, "UTF-8", true);
    writetofile(LMFilePath(), levelMapString(), "UTF-8", true);
}
*/


//Build the levelMap from a string representing it
void Book::setLevelMap(QString str)
{
    if (str == "Error!")
    {
        qDebug() << "Couldn't read LevelMap for:" << mPath;
        return;
    }

    QList <QString> l = str.split("\n");

    for (int i=0; i<l.size(); i++)
    {
        QList <QString> p = l[i].split("->");

        if (p.size() == 2) levelMap.insert(p[0].toInt(), BookIter::fromString(p[1]));
    }
}

//Return a QString representing the levelMap
QString Book::levelMapString()
{
    QString str = "";

    QList <int> keys = levelMap.keys();
    QList <BookIter> vals = levelMap.values();

    for (int i=0; i<keys.length(); i++)
    {
        str += QString::number(keys[i]) + "->" + vals[i].toString() + "\n";
    }

    return str;
}

/*
bool Book::hasDBFile()
{
    QFile f(DBFilePath());

    if (!f.exists()) return false;

    if (!f.open(QIODevice::ReadOnly)) return false;

    f.read(25); //Just some number...
    if (f.atEnd()) return false;

    f.close();
    return true;
}
*/

/*
void Book::BuildSearchTextDB()
{
    //TODO: remove references to other books

    //No need to build the DB twice...
    if (hasDBFile()) return;

    //Ignore dirs
    if (mFiletype == Book::Normal || mFiletype == Book::Html)
    {
        QString str = "";

        //Lock the DB from other functions:
        //(Because once pureText isn't empty, the function exists right away)
        levelMap.clear();
        mPureText = "Locked!";
        setPureText("Locked!");

        //Read book's contents
        QStringList text;

        if (mFiletype == Book::Normal)
        {
            //if (!ReadFileToList(mPath, text, "UTF-8", true))
            if (!ReadFileFromZip(mPath, "BookText", text, "UTF-8", true))
            {
                qDebug() << "Error reading the book's text" << mPath;
                return ;
            }
        }

        else if (mFiletype == Book::Html)
        {
            QFile file(mPath);
            if (!file.open(QIODevice::ReadOnly))
            {
                qDebug() << "couldn't open " << mPath;
                return;
            }

            QByteArray data = file.read( file.size() );
            file.close();

            QTextCodec* codec = QTextCodec::codecForHtml(data, QTextCodec::codecForName("UTF-8"));
            text = codec->toUnicode(data).replace(QRegExp("<[^>]*>"), "").split('\n', QString::SkipEmptyParts);
        }


        BookIter itr;

        //If a line starts with one of these, it's a level sign
        QString levelSigns = "!@#$^~";
        //Any char not matchong this pattern, is no *pure* text.
        QRegExp notText("[^א-תa-zA-Z0-9 ()'\"]");
        //These are turned into spaces, and not just ignored.
        QString spaceSigns = "[-_:.,?]";

        //Pad with spaces, so "full word searches" work on begining and end of text too
        str = " ";

        for (int i=0; i<text.size(); i++)
        {
            //Level line
            if (levelSigns.contains(text[i][0]))
            {
                //Update iter
                //but if levelSign is '$' ???
                itr.SetLevelFromLine(text[i]);

                //Map with it's position in the pureText
                levelMap.insert(str.length(), itr);

                if (text[i][0] == '!')
                {
                    str += " " +  text[i].mid(2) + " ";
                }
                //else pureText += "</br>" + text[i].mid(2).replace("{", "(").replace("}",")");
            }
            //Link
            else if (text.startsWith("<!--ex"))
            {

            }
            //Text line
            else
            {
                //Test if book is from the bible. Hope this is ok...
                if ( mPath.contains("מקרא") )
                {
                    //Turn קרי וכתיב into only קרי. Maybe this should be an option?
                    text[i].replace( QRegExp ("[(][^)]*[)] [[]([^]]*)[]]"), "\\1");
                }

                //Remove html tags
                text[i].replace( QRegExp("<[^>]*>"), "" );

                //Remove all non "pure-text" chars
                text[i].replace(QChar(0x05BE), " "); //Makaf
                text[i].replace(QRegExp(spaceSigns), " ");
                text[i].replace("{", "(").replace("}",")");
                text[i].replace("nbsp", " ");
                text[i].replace(notText, "");

                //Remove double spaces and line breaks
                str += text[i].simplified() + " ";
            }
        }

        //Pad with spaces, so "full word searches" work on begining and end of text too
        str += " ";

        setPureText(str);

        mPureText = "";
        levelMap.clear();
    }
}
*/

QList <SearchResult> Book::findInBook(const QString& phrase)
{
    //Naive convert to regexp
    QRegExp regexp = QRegExp(phrase);
    return findInBook(regexp);
}

#define CHAR_LIMIT 250
QList <SearchResult> Book::findInBook(const QRegExp& exp)
{
    //qDebug() << "searching in: " << this->getNormallDisplayName();

    QList <SearchResult> results;
    QRegExp regexp = QRegExp("(" + exp.pattern()+ ")");

    if (pureText() == "") qWarning() << this->getNormallDisplayName() << " has no Search DB, so it couldn't be searched!";

    int curr = 0, last = 0;
    while ((curr = pureText().indexOf(exp, last)) != -1)
    {
        QMap<int, BookIter>::iterator mapitr = levelMap.upperBound(curr);

        //Prevent undefined no-where iterator
        if (mapitr != levelMap.begin()) mapitr --;

        SearchResult s;
        s.preview = resultPreview(regexp, curr);
        s.itr = ( !levelMap.empty() ? mapitr.value() : BookIter() );

        //Prevent double results
        //if (    results.empty()
        //    || (results.back().itr != s.itr)
        //    || (curr - last > CHAR_LIMIT/2) )
        results << s;

        last = curr+1;
    }

    // --- *** ---
    mPureText = "";
    levelMap.clear();

    return results;
}

QString Book::resultPreview(const QRegExp& exp, int offset)
{
    QString s = pureText().mid(offset - (CHAR_LIMIT/2), CHAR_LIMIT);
    //Force full words
    s = s.mid(s.indexOf(" "));
    s = s.mid(0, s.lastIndexOf(" "));
    s = "... " + s + " ...";
    return s.replace(exp, "<span style='background-color:Yellow'>\\1</span>");
}

//Returns a bookiter representing the chapter coming before/after the given chapter.
BookIter Book::nextChap(BookIter iter)
{
    if (!isBookOpen()) readBook(LIL);

    //Find this iter
    int n = -1;
    for (int j=0; j < chapterIter.size(); j++)
    {
        if (iter == chapterIter[j]) n = j;
    }

    if (n != -1 && n + 1 < chapterIter.size()) return chapterIter[n+1];

    else return iter;
}

BookIter Book::prevChap(BookIter iter)
{
    //Who said 1 is default?
    if (!isBookOpen()) readBook(1);

    //Find this iter
    int n = -1;
    for (int j=0; j < chapterIter.size(); j++)
    {
        if (iter == chapterIter[j]) n = j;
    }

    if (n > 0) return chapterIter[n-1];

    else return iter;
}

//Returns True if readbook was called on the book, and closeBook() wasn't
bool Book::isBookOpen() { return (chapterIter.size() > 0) ; }

QDateTime Book::modificationDate()
{
    QFileInfo f(mPath);
    return f.lastModified();
}
