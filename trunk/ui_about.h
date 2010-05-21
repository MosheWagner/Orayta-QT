/********************************************************************************
** Form generated from reading ui file 'about.ui'
**
** Created: Fri May 21 14:01:20 2010
**      by: Qt User Interface Compiler version 4.5.2
**
** WARNING! All changes made in this file will be lost when recompiling ui file!
********************************************************************************/

#ifndef UI_ABOUT_H
#define UI_ABOUT_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QDialog>
#include <QtGui/QHBoxLayout>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>

QT_BEGIN_NAMESPACE

class Ui_About
{
public:
    QHBoxLayout *horizontalLayout;
    QLabel *label;

    void setupUi(QDialog *About)
    {
        if (About->objectName().isEmpty())
            About->setObjectName(QString::fromUtf8("About"));
        About->resize(400, 112);
        About->setLayoutDirection(Qt::RightToLeft);
        horizontalLayout = new QHBoxLayout(About);
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        label = new QLabel(About);
        label->setObjectName(QString::fromUtf8("label"));

        horizontalLayout->addWidget(label);


        retranslateUi(About);

        QMetaObject::connectSlotsByName(About);
    } // setupUi

    void retranslateUi(QDialog *About)
    {
        About->setWindowTitle(QApplication::translate("About", "About Orayta", 0, QApplication::UnicodeUTF8));
        label->setText(QString());
        Q_UNUSED(About);
    } // retranslateUi

};

namespace Ui {
    class About: public Ui_About {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_ABOUT_H
