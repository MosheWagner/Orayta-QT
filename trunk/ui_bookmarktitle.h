/********************************************************************************
** Form generated from reading ui file 'bookmarktitle.ui'
**
** Created: Fri May 28 09:44:06 2010
**      by: Qt User Interface Compiler version 4.5.2
**
** WARNING! All changes made in this file will be lost when recompiling ui file!
********************************************************************************/

#ifndef UI_BOOKMARKTITLE_H
#define UI_BOOKMARKTITLE_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QDialog>
#include <QtGui/QHBoxLayout>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>
#include <QtGui/QLineEdit>
#include <QtGui/QPushButton>
#include <QtGui/QSpacerItem>
#include <QtGui/QVBoxLayout>

QT_BEGIN_NAMESPACE

class Ui_BookMarkTitle
{
public:
    QVBoxLayout *verticalLayout;
    QLabel *label;
    QLineEdit *lineEdit;
    QHBoxLayout *horizontalLayout;
    QPushButton *cancel;
    QSpacerItem *horizontalSpacer;
    QPushButton *ok;

    void setupUi(QDialog *BookMarkTitle)
    {
        if (BookMarkTitle->objectName().isEmpty())
            BookMarkTitle->setObjectName(QString::fromUtf8("BookMarkTitle"));
        BookMarkTitle->setWindowModality(Qt::WindowModal);
        BookMarkTitle->resize(269, 105);
        verticalLayout = new QVBoxLayout(BookMarkTitle);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        label = new QLabel(BookMarkTitle);
        label->setObjectName(QString::fromUtf8("label"));
        label->setLayoutDirection(Qt::LeftToRight);

        verticalLayout->addWidget(label);

        lineEdit = new QLineEdit(BookMarkTitle);
        lineEdit->setObjectName(QString::fromUtf8("lineEdit"));
        lineEdit->setLayoutDirection(Qt::RightToLeft);

        verticalLayout->addWidget(lineEdit);

        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        cancel = new QPushButton(BookMarkTitle);
        cancel->setObjectName(QString::fromUtf8("cancel"));

        horizontalLayout->addWidget(cancel);

        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout->addItem(horizontalSpacer);

        ok = new QPushButton(BookMarkTitle);
        ok->setObjectName(QString::fromUtf8("ok"));

        horizontalLayout->addWidget(ok);


        verticalLayout->addLayout(horizontalLayout);


        retranslateUi(BookMarkTitle);

        QMetaObject::connectSlotsByName(BookMarkTitle);
    } // setupUi

    void retranslateUi(QDialog *BookMarkTitle)
    {
        BookMarkTitle->setWindowTitle(QApplication::translate("BookMarkTitle", "Bookmark's title", 0, QApplication::UnicodeUTF8));
        label->setText(QApplication::translate("BookMarkTitle", "Bookmark's title:", 0, QApplication::UnicodeUTF8));
        cancel->setText(QApplication::translate("BookMarkTitle", "Cancel", 0, QApplication::UnicodeUTF8));
        ok->setText(QApplication::translate("BookMarkTitle", "OK", 0, QApplication::UnicodeUTF8));
        Q_UNUSED(BookMarkTitle);
    } // retranslateUi

};

namespace Ui {
    class BookMarkTitle: public Ui_BookMarkTitle {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_BOOKMARKTITLE_H
