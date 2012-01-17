#include "treeitem_pdf.h"

/////////////////////////////////////////////////////
//           Pdf Book
/////////////////////////////////////////////////////

PdfBookItem::PdfBookItem ( BaseNodeItem* parent, QString path, QString name, bool isUserBook ) :
    NodeBook(parent, path, name, isUserBook)
{
    QTreeWidgetItem::setIcon(0, QIcon(":/Icons/book-pdf.png"));
}

NodeBook::Booktype PdfBookItem::booktype() const
{  return Pdf;  }

bool PdfBookItem::isSearchable() const
{  return false;  }
