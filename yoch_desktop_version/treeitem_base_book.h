#ifndef TREEITEM_BASE_BOOK_H
#define TREEITEM_BASE_BOOK_H


#include <QPointer>

#include "treeitem_base.h"
//#include "bookdisplayer.h"

//class NodeBook;


class NodeBook : public BaseNodeItem
{
public:
    enum Booktype {
        Orayta,
        Html,
        Pdf,
        Unkown
    };

    NodeBook(BaseNodeItem* parent, QString path, QString name, bool isUserBook = false);

    //getters
    Nodetype nodetype() const;
    BookDisplayer* tabWidget() const;
    int getUniqueId() const;
    int getRealUniqueId() const;

    virtual Booktype booktype() const = 0;
    virtual QString getLoadableFile() const;
    virtual bool isSearchable() const;

    // setters
    void setTabWidget(BookDisplayer*);
    void setUniqueId(int id);
    void setRandomUniqueId(int id);

    static NodeBook* BookFactory(BaseNodeItem* parent, QString path, QString name, NodeBook::Booktype ft, bool isUserBook);


protected:

    //QWidget* of current viewTab (0 if not exists)
    QPointer<BookDisplayer> mTabWidget;

    //Unique id used for linking to a specific book
    int mUniqueId;

    int mRandomId;
};


typedef QList<NodeBook*> BookList;


#endif // TREEITEM_BASE_BOOK_H
