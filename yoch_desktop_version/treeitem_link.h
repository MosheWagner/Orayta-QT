#ifndef TREEITEM_LINK_H
#define TREEITEM_LINK_H


#include "treeitem_base_book.h"

class LinkItem : public NodeBook
{
public:
    LinkItem ( BaseNodeItem *parent, QString path, QString name, bool isUserBook );

    Booktype booktype() const;
    virtual bool isSearchable() const;
    virtual bool isFontModifiable() const;

    virtual bool IsInSearch() const;
    virtual bool IsHidden() const;
    virtual bool IsUserCheckable() const;
    virtual bool IsUserBook() const;

    virtual QString getPath() const;

    //virtual void setCheckState(IconState state);
    virtual QList<QAction*> menuActions() const;

    virtual void changeFont( const QFont& );

    virtual QString getLoadableFile() const;

    NodeBook* bookPtr() const;

private:

};

#endif // TREEITEM_LINK_H
