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

#include "bookmarktitle.h"

//Called for creating a new bookmark
BookMarkTitle::BookMarkTitle(QWidget *parent, QString lnk, QString disp) : QDialog(parent), m_ui(new Ui::BookMarkTitle)
{
    m_ui->setupUi(this);

    if (LANG == "Hebrew") toRTL();

    m_ui->lineEdit->setText(disp);

    mLnk = lnk;
}

//Called for editing an existing bookmark's title
BookMarkTitle::BookMarkTitle(QWidget *parent, int id, QString oldtitle) : QDialog(parent), m_ui(new Ui::BookMarkTitle)
{
    m_ui->setupUi(this);

    m_ui->lineEdit->setText(oldtitle);

    mLnk = "";
    mId = id;
}

void BookMarkTitle::toRTL()
{
    this->setLayoutDirection(Qt::RightToLeft);
    m_ui->label->setLayoutDirection(Qt::RightToLeft);
}

BookMarkTitle::~BookMarkTitle()
{
    delete m_ui;
}

void BookMarkTitle::changeEvent(QEvent *e)
{
    QDialog::changeEvent(e);
    switch (e->type()) {
    case QEvent::LanguageChange:
        m_ui->retranslateUi(this);
        break;
    default:
        break;
    }
}

void BookMarkTitle::on_cancel_clicked()
{
    //if ( mLnk != "") emit OK(mLnk, "");
    close();
}

void BookMarkTitle::on_ok_clicked()
{
    if ( mLnk != "") emit OK(mLnk, m_ui->lineEdit->text());
    else emit RenameOK(mId, m_ui->lineEdit->text());
    close();
}
