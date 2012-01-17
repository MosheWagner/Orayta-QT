#include "treeitem_html.h"

/////////////////////////////////////////////////////
//           Html Book
/////////////////////////////////////////////////////

HtmlBookItem::HtmlBookItem ( BaseNodeItem* parent, QString path, QString name, bool isUserBook ) :
    NodeBook(parent, path, name, isUserBook)
{
    QTreeWidgetItem::setIcon(0, QIcon(":/Icons/book-html.png"));
}

NodeBook::Booktype HtmlBookItem::booktype() const
{  return Html;  }

// ######### make it searchable ?
bool HtmlBookItem::isSearchable() const
{  return false;  }

