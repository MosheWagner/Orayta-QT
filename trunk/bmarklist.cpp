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

    //TODO: let the user decide these options.
    dafYomiActive = true;
    parshaYomiActive = false;
    mishnaYomiActive = false;

    if (dafYomiActive){
        addDafYomi(booklist);
    }
}


BMarkList::~BMarkList(){
    QSettings settings("Orayta", "SingleUser");

    //save book mark list to memory
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

bool BMarkList::isDafYomiActive() {return dafYomiActive;}
bool BMarkList::isParshaYomiActive(){return parshaYomiActive;}
bool BMarkList::isMishnaYomiActive(){return mishnaYomiActive;}

void BMarkList::setDafYomiActive(bool b) {dafYomiActive = b;}
void BMarkList::setParshaYomiActive(bool b) {parshaYomiActive = b;}
void BMarkList::setMishnaYomiActive(bool b) {mishnaYomiActive = b;}

void BMarkList::addDafYomi(BookList booklist) {

    //get string representation of current day yomi:
    QString dyFull = dafYomi(QDate::currentDate ());

    QChar separator = '%';
    //get masechet and daf separatly:
    QString dyMasechet = dyFull.split(separator).at(0);
    QString dyDaf = dyFull.split(separator).at(1);

    // find book object from string
    Book* dyBook = booklist.FindBookByName(dyMasechet);
    //if we cant find the book
    if (!dyBook) return;

    // convert the string to bookIter
    BookIter iter;
    //TODO: this is only a dummy method, it wont work!
    //MOSHE please fix soon!@#$%^^&&*()!
    iter.fromString(dyDaf);

    newBookMark(dyBook, iter);
}
