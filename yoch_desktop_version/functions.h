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
#include <QHash>
#include <QFile>
#include <QFileInfo>
#include <QTextCodec>
#include <QTextStream>
#include <QStringList>
#include <QRegExp>
#include <QIcon>
#include <QTranslator>



//Enum defining the color of the folder icon.
// Blue is when all it's children are selected;
// Grey when none are;
// And half grey and half blue - when some are, and some are not.



//Small functions used by a few files:

//Reads the file with the given name and the given encoding, and inserts it's contents into the given vector
bool ReadFileToList(QString filename, QList<QString>&, const char* encoding_name, bool skipconflines = false);

//Reads the comment file into two vectors, one for the titles and one for the texts.
// If a book's id is given, only comments for that book are put into the vector, with the "id:" part of the line choped.
bool ReadCommentFile(QString path, std::vector<QString>& titles, std::vector<QString>& texts, const char* encoding_name, int id = -1);

QHash<QString,QString> getCommentsForBookId(QString path, int id, const char *encoding_name = "UTF-8");

//Writes the given data to the given file path with the given encoding
void writetofile(QString filename, const QString& data, const char* encoding_name, bool overwrite = true);
QString readfile(QString filename, const char* encoding_name);

// quazip functions
QString ReadFileFromZip(QString zippath, QString filepath, const char* encoding_name);
bool ReadFileFromZip(QString zippath, QString filepath, QList <QString>& text, const char* encoding_name, bool skipconflines);
bool ReadZipComment(QString zippath, QList <QString>& text, const char* encoding_name);


//Splits the given QString to two parts, (ONLY) on the FIRST occurrence of the splitter
QStringList splittotwo(const QString& str, const QString& sep);

//Sets the value of the pointed int to the value represented by the string
//  Returns failure or success
bool ToNum(QString str, int *out);

//Returns the Gematria of the given string
int GematriaValueOfString (const QString& str);

//Returns the gematria of the single given hebrew char
inline int GematriaOfChar(const QChar ch);

//Returns a hebrew string representing the given gematria value
// You can tell it weather to add quote signs (Geresh or Gershaiim) or not
QString NumberToGematria (int num, bool addquotes = true);

//Return only the hebrew letters in the given string, and makes every space into "_"
QString RemoveBrackets(QString str);

//Returns gmara page name of the given page number (From 4, which is Bet Amud Alef)
QString GmaraPage(int num);

//Makes sure the given filepath is absolute
QString absPath(QString);

//Because for some stupid reason QT can't handle internal html links with Hebrew chars, this function
// (which is called by the itr's toString), converts any given QString to a string representing each char's
// Base64 value, (seperated by 'U's), thus giving a valid digit-or-english-letter-only string.
QString escapeToBase64(const QString& str);
QString unescapeFromBase64(const QString& str);


//Gets a line in the form of foo=bar,
// and iserts bar to the pointed integer (if it's a valud integer)
// (Otherwise the value isn't changed)
void GetIntValue (const QString& valueline, int * pVar);


//Removes any sign from the given unicode string
// (Nikud or teamim)
QString removeSigns(const QString&);

//Removes any nikud sign from the given unicode string
QString removeNikud(const QString&);

//Removes any teamim sign from the given unicode string
QString removeTeamim(const QString&);

QString allowNikudAndTeamim( const QString& str );
QRegExp withNikudAndTeamim( const QString& str );

////Returns a regexp pattern matching all strings that are the same as the given string,
////  ignoring differences in the ו-s and י-s they have.
//QString AllowKtivMaleh(QString str);

QString AllowKtivHasser(const QString& str);

QString rangeToString(int a, int b);

QString simpleHtmlPage(QString title, QString contents);

//QString pluginPage(QString title);

//Copy the given folder (recursively) to the given path
void copyFolder(QString sourceFolder, QString destFolder, QStringList fileNameFilters);

//delete the given folder (recursively) to the given path
void deleteBooksFolder(QString sourceFolder);

void deleteOraytaBook(QString bookPath);


#endif // FUNCTIONS_H
