#ifndef MINIBMARK_H
#define MINIBMARK_H

//#include <QWidget>
#include <QListWidgetItem>
#include <QListWidget>
#include <QObject>
#include "book.h"
#include "bookiter.h"

class MiniBMark : public QObject, public QListWidgetItem
{
    Q_OBJECT
//class MiniBMark : public QListWidgetItem
//{
public:
//     MiniBMark();
    explicit MiniBMark(Book *b, BookIter iter);

signals:

public slots:
    Book* getBook();
    BookIter getBookIter();
    bool isConstant();
    void setBook(Book *b);
    void setBookIter(BookIter iter);
    void setConstant(bool);

    int getUid();

private:
    //id for this bookmark
    int id;
//    //uid of book
//    int bookUid;
//    //location in the book
//    QString location;
    Book* mBook;
    BookIter mBIter;
    //true if user wants this bookmark to remain in place;
    bool constant;


};

#endif // MINIBMARK_H
