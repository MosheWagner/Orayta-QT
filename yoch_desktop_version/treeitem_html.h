#ifndef TREEITEM_HTML_H
#define TREEITEM_HTML_H

#include "treeitem_base_book.h"

class HtmlBookItem : public NodeBook
{
public:
    HtmlBookItem ( BaseNodeItem* parent, QString path, QString name, bool isUserBook );

    Booktype booktype() const;
    virtual bool isSearchable() const;

private:

};

#endif // TREEITEM_HTML_H
