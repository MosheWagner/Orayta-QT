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

#include "bookfind.h"
#include "ui_bookfind.h"
#include "settings.h"

bookfind::bookfind(QWidget *parent, const BookList& booklist) :
    QDialog(parent),
    m_ui(new Ui::bookfind),
    mBookList(booklist)
{
    m_ui->setupUi(this);

    if (LANG == "Hebrew") toRTL();
}

void bookfind::toRTL()
{
    this->setLayoutDirection(Qt::RightToLeft);
}

bookfind::~bookfind()
{
    delete m_ui;
}

void bookfind::on_lineEdit_textEdited(const QString& text)
{
    reBuildList(text);
}

void bookfind::reBuildList(const QString& text)
{
    m_ui->listWidget->clear();

    if ( text != "")
    {
        QListWidgetItem *item = NULL;
        for (BookList::const_iterator it = mBookList.begin(); it != mBookList.end(); ++it)
        {
            QString bookname = (*it)->getTreeDisplayName();
/*
                //Mixed display book
                if ( !mBookList[i]->ShowAlone() ) bookname = bookname + " [תצוגה משולבת]";
*/
            if ( m_ui->radioButton->isChecked() ? bookname.indexOf(text) != -1 : bookname.startsWith(text))
            {
                item = new QListWidgetItem();
                item->setText( bookname );
                item->setData(Qt::UserRole, QVariant::fromValue((void*)*it));
                m_ui->listWidget->addItem(item);
            }
        }
    }
}

void bookfind::on_radioButton_2_toggled(bool checked)
{
    reBuildList( m_ui->lineEdit->text() );
}

void bookfind::on_pushButton_2_clicked()
{
    close();
}

void bookfind::on_pushButton_clicked()
{
    QListWidgetItem *item = m_ui->listWidget->currentItem();
    if ( item )
    {
        NodeBook* book = static_cast<NodeBook*>(item->data(Qt::UserRole).value<void*>());
        emit openBookRequested( book );
        close();
    }    
}

void bookfind::on_listWidget_itemDoubleClicked(QListWidgetItem *item)
{
    NodeBook* book = static_cast<NodeBook*>(item->data(Qt::UserRole).value<void*>());
    emit openBookRequested( book );
}
