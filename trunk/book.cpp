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

    mIsUnSelectable = false;

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

bool Book::IsUnSelectable()
{ return mIsUnSelectable; }

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

void Book::setIsUnSelectable(bool un)
{ mIsUnSelectable = un; }

void Book::repainticon()
{
    //UnSelectable should allways be grey
    //if (mIsUnSelectable) setIcon(mpTreeItem, GREY);


    bool blue_child = false, grey_child = false, mixed_child = false;

    //Check children
    for (unsigned int i=0;i < mvChildren.size(); i++)
    {
        if ( mvChildren[i]->mIsHidden == false && mvChildren[i]->IsUnSelectable() == false)
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

void Book::unselect()
{
    //if (!mIsUnSelectable)
    //{
        mInSearch = false;

        //Darken icon
        setIcon(mpTreeItem, GREY);

        //Unselect all children
        for (unsigned int i=0;i < mvChildren.size(); i++)
            mvChildren[i]->unselect();

        //Repaint parent (it will tell it's parent to repaint too, and so on...)
        if(mpParent != NULL)
            mpParent->repainticon();
    //}
}

void Book::select()
{
    //if (!mIsUnSelectable)
    //{
        mInSearch = true;

        //Make icon blue
        setIcon(mpTreeItem, BLUE);

        //Unselect all children
        for (unsigned int i=0;i < mvChildren.size(); i++)
            mvChildren[i]->select();

        //Repaint parent (it will tell it's parent to repaint too, and so on)
        if(mpParent != NULL)
            mpParent->repainticon();
    //}
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

    vector <QString> costype;
    int pos = confline.indexOf("=");
    confline = confline.mid(pos + 1);
    split(confline , costype , "#");

    //Handle 'CosmeticsType' confs:
    for(unsigned int j=0;j<costype.size();j++)
    {
        vector <QString> costypevals;
        split(costype[j],costypevals,"=");

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
