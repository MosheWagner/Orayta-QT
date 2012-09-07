#include "bmarklist.h"
#include <QSettings>
#include <QDebug>
#include <QString>

BMarkList::BMarkList(QWidget *parent) :
    QListWidget(parent)
{

}


//please call this method only after constructor.
void BMarkList::loadHistory(BookList booklist){

    QSettings settings("Orayta", "SingleUser");
    QString mName = this->objectName();
    //this will allocate a uniqe group to each list.
    settings.beginGroup(mName);

    //reload bookmark list
    settings.beginReadArray("BookMarkList");

    for (int id=1; id<limit; id++)
    {
        settings.setArrayIndex(id);
        int uid = settings.value("uid").toInt();
        QString iterStr = settings.value("iter").toString();
        bool constant = settings.value("constant").toBool();
        Book* b = booklist.findBookById(uid);
        if (!b) continue;
        BookIter iter = BookIter::fromEncodedString(iterStr);

        MiniBMark* bm = addBookMark(b, iter);
        bm->setConstant(constant);
    }
    settings.endArray();
    settings.endGroup();

}


BMarkList::~BMarkList(){
    QSettings settings("Orayta", "SingleUser");
    QString mName = this->objectName();
    settings.beginGroup(mName);

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
        settings.setValue("constant", b->isConstant());

        if (id > limit) break;

        id++;
    }
    settings.endArray();
    settings.endGroup();

}

MiniBMark* BMarkList::addBookMark(Book* book, BookIter iter)
{
    if(!book)  return NULL;

    //search in existing list and see if this bookmark already exists.
    int newUid = book->getUniqueId();
    const QList<QListWidgetItem*> list =  findItems("", Qt::MatchContains);
    foreach( QListWidgetItem* item, list)
    {
        MiniBMark* b = static_cast<MiniBMark*>(item) ;
        // delete the old bookmark if it for this book, only if it isn't constant.
        if (b && b->getUid() == newUid && b->isConstant() == false)
            delete b;
    }

    //add a new bookmark here
    MiniBMark* bm = new MiniBMark(book, iter);
//    addItem(bm);
    insertItem(0, bm);

    // return the bm so it can be modified later
    return bm;
}

//bool BMarkList::isDafYomiActive() {return dafYomiActive;}
//bool BMarkList::isParshaYomiActive(){return parshaYomiActive;}
//bool BMarkList::isMishnaYomiActive(){return mishnaYomiActive;}

//void BMarkList::setDafYomiActive(bool b) {dafYomiActive = b;}
//void BMarkList::setParshaYomiActive(bool b) {parshaYomiActive = b;}
//void BMarkList::setMishnaYomiActive(bool b) {mishnaYomiActive = b;}

//void BMarkList::addDafYomi(BookList booklist) {

//    //get string representation of current day yomi:
//    QString dyFull = dafYomi(QDate::currentDate ());

//    QChar separator = '%';
//    //get masechet and daf separatly:
//    QString dyMasechet = dyFull.split(separator).at(0);
//    QString dyDaf = dyFull.split(separator).at(1);

//    // find book object from string
//    Book* dyBook = booklist.FindBookByName(dyMasechet);
//    //if we cant find the book
//    if (!dyBook) return;

//    // convert the string to bookIter
//    BookIter iter;
//    //TODO: this is only a dummy method, it wont work!
//    //MOSHE please fix soon!@#$%^^&&*()!
//    iter.fromString(dyDaf);

//    newBookMark(dyBook, iter);
//}

void BMarkList::addDafYomi(BookList bl)
{
    QDate d = QDate::currentDate ();

    QList <int> masehtotUID;
    masehtotUID << 841 << 844 << 847 << 908 << 850 << 853 << 875 << 859 << 1106 << 888 << 862 << 892 << 1136 << 868 << 920 << 895 << 903 << 928 << 934 << 899 << 997 << 959 << 948 << 1003 << 1007 << 1014 << 1022 << 1027 << 1030 << 1040 << 1045 << 1132 << 1060 << 1035 << 1121 << 1117 << 1056;

    QList <int> dapim;
    dapim << 64 << 157 << 105 << 121 << 22 << 88 << 56 << 40 << 35 << 31 << 32 << 29 << 27 << /*Yevamot*/122 << 112 << 91 << 66 << 49 << 90 << 82 << /*Bava Kama*/119 << 119 << 176 << 113 << 24 << 49 << 76 << 14 << /*Zevahim*/120 << 110 << 142 << 61 << 34 << 34 << 28 << 37 << 73;

    int numDapim = 0;
    for (int i=0; i<dapim.size(); i++) numDapim += dapim[i] - 1;

    QDate firstDaf(1923, 7, 10); // כ"ו בתמוז ה'תרפ"ג
    int daf = (firstDaf.daysTo(d) + 1) % numDapim;

    if ( daf < 0 ) return;
    else if (daf==0) daf = numDapim;

    int masehet=0;

    while (dapim[masehet]-1 < daf)
    {
        daf-=dapim[masehet++]-1;
    }

    BookIter itr;
    Book * b = bl.findBookById(masehtotUID[masehet]);
    if (b != NULL)
    {
        QList <QString> t;
        if (!ReadFileFromZip(b->getPath(), "BookText", t, "UTF-8")) return;

        bool found = false;
        for (int i=0; i<t.size(); i++)
        {
            itr.SetLevelFromLine(t[i]);
            if (t[i].indexOf("~ דף " + NumberToGematria(daf+1, false)) != -1)
            {
                i = t.size(); //Break
                found = true;
            }
        }

        if (!found) itr = BookIter();
    }
    else return;

    addBookMark(b, itr);
}
