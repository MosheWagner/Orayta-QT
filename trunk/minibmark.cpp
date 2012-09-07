#include "minibmark.h"

//MiniBMark::MiniBMark()
MiniBMark::MiniBMark(Book *b, BookIter iter)
{
    mBook = b;
//    if (! iter)
//        mBIter = new BookIter();
//    else
        mBIter = iter;
    constant = false;


    setText(mBook->getNormallDisplayName() + ", " + mBIter.humanDisplay());

//    bookUid = uid;
//    location = loc;

    //TODO: set the value if needed.
    id = -1;
}

Book* MiniBMark::getBook(){
    return mBook;
}

BookIter MiniBMark::getBookIter(){
    return mBIter;
}
void MiniBMark::setBook(Book *b){
    mBook= b;
}
void MiniBMark::setBookIter(BookIter iter){
    mBIter = iter;
}

int MiniBMark::getUid(){
    return mBook->getUniqueId();
}

bool MiniBMark::isConstant(){
    return constant;
}

void MiniBMark::setConstant(bool b){
    constant = b;
}

//#ifdef MOBILE
//#include "mobileapp.h"
//void MobileApp::addBookMark(Book b, BookIter iter){

//    MobileApp::ui

//    //TODO: fill... @@@
////    MiniBMark* bm = new MiniBMark(b, iter, ui->bookMarkList);
////    ui->bookMarkList->addItem(mb);

//}



//#endif //#ifdef MOBILE
