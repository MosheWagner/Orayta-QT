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

#ifndef ADDCOMMENT_H
#define ADDCOMMENT_H

#include <QtGui/QDialog>
#include <QString>

namespace Ui {
    class AddComment;
}

class AddComment : public QDialog {
    Q_OBJECT
    Q_DISABLE_COPY(AddComment)
public:
    explicit AddComment(QWidget *parent = 0, QString lnk="", QString previosComment="");
    virtual ~AddComment();

protected:

private:
    Ui::AddComment *m_ui;
    QString mLnk;
    void toRTL();

signals:
    void commentWritten(QString, QString);

private slots:


private slots:
    void on_erase_clicked();
    void on_cancel_clicked();
    void on_ok_clicked();
};

#endif // ADDCOMMENT_H
