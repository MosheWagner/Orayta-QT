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
