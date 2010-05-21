/********************************************************************************
** Form generated from reading ui file 'addcomment.ui'
**
** Created: Fri May 21 14:01:20 2010
**      by: Qt User Interface Compiler version 4.5.2
**
** WARNING! All changes made in this file will be lost when recompiling ui file!
********************************************************************************/

#ifndef UI_ADDCOMMENT_H
#define UI_ADDCOMMENT_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QDialog>
#include <QtGui/QHBoxLayout>
#include <QtGui/QHeaderView>
#include <QtGui/QPushButton>
#include <QtGui/QSpacerItem>
#include <QtGui/QTextEdit>
#include <QtGui/QVBoxLayout>

QT_BEGIN_NAMESPACE

class Ui_AddComment
{
public:
    QVBoxLayout *verticalLayout;
    QTextEdit *textEdit;
    QHBoxLayout *horizontalLayout;
    QPushButton *erase;
    QSpacerItem *horizontalSpacer_2;
    QPushButton *cancel;
    QPushButton *ok;

    void setupUi(QDialog *AddComment)
    {
        if (AddComment->objectName().isEmpty())
            AddComment->setObjectName(QString::fromUtf8("AddComment"));
        AddComment->setWindowModality(Qt::WindowModal);
        AddComment->resize(400, 164);
        AddComment->setLayoutDirection(Qt::LeftToRight);
        verticalLayout = new QVBoxLayout(AddComment);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        textEdit = new QTextEdit(AddComment);
        textEdit->setObjectName(QString::fromUtf8("textEdit"));
        textEdit->setLayoutDirection(Qt::LeftToRight);

        verticalLayout->addWidget(textEdit);

        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        horizontalLayout->setContentsMargins(0, 0, -1, -1);
        erase = new QPushButton(AddComment);
        erase->setObjectName(QString::fromUtf8("erase"));

        horizontalLayout->addWidget(erase);

        horizontalSpacer_2 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout->addItem(horizontalSpacer_2);

        cancel = new QPushButton(AddComment);
        cancel->setObjectName(QString::fromUtf8("cancel"));

        horizontalLayout->addWidget(cancel);

        ok = new QPushButton(AddComment);
        ok->setObjectName(QString::fromUtf8("ok"));

        horizontalLayout->addWidget(ok);


        verticalLayout->addLayout(horizontalLayout);


        retranslateUi(AddComment);

        QMetaObject::connectSlotsByName(AddComment);
    } // setupUi

    void retranslateUi(QDialog *AddComment)
    {
        AddComment->setWindowTitle(QApplication::translate("AddComment", "Add comment", 0, QApplication::UnicodeUTF8));
        erase->setText(QApplication::translate("AddComment", "Delete comment", 0, QApplication::UnicodeUTF8));
        cancel->setText(QApplication::translate("AddComment", "Cancel", 0, QApplication::UnicodeUTF8));
        ok->setText(QApplication::translate("AddComment", "Save", 0, QApplication::UnicodeUTF8));
        Q_UNUSED(AddComment);
    } // retranslateUi

};

namespace Ui {
    class AddComment: public Ui_AddComment {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_ADDCOMMENT_H
