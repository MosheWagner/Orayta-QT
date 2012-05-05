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

#ifndef BOOKMARKTITLE_H
#define BOOKMARKTITLE_H

#include <QObject>
#include <QString>
#include <QDialog>

namespace Ui {
    class BookMarkTitle;
}

class BookMarkTitle : public QDialog {
    Q_OBJECT
    Q_DISABLE_COPY(BookMarkTitle)
public:
    //Called for creating a new bookmark
    explicit BookMarkTitle(QWidget *parent, QString lnk, QString disp);
    //Called for editing an existing bookmark's title
    explicit BookMarkTitle(QWidget *parent, int id, QString oldtitle);
    virtual ~BookMarkTitle();

signals:
    void OK(QString, QString);
    void RenameOK(int, QString);

protected:
    virtual void changeEvent(QEvent *e);
    QString mLnk;
    int mId;

private:
    Ui::BookMarkTitle *m_ui;
    void toRTL();

private slots:
    void on_ok_clicked();
    void on_cancel_clicked();
};

#endif // BOOKMARKTITLE_H
