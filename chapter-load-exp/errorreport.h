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

#ifndef ERRORREPORT_H
#define ERRORREPORT_H

#include <QtGui/QDialog>
#include <QDesktopServices>

namespace Ui {
    class errorReport;
}

class errorReport : public QDialog {
    Q_OBJECT
public:
    errorReport(QWidget *parent, QString location, QString bookdisp);
    ~errorReport();

private:
    Ui::errorReport *m_ui;
    void toRTL();

private slots:
    void on_accept_clicked();
    void on_cancel_clicked();
};

#endif // ERRORREPORT_H
