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

#ifndef SEARCHFORM_H
#define SEARCHFORM_H

#include <QtGui/QDialog>
#include "ui_searchform.h"
#include "functions.h"

namespace Ui {
    class SearchForm;
}

class SearchForm : public QDialog {
    Q_OBJECT
public:
    SearchForm(QWidget *parent);
    ~SearchForm();

signals:
    void Search (QRegExp, QString);
    void Search (QString);

private:
    Ui::SearchForm *m_ui;
    void toRTL();

private slots:

    void on_regexpCheckBox_clicked(bool checked);
    void on_cancel_clicked();
    void on_submit_clicked();
};

#endif // SEARCHFORM_H
