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

#include "addcomment.h"
#include "ui_addcomment.h"
#include "../OraytaBase/functions.h"

AddComment::AddComment(QWidget *parent, QString lnk, QString previosComment) : QDialog(parent), m_ui(new Ui::AddComment)
{
    m_ui->setupUi(this);


    if (LANG == "Hebrew") toRTL();

    mLnk = lnk;

    //Start with the previous comment (if exists)
    //(And make the cursor start on the right)
    m_ui->textEdit->setHtml("<div dir=\"RTL\">" + previosComment);

    if ( previosComment == "") m_ui->erase->hide();
}


void AddComment::toRTL()
{
    this->setLayoutDirection(Qt::RightToLeft);
}

AddComment::~AddComment()
{
    delete m_ui;
}


void AddComment::on_ok_clicked()
{
    close();
    //Update the DesktopApp with the new comment
    emit commentWritten(mLnk, m_ui->textEdit->toPlainText());
}

void AddComment::on_cancel_clicked()
{
    close();
}

void AddComment::on_erase_clicked()
{
    //Emit an empty comment
    emit commentWritten(mLnk, "");
    close();
}
