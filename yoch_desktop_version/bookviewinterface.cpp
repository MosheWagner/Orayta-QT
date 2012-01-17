
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
        throw("Invalid booktype : unable to create");
    }
}

BookViewInterface::BookViewInterface( BookDisplayer* parent ) :
    mBookDisp(parent)
{
    if (!parent) throw "Unable to construct BookViewInterface without BookDisplayer parent object";
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

QWidget* BookViewInterface::additionalButtons()
{  return NULL;  }
