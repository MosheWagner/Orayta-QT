#ifndef TREEITEM_DIRECTORY_H
#define TREEITEM_DIRECTORY_H


#include "treeitem_base.h"

class NodeDirectory : public BaseNodeItem
{
public:
    NodeDirectory(BaseNodeItem *parent, QString name, QString path, bool isUserBook = false);

    virtual Nodetype nodetype() const;
    virtual bool isFontModifiable() const;
    virtual bool isSearchable() const;
    virtual QList<QAction*> menuActions() const;

private:
    void setIcon();
};


#endif // TREEITEM_DIRECTORY_H
