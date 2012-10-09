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

#include "bookdisplayer.h"

#include "bookview_html.h"
#include "bookview_orayta.h"
#ifdef POPPLER
    #include "bookview_pdf.h"
#endif

BookViewInterface * BookViewFactory(BookDisplayer * parent, NodeBook::Booktype type )
{
    switch(type)
    {
    case NodeBook::Orayta:
        return new OraytaBookView(parent);
    case NodeBook::Html:
        return new HtmlBookView(parent);
#ifdef POPPLER
    case NodeBook::Pdf:
        return new PdfBookView(parent);
#endif
    default:
        qFatal("Invalid booktype : unable to create");
    }
}

BookViewInterface::BookViewInterface( BookDisplayer* parent ) :
    mBookDisp(parent)
{
    if (!parent)
        qFatal("Unable to construct BookViewInterface without BookDisplayer parent object");
}

//// default interfaces : do nothing

void BookViewInterface::ZoomIn()
{}

void BookViewInterface::ZoomOut()
{}

void BookViewInterface::normalZoom()
{}

void BookViewInterface::jumpToTop()
{}

void BookViewInterface::searchText(const QString& text, bool backwards)
{}

void BookViewInterface::reload()
{}

void BookViewInterface::setInternalLocation(const QString& location)
{}

void BookViewInterface::print()
{}

QWidget* BookViewInterface::additionalButtons()
{  return NULL;  }
