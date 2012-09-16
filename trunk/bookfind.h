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

#ifndef BOOKFIND_H
#define BOOKFIND_H

#include <QtGui/QDialog>
#include <booklist.h>
#include <QListWidgetItem>

namespace Ui {
    class bookfind;
}

class bookfind : public QDialog {
    Q_OBJECT
public:
    bookfind(QWidget *parent, BookList& booklist);
    ~bookfind();

protected:

private:
    Ui::bookfind *m_ui;
    BookList& mBookList;

    void toRTL();
    void reBuildList(QString);

signals:
    void openBook(int id);

private slots:
    void on_pushButton_clicked();
    void on_pushButton_2_clicked();
    void on_radioButton_2_toggled(bool checked);
    void on_lineEdit_textEdited(QString);
    void on_listWidget_itemDoubleClicked(QListWidgetItem *);
#ifdef MOBILE
    void on_listWidget_itemClicked(QListWidgetItem *);
#endif
};

#endif // BOOKFIND_H
