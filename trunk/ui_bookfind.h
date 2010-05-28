/********************************************************************************
** Form generated from reading ui file 'bookfind.ui'
**
** Created: Fri May 28 09:44:06 2010
**      by: Qt User Interface Compiler version 4.5.2
**
** WARNING! All changes made in this file will be lost when recompiling ui file!
********************************************************************************/

#ifndef UI_BOOKFIND_H
#define UI_BOOKFIND_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QDialog>
#include <QtGui/QHBoxLayout>
#include <QtGui/QHeaderView>
#include <QtGui/QLineEdit>
#include <QtGui/QListWidget>
#include <QtGui/QPushButton>
#include <QtGui/QRadioButton>
#include <QtGui/QVBoxLayout>

QT_BEGIN_NAMESPACE

class Ui_bookfind
{
public:
    QVBoxLayout *verticalLayout;
    QLineEdit *lineEdit;
    QRadioButton *radioButton_2;
    QRadioButton *radioButton;
    QListWidget *listWidget;
    QHBoxLayout *horizontalLayout;
    QPushButton *pushButton_2;
    QPushButton *pushButton;

    void setupUi(QDialog *bookfind)
    {
        if (bookfind->objectName().isEmpty())
            bookfind->setObjectName(QString::fromUtf8("bookfind"));
        bookfind->resize(273, 339);
        bookfind->setLayoutDirection(Qt::LeftToRight);
        verticalLayout = new QVBoxLayout(bookfind);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        lineEdit = new QLineEdit(bookfind);
        lineEdit->setObjectName(QString::fromUtf8("lineEdit"));

        verticalLayout->addWidget(lineEdit);

        radioButton_2 = new QRadioButton(bookfind);
        radioButton_2->setObjectName(QString::fromUtf8("radioButton_2"));
        radioButton_2->setChecked(true);

        verticalLayout->addWidget(radioButton_2);

        radioButton = new QRadioButton(bookfind);
        radioButton->setObjectName(QString::fromUtf8("radioButton"));

        verticalLayout->addWidget(radioButton);

        listWidget = new QListWidget(bookfind);
        listWidget->setObjectName(QString::fromUtf8("listWidget"));

        verticalLayout->addWidget(listWidget);

        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setSpacing(24);
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        horizontalLayout->setContentsMargins(-1, 0, -1, -1);
        pushButton_2 = new QPushButton(bookfind);
        pushButton_2->setObjectName(QString::fromUtf8("pushButton_2"));
        pushButton_2->setLayoutDirection(Qt::LeftToRight);

        horizontalLayout->addWidget(pushButton_2);

        pushButton = new QPushButton(bookfind);
        pushButton->setObjectName(QString::fromUtf8("pushButton"));

        horizontalLayout->addWidget(pushButton);


        verticalLayout->addLayout(horizontalLayout);


        retranslateUi(bookfind);

        QMetaObject::connectSlotsByName(bookfind);
    } // setupUi

    void retranslateUi(QDialog *bookfind)
    {
        bookfind->setWindowTitle(QApplication::translate("bookfind", "find book by name", 0, QApplication::UnicodeUTF8));
        radioButton_2->setText(QApplication::translate("bookfind", "Search in name's beginning", 0, QApplication::UnicodeUTF8));
        radioButton->setText(QApplication::translate("bookfind", "Search in the whole name", 0, QApplication::UnicodeUTF8));
        pushButton_2->setText(QApplication::translate("bookfind", "Cancel", 0, QApplication::UnicodeUTF8));
        pushButton->setText(QApplication::translate("bookfind", "Open book", 0, QApplication::UnicodeUTF8));
        Q_UNUSED(bookfind);
    } // retranslateUi

};

namespace Ui {
    class bookfind: public Ui_bookfind {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_BOOKFIND_H
