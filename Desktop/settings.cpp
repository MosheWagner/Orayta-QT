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

#include "settings.h"
#include "ui_settings.h"

Settings::Settings(QWidget *parent) : QDialog(parent), ui(new Ui::Settings)
{
    ui->setupUi(this);

    if (LANG == "Hebrew")
    {
        this->setLayoutDirection(Qt::RightToLeft);
        //ui->fontComboBox->setLayoutDirection(Qt::RightToLeft);
    }

    //Set available languages
    langs << "Hebrew" << "English" << "French";
    langsDisplay << "עברית" << "English" << "Français";

    //Show available languages in the language combobox
    for (int i=0; i<langs.size(); i++)
    {
        ui->langComboBox->addItem(langsDisplay[i]);
    }

    //Check if "use system lang" is set
    QSettings settings("Orayta", "SingleUser");
    settings.beginGroup("Confs");
    bool useSystemLang = settings.value("systemLang",true).toBool();
    settings.endGroup();

    ui->systemLangCbox->setChecked(useSystemLang);
    ui->groupBox->setEnabled(!useSystemLang);

    //Show current language
    int is = -1;
    for (int i=0; i<langs.size(); i++) if (LANG == langs[i]) is = i;
    ui->langComboBox->setCurrentIndex(is);

    //Show current font values
    QFont f (gFontFamily, gFontSize);
    ui->fontPreview->setFont(f);
    ui->fontComboBox->setCurrentFont(f);
    ui->fonSizeSpinBox->setValue(gFontSize);
}

Settings::~Settings()
{
    delete ui;
}

//Cancel clicked
void Settings::on_cancelBTN_clicked()
{
    close();
}

void Settings::on_fontComboBox_currentIndexChanged(QString font)
{
    //Settings have changed, so the save button should be enabled
    ui->saveConf->setEnabled(true);

    //Show the new font in the preview box
    ui->fontPreview->setFont(QFont(font, ui->fonSizeSpinBox->value()));
}

void Settings::on_fonSizeSpinBox_valueChanged(int size)
{
    //Settings have changed, so the save button should be enabled
    ui->saveConf->setEnabled(true);

    //Show the new font in the preview box
    ui->fontPreview->setFont(QFont(ui->fontComboBox->currentFont().family(), size));
}

//Save confs
void Settings::on_saveConf_clicked()
{
    //Save font
    gFontFamily = ui->fontComboBox->currentFont().family();
    gFontSize = ui->fonSizeSpinBox->value();

    ui->saveConf->setEnabled(false);

    //Change language if needed
    QSettings settings("Orayta", "SingleUser");
    settings.beginGroup("Confs");

    settings.setValue("systemLang",ui->systemLangCbox->isChecked());
    if (ui->systemLangCbox->isChecked())
    {
        LANG = QLocale::languageToString(QLocale::system().language());
    }
    //Use custom language only if "useSystemLang" is not checked
    else
    {
        int i = langsDisplay.indexOf(ui->langComboBox->currentText());
        if (i != -1)
        {
            settings.setValue("lang", langs[i]);
            settings.endGroup();
            LANG = langs[i];
        }
    }

    emit ChangeLang(LANG);

    close();
}

void Settings::on_systemLangCbox_clicked(bool checked)
{
    // Disable language choosing if the "use system language" is selected
    ui->groupBox->setEnabled(!checked);
}
