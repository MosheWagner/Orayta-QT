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

#ifndef FUNCTIONS_H
#define FUNCTIONS_H

#include <QString>
#include <vector>
#include <algorithm>
#include <iostream>
#include <QFile>
#include <QFileInfo>
#include <QTextCodec>
#include <QTextStream>
#include <QStringList>
#include <QRegExp>
#include <QIcon>
#include <QTranslator>

#ifdef POPPLER
    #include <fribidi/fribidi.h>
#endif


//Enum defining the color of the folder icon.
// Blue is when all it's children are selected;
// Grey when none are;
// And half grey and half blue - when some are, and some are not.
enum IconState { BLUE, HALF, GREY};

using namespace std;

extern QString LANG;

extern QString BOOKPATH;
extern QString TMPPATH;
extern QString HEBREWBOOKSPATH;
extern QString USERPATH;

#define VERSION "0.05"

//Small functions used by a few files:

//Reads the file with the given name and the given encoding, and inserts it's contents into the given vector
bool ReadFileToList(QString filename, QList <QString>& text, const char* encoding_name, bool skipconflines = false);

//Reads the comment file into two vectors, one for the titles and one for the texts.
// If a book's id is given, only comments for that book are put into the vector, with the "id:" part of the line choped.
bool ReadCommentFile(QString path, vector<QString>& titles, vector<QString>& texts, const char* encoding_name, int id=-1);

//Writes the given data to the given file path with the given encoding
void writetofile(QString filename, const QString& data, const char* encoding_name, bool overwrite = true);
QString readfile(QString filename, const char* encoding_name);

//Splits the given QString to two parts, (ONLY) on the FIRST occurrence of the splitter
void splittotwo(QString , vector <QString>& , QString);

//Sets the value of the pointed int to the value represented by the string
//  Returns failure or success
bool ToNum(QString str, int *out);

//Prints (couts) the given Qstring
void print(QString);

//Prints (couts) the given int
void print(int);

//Returns the Gematria of the given string
int GematriaValueOfString (const QString& str);

//Returns the gematria of the single given hebrew char
inline int GematriaOfChar(const QChar ch);

//Returns a hebrew string representing the given gematria value
// You can tell it weather to add quote signs (Geresh or Gershaiim) or not
QString NumberToGematria (int num, bool addquotes = true);

//Return only the hebrew letters in the given string, and makes every space into "_"
QString RemoveBrackets(QString str);

//Converts numbers into strings:
// (Returns a QString representing the given value)
QString stringify(double);

//Returns gmara page name of the given page number (From 4, which is Bet Amud Alef)
QString GmaraPage(int num);

//Makes sure the given filepath is absolute
QString absPath(QString);

//Because for some stupid reason QT can't handle internal html links with Hebrew chars, this function
// (which is called by the itr's toString), converts any given QString to a string representing each char's
// Base32 value, (seperated by 'U's), thus giving a valid digit-or-english-letter-only string.
QString escapeToBase32(QString str);
QString unescapeFromBase32(QString str);

// Returns a string the search should display for this book and position
///QString BookSearchDisplay (QString BookDisplayName , QString PositionName);

// Puts into the vector, all words of the first string,
//  excluding any words that are also in the second.
void WordExclude(QString stra , QString strb, vector<QString>& out);

//Gets a line in the form of foo=bar,
// and iserts bar to the pointed integer (if it's a valud integer)
// (Otherwise the value isn't changed)
void GetIntValue (const QString& valueline, int * pVar);

//Gets a line in the form of foo="bar",
// and iserts bar to the pointed qstring, unless it's empty
void GetStringValue (const QString& valueline, QString * pStr);

//Fix the syntax of the 'span' attributes given in source files
    //Fixes the double single quotes to a single double quote
    //Fixes the '=' sign given with "color=", to "color:"
QString fixSpan(QString str);

//Returns the given string the given number of times
QString stringTimes(const QString& str, int t);

//Limits the given string to the given length, removing the beggining of the string.
// Cuts only at spaces.
QString startChop(const QString& str, int limit);
//Limits the given string to the given length, removing the end of the string.
// Cuts only at spaces.
QString endChop(const QString& str, int limit);


//Removes any sign from the given unicode string
// (Nikud or teamim)
QString removeSigns(const QString& str);

//Removes any nikud sign from the given unicode string
QString removeNikud(const QString&);

//Removes any teamim sign from the given unicode string
QString removeTeamim(const QString& str);

QString allowNikudAndTeamim( const QString& str );
QRegExp withNikudAndTeamim( const QString& str );

////Returns a regexp pattern matching all strings that are the same as the given string,
////  ignoring differences in the ו-s and י-s they have.
//QString AllowKtivMaleh(QString str);

QString AllowKtivHasser(const QString& str);


QString simpleHtmlPage(QString title, QString contents);
QString pluginPage(QString title);

//Copy the given folder (recursively) to the given path
void copyFolder(QString sourceFolder, QString destFolder, QStringList fileNameFilters);
//delete the given folder (recursively) to the given path
void deleteBooksFolder(QString sourceFolder);

#ifdef POPPLER

// Returns a corrected Bidi string from the given one.
// (Using the 'LineToBidiText' function)
QString ToBidiText(QString str);

// Returns a corrected single line Bidi string from the given one.
// (Using fribidi's functions)
QString LineToBidiText(QString str);

#endif //POPPLER

#endif // FUNCTIONS_H
