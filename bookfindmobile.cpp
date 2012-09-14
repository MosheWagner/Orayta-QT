#include "bookfindmobile.h"
#include "ui_bookfindmobile.h"
#include "bookfind.h"

BookFindMobile::BookFindMobile(QWidget *parent, BookList &booklist) :
    bookfind(parent, booklist),
    m_ui(new Ui::BookFindMobile)
{
    m_ui->setupUi(this);
}

BookFindMobile::~BookFindMobile()
{
    delete m_ui;
}
