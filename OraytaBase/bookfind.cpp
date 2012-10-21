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
#include "functions.h"
#ifdef  MOBILE
    #include "Mobile/QKinetic/flickcharm.h"
#else
    #include "Desktop/desktopapp.h"
#endif


bookfind::bookfind(QWidget *parent, BookList& booklist) : QDialog(parent), m_ui(new Ui::bookfind), mBookList(booklist)
{
    m_ui->setupUi(this);

    m_ui->radioButton->setChecked(true);

#ifdef MOBILE
    m_ui->buttonGRP->hide();
    m_ui->radioGRP->hide();
    FlickCharm *f = new FlickCharm(this);
    f->activateOn(m_ui->listWidget);
#endif //mobile

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

void bookfind::on_lineEdit_textEdited(QString text)
{
    reBuildList(text);
}

void bookfind::reBuildList(QString text)
{
    m_ui->listWidget->clear();

    if ( text != "")
    {
        QListWidgetItem *item = NULL;
        for (unsigned int i=0; i<mBookList.size(); i++)
        {
            if (!mBookList[i]->IsDir())
            {
                QString bookname = mBookList[i]->getNormallDisplayName();

                //Mixed display book
                if ( !mBookList[i]->ShowAlone() ) bookname = bookname + " [תצוגה משולבת]";

                if ( m_ui->radioButton->isChecked())
                {
                    if ( bookname.indexOf(text) != -1)
                    {
                        item = new QListWidgetItem();
                        item->setText(bookname);
                        m_ui->listWidget->addItem(item);

                        item->setData(Qt::UserRole, mBookList[i]->getUniqueId());
                    }
                }
                else
                {
                    if (bookname.startsWith(text))
                    {
                        item = new QListWidgetItem();
                        item->setText( bookname );
                        m_ui->listWidget->addItem(item);

                        item->setData(Qt::UserRole, mBookList[i]->getUniqueId());
                    }
                }
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
        int bookid = item->data(Qt::UserRole).toInt();
        emit openBook(bookid);
        close();
    }    
}

void bookfind::on_listWidget_itemDoubleClicked(QListWidgetItem *item)
{
    int bookid = item->data(Qt::UserRole).toInt();
    emit openBook( bookid );
}

#ifdef MOBILE
void bookfind::on_listWidget_itemClicked(QListWidgetItem *item)
    {on_listWidget_itemDoubleClicked(item);}
#endif

void bookfind::Reset()
{
     m_ui->lineEdit->clear();
     reBuildList("");
}

void bookfind::on_clearSearchBTN_clicked()
{
   Reset();
}

void bookfind::on_lineEdit_textChanged(const QString &arg1)
{
    reBuildList(arg1);
}
