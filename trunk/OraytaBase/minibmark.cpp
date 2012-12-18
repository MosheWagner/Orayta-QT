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
* Author: Izar <izar00@gmail.com>
*/

#include "minibmark.h"

//MiniBMark::MiniBMark()
MiniBMark::MiniBMark(Book *b, BookIter iter, BMarkList * parent)
{
    mBook = b;
    mBIter = iter;
    constant = false;
    parentList = parent;

    setText(mBook->getNormallDisplayName() + ", " + mBIter.humanDisplay());

    setIcon(QIcon(":Icons/bookmarks.png"));
//    bookUid = uid;
//    location = loc;

    //TODO: set the value if needed.
    id = -1;
}


Book* MiniBMark::getBook(){
    return mBook;
}

void MiniBMark::setTitle(QString title)
{
    if (title.simplified() != "") setText(title + " (" + mBook->getNormallDisplayName() + ", " + mBIter.humanDisplay() + ")");
}

QString MiniBMark::title()
{
    QString nt = mBook->getNormallDisplayName() + ", " + mBIter.humanDisplay();
    return text().replace(nt, "").replace("(","").replace(")","").simplified();
}

BookIter MiniBMark::getBookIter(){
    return mBIter;
}
void MiniBMark::setBook(Book *b){
    mBook= b;
}
void MiniBMark::setBookIter(BookIter iter){
    mBIter = iter;
}

int MiniBMark::getUid(){
    return mBook->getUniqueId();
}

BMarkList * MiniBMark::getParentList(){
    return parentList;
}

bool MiniBMark::isConstant(){
    return constant;
}

void MiniBMark::setConstant(bool b){
    constant = b;
}
