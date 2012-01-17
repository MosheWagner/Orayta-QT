#include "treeitem_directory.h"



NodeDirectory::NodeDirectory(BaseNodeItem* parent, QString name, QString path, bool isUserBook) :
    BaseNodeItem(parent, name, path, isUserBook)
{
//    qDebug() << "NodeDirectory()" << name;
}

BaseNodeItem::Nodetype NodeDirectory::nodetype() const
{  return Node;  }

void NodeDirectory::setIcon()
{
    if (mUserBook)
        QTreeWidgetItem::setIcon(0, QIcon(":/Icons/folder-user.png"));
    else if (mIconState == BLUE)
        QTreeWidgetItem::setIcon(0, QIcon(":/Icons/folder-blue.png"));
    else if (mIconState == GREY)
        QTreeWidgetItem::setIcon(0, QIcon(":/Icons/folder-grey.png"));
    else if (mIconState == HALF)
        QTreeWidgetItem::setIcon(0, QIcon(":/Icons/folder-blue-grey.png"));
}
