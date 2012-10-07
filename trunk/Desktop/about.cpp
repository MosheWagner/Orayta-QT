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

#include "about.h"
#include "ui_about.h"
#include "../OraytaBase/functions.h"
#include <QDesktopServices>
#include <QUrl>


About::About(QWidget *parent) : QDialog(parent), m_ui(new Ui::About)
{
    m_ui->setupUi(this);

    setWindowIcon(QIcon(":/Icons/Orayta.png"));

    QString text = tr("<center><b>Orayta - Hebrew Books");
    text += QString(" , ") + VERSION;

    //Add some info about this build
    text += " , QT-WEBKIT Engine";

#ifdef Q_WS_WIN
    text += " , Win32";
#else
    text += " , GNU\\Linux";
#endif

    text += "<br> </b></center>";

    text += "   <a href=\"http://orayta.googlecode.com\">http://orayta.googlecode.com</a> <br><br>";
    text += "   Don't skip our android app! ";

    text += tr("   Moshe Wagner - ");
    text += tr("   moshe.wagner@gmail.com, 5773");

    m_ui->label->setText(text);

}

About::~About()
{
    delete m_ui;
}

void About::changeEvent(QEvent *e)
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

void About::on_label_linkActivated(QString link)
{
    //Open the project's site in a browser
    QDesktopServices::openUrl(QUrl(link));
}
