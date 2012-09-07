/**
  written by abe.izar <izar00@gmail.com>
  this class holds a list of the most recent locations the user has opened.
  these locations are set as 'mini book marks' that actually hold a book and bookIter.
  */

#ifndef BMARKLIST_H
#define BMARKLIST_H

#include <QObject>
#include <QListWidget>
#include <QWidget>
#include "minibmark.h"
#include "booklist.h"
#include <QSettings>
#include "functions.h"
#include <QDate>

class BMarkList : public QListWidget
{

public:
    explicit BMarkList(QWidget *parent = 0);
    ~BMarkList();
    MiniBMark* addBookMark(Book* book, BookIter iter);


signals:

public slots:
    void loadHistory(BookList booklist);

    // add daf yomi to this list
    void addDafYomi(BookList booklist);

private:
    // max number of bookmarks to remember
    // NOTE: currently this is a const, but later we may want to let the user set this manually
   static const int limit = 30;

   // does the user want to show daf yomi in this list
//   bool dafYomiActive;
//   bool parshaYomiActive;
//   bool mishnaYomiActive;
//   //etc...

//   bool isDafYomiActive();
//   bool isParshaYomiActive();
//   bool isMishnaYomiActive();

//   void setDafYomiActive(bool);
//   void setParshaYomiActive(bool);
//   void setMishnaYomiActive(bool);




};

#endif // BMARKLIST_H
