#include "treeitem_directory.h"
#include "functions.h"


NodeDirectory::NodeDirectory(BaseNodeItem* parent, QString name, QString path, bool isUserBook) :
    BaseNodeItem(parent, name, path, isUserBook)
{
    QString filename = path + ".folder";

    QStringList lines;

    //Read the book's conf file file:
    if(!ReadFileToList(filename, lines, "UTF-8"))
        return;

    for (QStringList::const_iterator it = lines.begin(); it != lines.end(); ++it)
    {
        if ( it->indexOf("BranchName") != -1)
        {
            mTreeDisplayName = it->mid(11);
        }
    }
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
