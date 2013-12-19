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

//Symbols representing level marks in the text. '!' is the lowest (1), and '#' is the highest (5)
const QString LevelSigns = "!~@^#";

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
    if(! other_book)
        BookIter();
    else
    {
        for (int i=0; i<5; i++)
        {
            this->mLevelName[i] = other_book->mLevelName[i];
        }
    }
}

BookIter::~BookIter() {}

bool BookIter::operator==(const BookIter& other)
{
    bool hadValue = false ;
    for (int i=0; i<5; i++)
    {
        QString a = mLevelName[i].replace("0", "").simplified();
        QString b = QString(other.mLevelName[i]).replace("0", "").simplified();

        //New experimental thingy. If we reached a non-important level, then ignore it.
        //This could be a great hack to solve the parashah problems
        //if (a != "") hadValue = true;
        //if (a == "") if (hadValue) break;

        if ( a != b)
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
        int level = LevelSigns.indexOf(level_line[0]);

        if (level != -1)
        {
            mLevelName[level] = removeSigns(level_line.mid(2)).simplified();

            if (level > 0) ZeroLevel(level - 1);
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

//Returns a full representation of the bookiter,
// in a way it could be reconstructed from it.
QString BookIter::toString(int fromLevel)
{
    QString s = "";
    for (int i=0; i<5; i++)
    {
        s += "*";
        if (i >= fromLevel) s += mLevelName[i].replace("0", "");
    }

    return s;
}

//Returns a full representation of the bookiter,
// in a way it could be reconstructed from it.
QString BookIter::toEncodedString(int fromLevel)
{
    QString s = this->toString();

    s = escapeToBase32(s);

    return s;
}

//Return a BookIter from the given 'itr.toString()' string
BookIter BookIter::fromString(QString str)
{
    BookIter iter;

    if (str.mid(0,1) == "*" || str.mid(0,1) == " ") str = str.mid(1);

    QList <QString> l = str.split("*");

    for (int i=0; i<l.size() && i < 5; i++)
    {
        iter.mLevelName[i] = (l[i]);
    }

    return iter;
}

//Return a BookIter from the given 'itr.toEncodedString()' string
BookIter BookIter::fromEncodedString(QString str)
{
    str = unescapeFromBase32(str);

    return BookIter::fromString(str);
}


//Returns a string representing the position of the itr formatted for display
QString BookIter::humanDisplay()
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

//Returns a string representing the position of the itr formatted in gmara page style
QString BookIter::gmaraDisplay()
{
    QString str;
    vector <QString> tmp;

    unsigned int i;
    //Find lowest level with value
    for (i=0; i < 5 && !mLevelName[i].contains("דף "); i++);

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
        return humanDisplay();
    }
}

//Returns true if all levels of this iter are empty.
bool BookIter::isEmpty()
{
    for (int i=0; i<5; i++) { if (mLevelName[i] != "") return false; }
    return true;
}
