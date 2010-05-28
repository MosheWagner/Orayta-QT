/********************************************************************************
** Form generated from reading ui file 'errorreport.ui'
**
** Created: Fri May 28 09:44:06 2010
**      by: Qt User Interface Compiler version 4.5.2
**
** WARNING! All changes made in this file will be lost when recompiling ui file!
********************************************************************************/

#ifndef UI_ERRORREPORT_H
#define UI_ERRORREPORT_H

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
#include <QtGui/QTextEdit>
#include <QtGui/QVBoxLayout>

QT_BEGIN_NAMESPACE

class Ui_errorReport
{
public:
    QVBoxLayout *verticalLayout;
    QLabel *label_2;
    QLineEdit *lineEdit;
    QLabel *label;
    QTextEdit *textEdit;
    QHBoxLayout *horizontalLayout;
    QPushButton *cancel;
    QSpacerItem *horizontalSpacer;
    QPushButton *accept;

    void setupUi(QDialog *errorReport)
    {
        if (errorReport->objectName().isEmpty())
            errorReport->setObjectName(QString::fromUtf8("errorReport"));
        errorReport->resize(348, 229);
        errorReport->setLayoutDirection(Qt::LeftToRight);
        verticalLayout = new QVBoxLayout(errorReport);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        label_2 = new QLabel(errorReport);
        label_2->setObjectName(QString::fromUtf8("label_2"));

        verticalLayout->addWidget(label_2);

        lineEdit = new QLineEdit(errorReport);
        lineEdit->setObjectName(QString::fromUtf8("lineEdit"));
        lineEdit->setEnabled(false);

        verticalLayout->addWidget(lineEdit);

        label = new QLabel(errorReport);
        label->setObjectName(QString::fromUtf8("label"));

        verticalLayout->addWidget(label);

        textEdit = new QTextEdit(errorReport);
        textEdit->setObjectName(QString::fromUtf8("textEdit"));

        verticalLayout->addWidget(textEdit);

        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        horizontalLayout->setContentsMargins(-1, 0, -1, -1);
        cancel = new QPushButton(errorReport);
        cancel->setObjectName(QString::fromUtf8("cancel"));

        horizontalLayout->addWidget(cancel);

        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout->addItem(horizontalSpacer);

        accept = new QPushButton(errorReport);
        accept->setObjectName(QString::fromUtf8("accept"));

        horizontalLayout->addWidget(accept);


        verticalLayout->addLayout(horizontalLayout);


        retranslateUi(errorReport);

        QMetaObject::connectSlotsByName(errorReport);
    } // setupUi

    void retranslateUi(QDialog *errorReport)
    {
        errorReport->setWindowTitle(QApplication::translate("errorReport", "Report typo", 0, QApplication::UnicodeUTF8));
        label_2->setText(QApplication::translate("errorReport", "Typo's location", 0, QApplication::UnicodeUTF8));
        label->setText(QApplication::translate("errorReport", "Describe the typo:", 0, QApplication::UnicodeUTF8));
        cancel->setText(QApplication::translate("errorReport", "Cancel", 0, QApplication::UnicodeUTF8));
        accept->setText(QApplication::translate("errorReport", "Send", 0, QApplication::UnicodeUTF8));
        Q_UNUSED(errorReport);
    } // retranslateUi

};

namespace Ui {
    class errorReport: public Ui_errorReport {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_ERRORREPORT_H
