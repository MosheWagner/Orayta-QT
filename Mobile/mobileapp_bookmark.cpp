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
* Author: Moshe Wagner. <moshe.wagner@gmail.com>
*/

#include "mobileapp.h"
#include "ui_mobileapp.h"
#include "../OraytaBase/minibmark.h"
#include <QDebug>
#include <QDir>
#include <QMenu>

/*
  This file includes all functions of the mobileapp class that deal with bookmarks.
*/

void MobileApp::setupBookmarkList(){

    ui->dailyLearningList->addDafYomi(bookList);
    ui->dailyLearningList->addMishnaYomit(bookList);
    ui->dailyLearningList->addHalachaYomit(bookList);
}


void MobileApp::BMShortClicked(QListWidgetItem *item)
{
    qDebug() << "tA";
    loadBookFromBM(item);
}

void MobileApp::BMLongClicked(QListWidgetItem *item)
{
    menu = new QMenu(this);

    action = new QAction(tr("Delete bookmark"), menu);
    action->setIcon(QIcon(":/Icons/edit-delete.png"));

    QFont f; f.setPixelSize(gFontSize / 2);
    action->setFont(f);

    menu->addAction(action);

    //I know this is an ugly way to do this, but the good way with signalMapper wouldn't work.
    // Ugly code just works.
    bm = dynamic_cast<MiniBMark *>(item);
    if (!bm) return;

    connect(action, SIGNAL(triggered()), this , SLOT(removeBM()));

    menu->setLayoutDirection(Qt::RightToLeft);

    //Open the menu under the cursor's position
    QPoint p = QPoint(QCursor::pos().x() - (menu->width() / 2), QCursor::pos().y());
    menu->exec(p);
}

void MobileApp::removeBM()
{
    if (!bm) return;

    BMarkList *bml = bm->getParentList();

    if (bml) bml->eraseBookMark(bm);

    //TODO- fix bug where app crashes if back pressed at this point. the following workaround doesnt work
//    qDebug()<<"oh please dont shut down :(";
//    setFocus(); grabKeyboard(); grabMouse();
}

void MobileApp::loadBookFromBM(QListWidgetItem *item)
{
    MiniBMark *bm= dynamic_cast<MiniBMark *>(item);
    if (!bm || bm == 0) return;
    BookIter it = bm->getBookIter();
    showBook(bm->getBook(), it);
    return;
}

void MobileApp::addStaticBMhere(){
    Book* b = displayer->getCurrentBook();
    BookIter it = displayer->getCurrentIter();
    MiniBMark* bm = ui->staticBookMarkList->addBookMark(b, it);

    //make the bookmark static so it doesn't get removed automatically
    if (!bm) return;
    bm->setConstant(true);
}

void MobileApp::on_bookMarksBTN_clicked()
{
    ui->stackedWidget->setCurrentIndex(HISTORY_PAGE);
}

void MobileApp::addBookMark(Book * b, BookIter iter){
    // dont create a bookmark for an index page
    // TODO: let the user decide?
    if (iter.isEmpty()) return;

    ui->historyBookmarkList->addBookMark(b, iter);
}

