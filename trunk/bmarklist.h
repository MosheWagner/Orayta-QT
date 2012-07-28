#ifndef BMARKLIST_H
#define BMARKLIST_H

#include <QObject>
#include <QListWidget>
#include <QWidget>
#include "minibmark.h"
#include "booklist.h"
#include <QSettings>

class BMarkList : public QListWidget
{

public:
    explicit BMarkList(QWidget *parent = 0);
    ~BMarkList();
    void newBookMark(Book* book, BookIter iter);


signals:

public slots:
    void loadHistory(BookList booklist);

private:
    // max number of bookmarks to remember
    // NOTE: currently this is a const, but later we may want to let the user set this manually
   static const int limit = 30;

};

#endif // BMARKLIST_H
