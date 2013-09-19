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

#include "errorreport.h"
#include "../OraytaBase/functions.h"
#include "desktopapp.h"

#include "ui_errorreport.h"
#include <QUrl>

errorReport::errorReport(QWidget *parent, QString location, QString bookdisp) : QDialog(parent), m_ui(new Ui::errorReport)
{
    m_ui->setupUi(this);

    if (LANG == "Hebrew") toRTL();

    QString disp = unescapeFromBase32(location).replace("_", " ").replace("-", " ").replace("{", "").replace("}", "").replace(" 0", "");

    disp = bookdisp + " " + disp;
    //Remove extra spaces that appear for some reason:
    disp.replace(QRegExp("[ ][ ]*"), " ");

    m_ui->lineEdit->setText( disp );

    m_ui->textEdit->setHtml("<div dir=\"RTL\">");
}

errorReport::~errorReport()
{
    delete m_ui;
}

void errorReport::toRTL()
{
    this->setLayoutDirection(Qt::RightToLeft);
}

void errorReport::on_cancel_clicked()
{
    close();
}

void errorReport::on_accept_clicked()
{
    QDesktopServices::openUrl(QUrl("mailto:moshe.wagner@gmail.com?subject=OraytaErrorReport&body=מיקום:\n" + m_ui->lineEdit->text() + "\n\nתיאור:\n" + m_ui->textEdit->toPlainText()));
    close();
}
