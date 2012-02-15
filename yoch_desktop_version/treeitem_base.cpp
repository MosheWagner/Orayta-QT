#include "treeitem_base.h"
#include "treeitem_directory.h"
#include "booktree.h"

BaseNodeItem::BaseNodeItem ( BaseNodeItem* parent, QString name, QString path, bool isUserBook ) :
    mpParent(parent),
    mPath(path),
    mTreeDisplayName(name),   // ######### completement faux...
    mIconState(GREY),
    mInSearch(false),
    mIsHidden(false),
    mUserBook(isUserBook)
{}

// tres casse pieds, il faut supprimer l'item présent du vector au niveau supérieur...
BaseNodeItem::~BaseNodeItem()
{
    //qDebug() << "delete " << mTreeDisplayName;
    qDeleteAll(mvChildren.begin(), mvChildren.end());
    // ######## hum, plutot couteux...
    if (mpParent) mpParent->mvChildren.removeOne(this);
}

// recursive (call it after tree is complete)
bool BaseNodeItem::isSearchable() const
{
    for (QList<BaseNodeItem*>::const_iterator it = mvChildren.begin(); it != mvChildren.end(); ++it)
        if ( (*it)->isSearchable() ) return true;

    return false;
}

QString BaseNodeItem::getPath() const
{   return mPath;  }

bool BaseNodeItem::IsHidden() const
{  return mIsHidden; }

bool BaseNodeItem::IsInSearch() const
{  return mInSearch; }

bool BaseNodeItem::IsUserBook() const
{ return mUserBook; }

bool BaseNodeItem::IsUserCheckable() const
{  return flags() & Qt::ItemIsUserCheckable;  }

void BaseNodeItem::changeFont( const QFont& font )
{
    // recursively apply to all childrens
    for (QList<BaseNodeItem*>::const_iterator it = mvChildren.begin(); it != mvChildren.end(); ++it)
        (*it)->changeFont(font);
}

void BaseNodeItem::setName(QString name)
{   mTreeDisplayName = name;  }

void BaseNodeItem::add_child(BaseNodeItem* child)
{
    if (child != NULL)  // sanity check
        mvChildren.push_back(child);
}

BaseNodeItem* BaseNodeItem::getParent()
{   return mpParent;  }

QList<BaseNodeItem*>& BaseNodeItem::getChildren()
{   return mvChildren;  }

QString BaseNodeItem:: getTreeDisplayName() const
{   return mTreeDisplayName;   }

BaseNodeItem::IconState BaseNodeItem::getIconState() const
{   return mIconState;  }

void BaseNodeItem::setCheckState(IconState newstate)
{
    mIconState = newstate;

    if ( isSearchable() )
    {
        //Show state in the checkbox too
        if (mIconState == BLUE)
            QTreeWidgetItem::setCheckState(0, Qt::Checked);
        else if (mIconState == HALF)
            QTreeWidgetItem::setCheckState(0, Qt::PartiallyChecked);
        else if (mIconState == GREY)
            QTreeWidgetItem::setCheckState(0, Qt::Unchecked);
    }

    // virtual : set the correct icon for this item (default does nothing)
    setIcon();
}

// default : do nothing
void BaseNodeItem::setIcon()
{
}

// #################
// La gestion de la selection est foireuse pour l'instant...
void BaseNodeItem::repaintIcon()
{
#define UNSELECTED          0x1
#define SELECTED            0x2
#define PARTIALLY_SELECTED  (UNSELECTED | SELECTED)

    // en pricipe, cette fonction est appelée depuis le bas, donc pas de risques
    // qu'on soit dans une feuille, mais attention...
    int flag = 0;

    //Check children
    for (int i=0; i < mvChildren.size(); i++)
    {
        if ( mvChildren[i]->isSearchable() )
        {
            if(mvChildren[i]->mIconState == BLUE)
                flag |= SELECTED;
            else if(mvChildren[i]->mIconState == GREY)
                flag |= UNSELECTED;
            else if(mvChildren[i]->mIconState == HALF)
                flag |= PARTIALLY_SELECTED;
        }
    }

    switch (flag)
    {
    case UNSELECTED:
        setCheckState(GREY);
        break;
    case PARTIALLY_SELECTED:
        setCheckState(HALF);
        break;
    case SELECTED:
        setCheckState(BLUE);
        break;
    }

    if(mpParent != NULL)
        mpParent->repaintIcon();
}

// recursive function, for not unecessarly use repaintIcon() on children
void BaseNodeItem::recSetSelection(bool selected)
{
    if ( !IsUserCheckable() )
        return;

    mInSearch = selected;

    //Darken icon
    setCheckState(selected ? BLUE : GREY);

    //Unselect all children
    for (int i=0; i < mvChildren.size(); i++)
        mvChildren[i]->recSetSelection(selected);
}

void BaseNodeItem::setSelected(bool selected)
{
    // unselect all children
    recSetSelection(selected);

    //Repaint parent (it will tell it's parent to repaint too, and so on...)
    if(mpParent != NULL)
        mpParent->repaintIcon();
}


NodeBook* BaseNodeItem::getBookPtrFromId(int uid) const
{
    BookTree* tree = dynamic_cast<BookTree*>(treeWidget());
    return ( tree ? tree->findBookById(uid) : NULL );
}

