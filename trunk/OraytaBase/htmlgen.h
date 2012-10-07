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

#ifndef HTMLGEN_H
#define HTMLGEN_H

#include "book.h"
#include "bookiter.h"
#include "functions.h"

using namespace std;



//Returns a string that will be the header of the output HTML file
//QString html_head(QString title, QString fontFamily, int basesize);
QString html_head(QString title, QFont font);

//Generates and returns the Title of the book
QString html_book_title(QString , QString, QString);

//Generates and returns the Index ('Tochen') of the HTML file
QString html_link_table(QList <IndexItem>,int short_index_level, bool dot , bool hasRUS = false);

//Generates an index to all LINKS with the given level (NOTE: links to links, not to text)
inline QString index_to_index(QList <IndexItem> indexitemlist,int short_index_level);

//Returns html code of a "<a name" tag, for the given name
inline QString namepoint (QString name);

//Returns an html link ("<a href") by the given link_to and name.
inline QString link(QString linkto, QString displaytext, int id = 0);

//Return html code of dots:
QString bluedot();
QString reddot();


QString CSS(QString fontFamily, int fontSize);
QString Script();


//Returns the Html code for the given external link
QString ExternalLink(QString linkcode);

//I have no idea how this works, it just does.
// Many thanks to the developer of the originall program
string Decrypt (string text, bool decrypt);


#endif
