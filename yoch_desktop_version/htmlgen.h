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
* Author: Yoch Melka. <yoch.melka@gmail.com>
*/

#ifndef HTMLGEN_H
#define HTMLGEN_H

#include "treeitem_orayta.h"
#include "bookiter.h"
#include "functions.h"
#include "mainwindow.h"



//Helper struct and functions for generating the html of the book

struct weavedSourceData
{
    QList <QString> text;
    QString Title;
    QString FileName;
    QString str;
    QString Prefix;
    QString Suffix;
    BookIter itr;
    QFont font;
    int line;
};

//Struct holding information about a link to a position in the page
struct IndexItem
{
    QString displayText;
    QString linkPoint;
    //The level of the link.
    // (Used for deciding how the links should be formatted at the top of the page)
    int level;
};

//Returns a string that will be the header of the output HTML file
QString html_head(const QString& title, const QFont &font);

//Generates and returns the Title of the book
QString html_book_title(QString , QString, QString);

//Generates and returns the Index ('Tochen') of the HTML file
QString html_link_table(const std::vector<IndexItem>&, int short_index_level, bool dot , bool hasRUS = false);

//Generates an index to all LINKS with the given level (NOTE: links to links, not to text)
QString index_to_index(const std::vector<IndexItem>&, int short_index_level);

//Generates the div declaration for the top of the html file
//inline QString html_main_div( QString fontFamily, int fontSize );


//Returns true if the given line has either a empty line after it,
// or only lines with "//" and an empty line at the end of them.
// (Recusive function). Used to recognize the 'special name' links.
bool space_or_slash_after_this(const std::vector <QString>& text, int line);

//Returns an html link ("<a href") by the given link_to and name.
QString link(QString linkto, QString displaytext, int id = 0);

//Return html code of dots:
QString bluedot();
QString reddot();


QString basicCSS(const QFont&);
QString oraytaScripts();

//Returns the Html code for the given external link
QString ExternalLink(QString linkcode);

//I have no idea how this works, it just does.
// Many thanks to the developer of the originall program
std::string Decrypt (std::string text, bool decrypt);


#endif //#define HTMLGEN_H
