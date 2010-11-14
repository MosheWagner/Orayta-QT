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


//TODO: Make sure bookiters are intiallized


//Constructor of the Books
Book::Book(Book * parent, QString path, QString name, QString displayname, bool is_dir)
{
    mUniqueId = -1;
    mNormallDisplayName = displayname;
    mpParent = parent;
    mPath = path;
    //In most cases this will be overriden when rendered
    mNameForTitle = name;

    mIsDir = is_dir;

    mpTreeItem = NULL;

    for(int i=0;i<5;i++)
    {
        mRemoveSuffix[i]="";
        mTitleEmptyLines[i] = 0;
        mIndexSuffix[i] = "";
        mIndexPrefix[i] = "";
    }

    mShortIndexLevel = 0;

    mInSearch = true;
    mIconState = BLUE;

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

    //pureText = "";
}

Book::~Book()
{
    delete mpTreeItem;
}


// Book Getters

QString Book::getPath()
{   return mPath;  }

int Book::getUniqueId()
{   return mUniqueId; }

QString Book::getNormallDisplayName()
{ return mNormallDisplayName;         }

QString Book::getTreeDisplayName()
{ return mTreeDisplayName;         }

QString Book::getName()
{   return mNameForTitle;  }

bool Book::IsHidden()
{ return mIsHidden; }

bool Book::IsDir()
{   return mIsDir;    }

bool Book::IsInSearch()
{  return mInSearch; }

bool Book::isMixed()
{ return (mWeavedSources.size() != 0); }

QTreeWidgetItem * Book::getTreeItemPtr()
{   return mpTreeItem; }

// Book Setters:
void Book::setPath(QString path)
{   mPath = path;  }

void Book::setName(QString name)
{   mNameForTitle = name;  }

void Book::setNormallDisplayName(QString displayname)
{ mNormallDisplayName = displayname;  }

void Book::setTreeDisplayName(QString treedisplayname)
{ mTreeDisplayName = treedisplayname;  }

void Book::setIsDir(bool is_dir)
{   mIsDir = is_dir;  }

void Book::setParent(Book * parent)
{   mpParent = parent;  }

Book *  Book::getParent()
{   return mpParent;          }

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

void Book::repainticon()
{
    //UnSelectable should allways be grey
    //if (mIsUnSelectable) setIcon(mpTreeItem, GREY);

    bool blue_child = false, grey_child = false, mixed_child = false;

    //Check children
    for (unsigned int i=0;i < mvChildren.size(); i++)
    {
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

    if ((blue_child == false) && (mixed_child == false))
    {
        //Dark icon
        setIcon(mpTreeItem, GREY);
    }
    else if (( grey_child == false) && (mixed_child == false))
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
    mInSearch = false;

    //Darken icon
    setIcon(mpTreeItem, GREY);

    //Unselect all children
    for (unsigned int i=0;i < mvChildren.size(); i++)
        mvChildren[i]->_unselect();
}

void Book::unselect()
{
    this->_unselect();

    //Repaint parent (it will tell it's parent to repaint too, and so on...)
    if(mpParent != NULL)
        mpParent->repainticon();
}

void Book::_select()
{
    mInSearch = true;

    //Make icon blue
    setIcon(mpTreeItem, BLUE);

    //select all children
    for (unsigned int i=0;i < mvChildren.size(); i++)
        mvChildren[i]->_select();
}

void Book::select()
{
    this->_select();
    //Repaint parent (it will tell it's parent to repaint too, and so on)
    if(mpParent != NULL)
        mpParent->repainticon();
}

void Book::setIcon(QTreeWidgetItem *TreeItem, IconState newiconstate)
{
    if (TreeItem == NULL) return;

    //set icon state
    mIconState = newiconstate;
    QIcon *icon;
    icon = bookIcon(IsDir(), isMixed(), mIconState);
    TreeItem->setIcon(0, *icon);
    delete icon;

    //Show state in the checkbox too
    if (mIconState == BLUE)
        TreeItem->setCheckState(0,Qt::Checked);
    else if (mIconState == HALF)
        TreeItem->setCheckState(0,Qt::PartiallyChecked);
    else if (mIconState == GREY)
        TreeItem->setCheckState(0,Qt::Unchecked);
}


//TODO: use the "Get*Value" funcs...

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
        if(costypevals[0].mid(0,3) == "rus")
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
        else if(costypevals[0].mid(0,2) == "ip")
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
        else if(costypevals[0].mid(0,2) == "ix")
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
        else if(costypevals[0].mid(0,2) == "is")
        {
            if ( ToNum((QString(costypevals[0][2])), &num) && num < 5 && num > 0)
            {
                //In some sources there's a '_' instead of a '&'. this should deal with that.
                if ( ToNum(QString(costypevals[1]), &num2))
                    mIndexTextSize[num - 1] = num2;
            }
        }

        //Index suffix code (for the given level)
        else if(costypevals[0].mid(0,2) == "sp")
        {
            if ( ToNum((QString(costypevals[0][2])), &num) && num <= 5 && num > 0)
            {
                //In some sources there's a '_' instead of a '&'. this should deal with that.
                if ( ToNum(QString(costypevals[1]), &num2))
                    mTitleEmptyLines[num - 1] = num2;
            }
        }

        else if(costypevals[0].mid(0,3) == "rep")
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
    if (mIsDir == TRUE) return "";

    QString conf = "";

    //conf += "[" + mPath + "]" + "\n";

    // ?
    conf += "DisplayName=" + mNormallDisplayName + "\n";

    conf += "SearchInTitles=" + QString(SearchInTitles ? "1" : "0") + "\n";
    conf += QString("LastLevelIndex=") + stringify(LastLevelIndex) + "\n";
    if (mPath.indexOf("משנה") != -1 && mPath.indexOf("סדר") != -1) conf += "PutNewLinesAsIs=" + QString("0") + QString("\n");
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
    for (int i=1; i<mWeavedSources.size(); i++) CommenteriesMagicString += stringify(mWeavedSources[i].show);

    if (showAlone || mWeavedSources.size() == 0) CommenteriesMagicString = "";

    QString htmlfilename = TMPPATH + stringify(mUniqueId) + CommenteriesMagicString;

    return htmlfilename;
}






/*
void Book::BuildSearchTextDB()
{


    //TODO: remove non-link references to other books (?)

    //TODO: still much to be done

    //TODO: test thoroughly!


    //Ignore dirs
    if (mIsDir) return;

    //No need to build the DB twice...
    if (pureText() != "") return ;

    //Lock the DB from other functions:
    //(Because once pureText isn't empty, the function exists right away)
    setPureText("Locked!");

    //Read book's contents
    QStringList text;
    if (!ReadFileToList(mPath, text, "UTF-8", true))
    {
        qDebug() << "Error reading the book's text" << mPath;
        return ;
    }

    //If a line starts with one of these, it's a level sign
    QString levelSigns = "!@#$^~";
    //Any char not matching this pattern, is no puretext.
    //QRegExp notText("[^א-תa-zA-Z0-9 ]");
    //These are turned into spaces, and not just ignored.
    QString spaceSigns = " ־-_'::.,?";


    QString hebrew = "אבגדהוזחטיכלמנסעפצקרשתםןץףך";
    QString ptxt = " ";
    //The first char has no source in the real text
    Map.append(-1);
    for (int i=0; i<text.size(); i++)
    {
        //Level line or link -> Ignore
        if (levelSigns.contains(text[i][0]) || text[i].startsWith("<!--ex"))
        {
            //
        }
        //Text line
        else
        {
            //Test if book is from the bible. Hope this is ok...
            if (mPath.contains("מקרא") == true)
            {
                //Turn קרי וכתיב into only קרי. Maybe this should be an option?
                text[i] = text[i].replace( QRegExp ("[(][^)]*[)] [[]([^]]*)[]]"), "\\1");
            }

            //Add only hebrew (or space-like) chars to the puretxt, and map thier originall position
            for (int j=0; j<text[i].size(); j++)
            {
                if (hebrew.contains(text[i][j]))
                {
                    ptxt += text[i][j];
                    Map.append(i);
                }
                else if (spaceSigns.contains(text[i][j]))
                {
                    if (ptxt[ptxt.size()-1] != ' ') ptxt += ' ';
                    Map.append(i);
                }
            }
        }
    }


    //Pad with spaces, so "full word searches" work on begining and end of text too
    ptxt += " ";

    setPureText(ptxt);


}

QList <SerachResult> Book::findInBook(QString phrase)
{
    //Naive convert to regexp
    return findInBook(QRegExp(phrase));
}

QList <SerachResult> Book::findInBook(QRegExp exp)
{


    QList <SerachResult> results;

    //Make sure DB exists
    BuildSearchTextDB();


    //Wait for the DB to unlock
    while (pureText() == "Locked!") {};

    int j = 0;

    QString ptxt = pureText();

    while ((j = ptxt.indexOf(exp, j)) != -1)
    {
        SerachResult s;
        //s.preview = resultPreview(exp, j);
        //s.itr = itrFromOffset(upper_bound(Map.begin(), Map.end(), j));

        QMap<int, BookIter>::iterator mapitr = levelMap.upperBound(j);
        mapitr --;

        SerachResult s;
        s.preview = resultPreview(exp, j);
        s.itr = mapitr.value();


        //Prevent double results
        if (results.isEmpty()) results << s;
        else if (results[results.size()-1].itr.toStringForLinks() != s.itr.toStringForLinks()) results << s;

        ++j;
    }

    return results;


}

#define CHAR_LIMIT 200
QString Book::resultPreview(QRegExp exp, int offset)
{

    QString s = pureText().mid(offset - (CHAR_LIMIT/2), CHAR_LIMIT);

    //Force full words
    s = s.mid(s.indexOf(" "));
    s = s.mid(0, s.lastIndexOf(" "));
    s = "... " + s + " ...";
    return s.replace(exp, "<span style='color:Yellow'>" + exp.cap(0) + "</span>");
}


QString Book::pureText()
{
    //return mPureText;
    return readfile(TMPPATH + "/" + stringify(mUniqueId) + ".TDB", "UTF-8");
}

void Book::setPureText(QString ptxt)
{
    //mPureText = ptxt;
    writetofile(TMPPATH + "/" + stringify(mUniqueId) + ".TDB", ptxt, "UTF-8", true);
}

BookIter Book::itrFromOffset(QString posString)
{
    BookIter itr;

    //Read book's contents
    QStringList text;
    if (!ReadFileToList(mPath, text, "UTF-8", true))
    {
        qDebug() << "Error reading the book's text" << mPath;
        return itr;
    }

    bool ok = true;
    QStringList p = posString.split(":");
    int line = p[0].toInt(&ok);

    if (!ok) return itr;

    //If a line starts with one of these, it's a level sign
    QString levelSigns = "!@#$^~";


    for (int i=0; i<line; i++)
    {
        if (levelSigns.indexOf(text[i][0]) != -1 )
        {
            //Advance the book itr to the new position
            itr.SetLevelFromLine(text[i]);
        }
    }

    return itr;
}

*/


void Book::BuildSearchTextDB()
{
    //TODO: remove references to other books

    //TODO: still much to be done

    //TODO: test thoroughly!

    //Ignore dirs
    if (mIsDir) return;

    //No need to build the DB twice...
    if (pureText != "") return ;

    //Read book's contents
    QStringList text;
    if (!ReadFileToList(mPath, text, "UTF-8", true))
    {
        qDebug() << "Error reading the book's text" << mPath;
        return ;
    }


    BookIter itr;

    //If a line starts with one of these, it's a level sign
    QString levelSigns = "!@#$^~";
    //Any char not matchong this pattern, is no *pure* text.
    QRegExp notText("[^א-תa-zA-Z0-9 ]");
    //These are turned into spaces, and not just ignored.
    QString spaceSigns = "-_'::.,?";

    for (int i=0; i<text.size(); i++)
    {
        //Level line
        if (levelSigns.contains(text[i][0]))
        {
            //Update iter
            itr.SetLevelFromLine(text[i]);

            //Map with it's position in the pureText
            levelMap.insert(pureText.length(), itr);

            pureText += " ";
        }
        //Link
        else if (text.startsWith("<!--ex"))
        {

        }
        //Text line
        else
        {
            //Test if book is from the bible. Hope this is ok...
            if (mPath.contains("מקרא") == true)
            {
                //Turn קרי וכתיב into only קרי. Maybe this should be an option?
                text[i] = text[i].replace( QRegExp ("[(][^)]*[)] [[]([^]]*)[]]"), "\\1");
            }

            //Remove html tags
            text[i] = text[i].replace( QRegExp("<[^>]*>"), "" );

            //Remove all non "pure-text" chars
            text[i] = text[i].replace(QChar(0x05BE), " "); //Makaf
            text[i] = text[i].replace(spaceSigns, " ");
            text[i] = text[i].replace(notText, "");

            //Remove double spaces and line breaks
            pureText += text[i].simplified();
        }
    }

    //Pad with spaces, so "full word searches" work on begining and end of text too
    pureText = " " + pureText + " ";
}

QList <SearchResult> Book::findInBook(QString phrase)
{
    //Naive convert to regexp
    return findInBook(QRegExp(phrase));
}

QList <SearchResult> Book::findInBook(QRegExp exp)
{
    QList <SearchResult> results;

    //Make sure DB exists
    BuildSearchTextDB();

    int j = 0;
    while ((j = pureText.indexOf(exp, j)) != -1)
    {
        QMap<int, BookIter>::iterator mapitr = levelMap.upperBound(j);
        mapitr --;

        SearchResult s;
        s.preview = resultPreview(exp, j);
        s.itr = mapitr.value();

        //Prevent double results
        if (results.size() == 0) results << s;
        else if (results[results.size()-1].itr.toStringForLinks() != s.itr.toStringForLinks()) results << s;

        ++j;
    }


    // --- *** ---
    pureText = "";
    levelMap.clear();


    return results;
}

#define CHAR_LIMIT 200
QString Book::resultPreview(QRegExp exp, int offset)
{
    QString s = pureText.mid(offset - (CHAR_LIMIT/2), CHAR_LIMIT);
    //Force full words
    s = s.mid(s.indexOf(" "));
    s = s.mid(0, s.lastIndexOf(" "));
    s = "... " + s + " ...";
    return s.replace(exp, "<span style='background-color:Yellow'>" + exp.cap(0) + "</span>");
}
