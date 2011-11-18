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


//TODO: add gereshn or gershaiim to posiotion's toString function

#include "bookiter.h"

BookIter::BookIter()
{
    mLevelName[0] = "";
    mLevelName[1] = "";
    mLevelName[2] = "";
    mLevelName[3] = "";
    mLevelName[4] = "";
}

//Copy constructor, making this itr equivalent to the given one
BookIter::BookIter(BookIter * other_book)
{   
    for (int i=0; i<5; i++)
    {
        this->mLevelName[i] = other_book->mLevelName[i];
    }
}


BookIter::~BookIter() {}

bool BookIter::operator==(const BookIter& other)
{
    for (int i=0; i<5; i++)
    {
        if ( mLevelName[i] != other.mLevelName[i])
            return false;
    }
    return true;
}

bool BookIter::operator!=(const BookIter& other)
{
    return !(*this == other);
}


//Gives the level symbolized by the first char of the string,
// the value written in the rest of it .
// ( I.E., "~ פרק יא ", will set  level 2 to "פרק יא" ).
void BookIter::SetLevelFromLine(const QString& level_line)
{
    if (level_line.size() > 2) //It must have at least a symbol, a space, and then a value
    {
        QChar symbol = level_line[0];
        if (symbol == '!')
        {
            mLevelName[0] = removeSigns(level_line.mid(2));
        }
        else if (symbol == '~')
        {
            mLevelName[1] = removeSigns(level_line.mid(2));
            ZeroLevel(0);
        }
        else if (symbol == '@')
        {
            mLevelName[2] = removeSigns(level_line.mid(2));
            ZeroLevel(1);
        }
        else if (symbol == '^')
        {
            mLevelName[3] = removeSigns(level_line.mid(2));
            ZeroLevel(2);
        }
        else if (symbol == '#')
        {
            mLevelName[4] = removeSigns(level_line.mid(2));
            ZeroLevel(3);
        }
    }
}


// When values are 1 level apart, they are related  (like perek and pasuk)
// so when the more significant one is increased, the less significant one is set to zero.
// But when they are 2 or more apart, they are not related (like perek and daf), And don't effect each other
// So if level 4 is increased, if level 3 is not 0, it is zerod, and level 2 is zeroed too.
// (because it is affected by level 3). And so on, until level 1 is zeroed.
// but if 3 was zero, 2 is not zeroed, because it is not realated to 4, and the chain stops.
void BookIter::ZeroLevel(int level)
{
    if ( mLevelName[level] != "") mLevelName[level] = "0";

    //Zero the one right under him
    if ( level != 0)
    {
        if ( mLevelName[level] != "")
            ZeroLevel(level-1);
    }
}

//Returns a string representing the current position
// (for html links and name).
QString BookIter::toStringForLinks(int from_level)
{
    int lastlevel = 100;
    QStringList relevantValues;

    //Find all continuous level values, starting from the given level (-1 because the array is 1 lower) or 0.

    //Find all values of continuous levels , starting from 0
    for (int i=0; i<5; i++)
    {
        if( (mLevelName[i] != "") && (mLevelName[i] != "0") )
        {
            if ( (relevantValues.size() == 0) || (i - lastlevel == 1) )
            {
                lastlevel = i;
                relevantValues.prepend( mLevelName[i] );
            }
        }
    }

    //But, if the level given to the function is not one of those found,
    // start all over again, this time from the given level
    if (lastlevel < from_level - 1)
    {
        relevantValues.clear();
        lastlevel = 100;

        for (int i = from_level-1; i<5; i++)
        {
            if( (mLevelName[i] != "") && (mLevelName[i] != "0") )
            {
                if ( (relevantValues.size() == 0) || (i - lastlevel == 1) )
                {
                    lastlevel = i;
                    relevantValues.prepend( mLevelName[i] );
                }
            }
        }
    }

    //Join all relevant values, and make thier spaces into '_':
    QString str = relevantValues.join("-").replace(' ','_');

    //Escape the position so it has no hebrew letters, because QT can't deal with hebrew links (!)
    return escapeToBase32(removeSigns(str));
}

QString BookIter::toHumanString()
{
    int lastlevel = 100;
    QStringList relevantValues;

    //Find all continuous level values, starting from the lowest
    for (int i=0; i<5; i++)
    {
        if( (mLevelName[i] != "") && (mLevelName[i] != "0") )
        {
            if ( (relevantValues.size() == 0) || (i - lastlevel == 1) )
            {
                lastlevel = i;
                relevantValues.prepend( mLevelName[i] );
            }
        }
    }

    QString str = relevantValues.join(" ").replace('-',' ');

    str = RemoveBrackets(str);

    return str;
}

//Returns a string representing the current position of gmara pages only,
// In the way it should be shown to the user (such as "טז:" ).
QString BookIter::toGmaraString()
{
    QString str;
    vector <QString> tmp;

    unsigned int i;
    //Find lowest level with value
    for (i=0; !mLevelName[i].contains("דף ") && i < 5; i++);

    if (i < 5)
    {
        int p = mLevelName[i].indexOf("דף ");
        str = mLevelName[i].mid(p+3);
        splittotwo(str, tmp, "-");

        int daf = GematriaValueOfString(tmp[0]) * 2;
        int amud = GematriaValueOfString(tmp[1]) - 1;

        return GmaraPage( daf + amud );
    }
    else
    {
        return toHumanString();
    }
}
