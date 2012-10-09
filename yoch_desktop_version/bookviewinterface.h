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

#ifndef BOOKVIEWINTERFACE_H
#define BOOKVIEWINTERFACE_H


#include "treeitem_base_book.h"


class BookDisplayer;

class BookViewInterface
{
public:
    BookViewInterface(BookDisplayer*);

    // virtual pure
    virtual NodeBook::Booktype booktype() const = 0;

    virtual QWidget* widget() = 0;

    virtual void loadBook(const NodeBook*) = 0;
    // default implementation : nothing
    virtual void searchText(const QString& text, bool backwards);
    virtual void ZoomIn();
    virtual void ZoomOut();
    virtual void normalZoom();
    virtual void jumpToTop();
    virtual void reload();
    virtual void setInternalLocation(const QString& location);
    virtual void print();

    virtual QWidget* additionalButtons();


protected:
    BookDisplayer* mBookDisp;

};


BookViewInterface* BookViewFactory(BookDisplayer *, NodeBook::Booktype);

#endif // BOOKVIEWINTERFACE_H
