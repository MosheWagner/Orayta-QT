#ifndef TREEITEM_DIRECTORY_H
#define TREEITEM_DIRECTORY_H


#include "treeitem_base.h"

class NodeDirectory : public BaseNodeItem
{
public:
    NodeDirectory(BaseNodeItem* parent, QString name, QString path, bool isUserBook = false);

    Nodetype nodetype() const;

private:
    void setIcon();
};


#endif // TREEITEM_DIRECTORY_H
