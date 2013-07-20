/* This program is free software; you can redistribute it and/or modify
* it under the terms of the GNU General Public License version 2
* as published by the Free Software Foundation.
*
* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
* GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License
* along with this program; if not, write to the Free Software
* Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307 USA
*
* Author: Izar <izar00@gmail.com>
*/

#include "bmarklist.h"
#include "minibmark.h"
#include <QSettings>
#include <QDebug>
#include <QString>
#include <QMouseEvent>

BMarkList::BMarkList(QWidget *parent) : QListWidget(parent)
{
    setMouseTracking(true);
}

//For some reason mousePress is called only at release, so we act as if the last move was a press event
void BMarkList::mouseMoveEvent(QMouseEvent* event)
{
    //Save the time the click started
    clickT = QTime::currentTime();
}

void BMarkList::mouseReleaseEvent(QMouseEvent *e)
{
    // Custom logic
    QListWidgetItem *item = itemAt(e->pos());

    int delta = clickT.secsTo(QTime::currentTime());

    if (delta >= 1) emit longPress(item);
    else emit shortPress(item);
}

//Warning! Call this method only after constructor.
void BMarkList::loadHistory(BookList booklist)
{
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

        QString title = settings.value("title", "").toString();

        MiniBMark* bm = addBookMark(b, iter);
        bm->setConstant(constant);
        bm->setTitle(title);
    }
    settings.endArray();
    settings.endGroup();
}


BMarkList::~BMarkList()
{
    saveSettings();
}

void BMarkList::saveSettings()
{
    qDebug()<< "saving bookmarks...";
    QSettings settings("Orayta", "SingleUser");
    QString mName = this->objectName();
    settings.beginGroup(mName);

    //save book mark list to memory
    settings.beginWriteArray("BookMarkList");
    settings.remove("");
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
        settings.setValue("title", b->title());

        if (id > limit) break;

        id++;
    }
    settings.endArray();
    settings.endGroup();
    qDebug()<<"done.";
}

#include <QPushButton>
#include <QHBoxLayout>

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
    MiniBMark* bm = new MiniBMark(book, iter, this);

    insertItem(0, bm);

    // return the bm so it can be modified later
    return bm;
}

void BMarkList::eraseBookMark(MiniBMark *bm)
{
    takeItem(row(bm));
    delete bm;
}

//bool BMarkList::isDafYomiActive() {return dafYomiActive;}
//bool BMarkList::isParshaYomiActive() {return parshaYomiActive;}
//bool BMarkList::isMishnaYomiActive() {return mishnaYomiActive;}

//void BMarkList::setDafYomiActive(bool b) {dafYomiActive = b;}
//void BMarkList::setParshaYomiActive(bool b) {parshaYomiActive = b;}
//void BMarkList::setMishnaYomiActive(bool b) {mishnaYomiActive = b;}

void BMarkList::addMishnaYomit(BookList bl)
{
    QList <QString> lst;
    ReadFileToList(":/OraytaBase/DailyLimud.csv", lst, "UTF-8");

    //I know this isn't efficent, but for such small numbers, it really dosn't matter much.
    QString d;
    QStringList line;
    foreach(const QString s, lst)
    {
        d = s.split(",")[0];

        if (QDate::fromString(d,  "dd/MM/yyyy") == QDate::currentDate())
        {
            line = s.split(",");
            break;
        }
    }

    if (line.size() >= 4)
    {
        QString masechet, perek, mishna;
        masechet = line[1].split("-")[0];
        perek = line[2].split("-")[0];
        mishna = line[3].split("-")[0];

        QString n = "משנה - " + masechet;
        Book * bk = NULL;

        //Find book
        foreach (Book *b, bl)
        {
            if (b->getNormallDisplayName() == n)
            {
                bk = b;
                break;
            }
        }

        if (!bk) return;

        //Set iter
        BookIter it;
        it.SetLevelFromLine("$ משנה " + masechet);
        it.SetLevelFromLine("^ פרק " + perek);
        it.SetLevelFromLine("~ פרק " + perek + " - משנה " + mishna);

        MiniBMark *bm = addBookMark(bk, it);
        bm->setTitle("משנה יומית");
    }
}

void BMarkList::addHalachaYomit(BookList bl)
{
    QList <QString> lst;
    ReadFileToList(":/OraytaBase/DailyLimud.csv", lst, "UTF-8");

    //I know this isn't efficent, but for such small numbers, it really dosn't matter much.
    QString d;
    QStringList line;
    foreach(const QString s, lst)
    {
        d = s.split(",")[0];

        if (QDate::fromString(d,  "dd/MM/yyyy") == QDate::currentDate())
        {
            line = s.split(",");
            break;
        }
    }

    if (line.size() >= 6)
    {
        QString siman, seif;
        siman = line[4].split("-")[0];
        int sim = GematriaValueOfString(siman);

        seif = line[5].split("-")[0];

        //Find which part of Shulchan aruch we are in
        QList <int> parts;
        parts << 127 << 241 << 344 << 428 << 529 << 697;
        int id;
        for (id=0; parts[id] < sim; id++);
        id += 80000; //Id of Shulchan aruch part 1

        Book * bk = NULL;
        bk = bl.findBookById(id);

        if (!bk) return;

        //Set iter
        BookIter it;
        it.SetLevelFromLine("~ סימן " + siman);
        it.SetLevelFromLine("! {" + seif + "}");

        MiniBMark *bm = addBookMark(bk, it);
        bm->setTitle("הלכה יומית");
    }
}

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
    Book * b = NULL;
    b = bl.findBookById(masehtotUID[masehet]);
    if (b)
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

    MiniBMark *bm = addBookMark(b, itr);
    bm->setTitle("דף יומי");
}
