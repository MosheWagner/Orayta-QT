#ifndef TREEITEM_PDF_H
#define TREEITEM_PDF_H


#include "treeitem_base_book.h"


class PdfBookItem : public NodeBook
{
public:
    PdfBookItem ( BaseNodeItem *parent, QString path, QString name, bool isUserBook );

    Booktype booktype() const;
    virtual bool isSearchable() const;

private:

};

#endif // TREEITEM_PDF_H
