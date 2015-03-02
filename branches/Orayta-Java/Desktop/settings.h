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


#ifndef SETTINGS_H
#define SETTINGS_H

#ifndef MOBILE
#include "desktopapp.h"
#endif

#include <QDialog>

namespace Ui {
    class Settings;
}

class Settings : public QDialog
{
    Q_OBJECT

public:
    explicit Settings(QWidget *parent = 0);
    ~Settings();

private:
    Ui::Settings *ui;

    //Holds the available languages and the way they should be displayed (such as "עברית" for "Hebrew")
    QStringList langs, langsDisplay;

signals:
    void ChangeLang(QString);

private slots:
    void on_systemLangCbox_clicked(bool checked);
    void on_saveConf_clicked();
    void on_fonSizeSpinBox_valueChanged(int );
    void on_fontComboBox_currentIndexChanged(QString );
    void on_cancelBTN_clicked();
};

#endif // SETTINGS_H
