#include "bmarklist.h"
#include <QSettings>

BMarkList::BMarkList(QWidget *parent) :
    QListWidget(parent)
{}

void BMarkList::loadHistory(BookList booklist){
    QSettings settings("Orayta", "SingleUser");

    //reload bookmark list
    settings.beginReadArray("BookMarkList");

    for (int id=1; id<limit; id++)
    {
        settings.setArrayIndex(id);
        int uid = settings.value("uid").toInt();
        QString iterStr = settings.value("iter").toString();
        Book* b = booklist.findBookById(uid);
        if (!b) continue;
        BookIter iter = BookIter::fromEncodedString(iterStr);

        newBookMark(b, iter);
    }
    settings.endArray();

//blaTODO: read history to list.
}


BMarkList::~BMarkList(){
    QSettings settings("Orayta", "SingleUser");

    //reload bookmark list
    settings.beginWriteArray("BookMarkList");
    const QList<QListWidgetItem*> list =  findItems("", Qt::MatchContains);
    int id = 1;
    foreach( QListWidgetItem* item, list)
    {
        MiniBMark* b = static_cast<MiniBMark*>(item) ;

        if (!b) continue;

        settings.setArrayIndex(id);
        settings.setValue("uid", b->getUid());
        settings.setValue("iter", b->getBookIter().toEncodedString());

        if (id > limit) break;

        id++;
    }
    settings.endArray();

}

void BMarkList::newBookMark(Book* book, BookIter iter)
{
    if(!book)  return;

    //search in existing list and see if this bookmark already exists.
    int newUid = book->getUniqueId();
    const QList<QListWidgetItem*> list =  findItems("", Qt::MatchContains);
    foreach( QListWidgetItem* item, list)
    {
        MiniBMark* b = static_cast<MiniBMark*>(item) ;
        // delete the old bookmark
        if (b && b->getUid() == newUid)
            delete b;
    }

    //add a new bookmark here
    MiniBMark* bm = new MiniBMark(book, iter);
//    addItem(bm);
    insertItem(0, bm);
}
