#ifndef BOOKFINDMOBILE_H
#define BOOKFINDMOBILE_H

#include <QDialog>
#include "bookfind.h"

namespace Ui {
    class BookFindMobile;
}

class BookFindMobile : public bookfind
{
    Q_OBJECT

public:
    BookFindMobile(QWidget *parent, BookList& booklist);
    ~BookFindMobile();

private:
    Ui::BookFindMobile *m_ui;
};

#endif // BOOKFINDMOBILE_H
