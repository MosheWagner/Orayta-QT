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

#include "searchform.h"

SearchForm::SearchForm(QWidget *parent) : QDialog(parent), m_ui(new Ui::SearchForm)
{
    m_ui->setupUi(this);

    if (LANG == "Hebrew") toRTL();

    m_ui->lineEdit->setFocus();

}

void SearchForm::toRTL()
{
    this->setLayoutDirection(Qt::RightToLeft);
}

SearchForm::~SearchForm()
{
    delete m_ui;
}

void SearchForm::on_submit_clicked()
{
    QString otxt = m_ui->lineEdit->text();
    QString stxt = m_ui->lineEdit->text();

    if (m_ui->regexpCheckBox->isChecked())
    {
        emit Search(QRegExp(stxt), tr("RegExp: ") + otxt);
    }
    else
    {
        if ( m_ui->radioButton->isChecked() )
        {
            if ( m_ui->fullCheckBox->isChecked()) stxt = "[ :.,-]" + stxt + "[ :.,-]";

            QString pat = "";

            QStringList words = stxt.split(" ");
            for (int i=0; i<words.size(); i++)
            {
                if (words[i] != "") pat += allowNikudAndTeamim(words[i]);
                if ( i != words.size()-1 ) pat+= "|";
            }
            //print (pat);

            emit Search(QRegExp(pat), otxt);
        }
        else
        {
            stxt = allowNikudAndTeamim(stxt);
            if ( m_ui->fullCheckBox->isChecked()) stxt = "[ :.,-]" + stxt + "[ :.,-]";

            emit Search(QRegExp(stxt), otxt);
        }
    }
    close();
}

void SearchForm::on_cancel_clicked()
{
    close();
}


void SearchForm::on_regexpCheckBox_clicked(bool checked)
{
    m_ui->groupBox->setEnabled(!checked);
}
