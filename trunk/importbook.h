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

#ifndef IMPORTBOOK_H
#define IMPORTBOOK_H

#include <QDialog>

namespace Ui {
    class importBook;
}

class importBook : public QDialog
{
    Q_OBJECT

public:
    explicit importBook(QWidget *parent = 0);
    ~importBook();

private:
    Ui::importBook *ui;

private slots:
    void on_addBooks_clicked();
    void on_addFolder_clicked();
    void on_importBTN_clicked();
    void on_cancelBTN_clicked();
    void on_deleteButton_clicked();
    void on_label_2_linkActivated(QString link);

signals:
    void updateTree();
};

#endif // IMPORTBOOK_H
