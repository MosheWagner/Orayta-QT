/********************************************************************************
** Form generated from reading ui file 'searchform.ui'
**
** Created: Thu May 27 16:46:16 2010
**      by: Qt User Interface Compiler version 4.5.2
**
** WARNING! All changes made in this file will be lost when recompiling ui file!
********************************************************************************/

#ifndef UI_SEARCHFORM_H
#define UI_SEARCHFORM_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QCheckBox>
#include <QtGui/QDialog>
#include <QtGui/QGroupBox>
#include <QtGui/QHBoxLayout>
#include <QtGui/QHeaderView>
#include <QtGui/QLineEdit>
#include <QtGui/QPushButton>
#include <QtGui/QRadioButton>
#include <QtGui/QSpacerItem>
#include <QtGui/QVBoxLayout>

QT_BEGIN_NAMESPACE

class Ui_SearchForm
{
public:
    QVBoxLayout *verticalLayout;
    QLineEdit *lineEdit;
    QGroupBox *groupBox;
    QVBoxLayout *verticalLayout_2;
    QHBoxLayout *horizontalLayout_3;
    QRadioButton *radioButton_2;
    QRadioButton *radioButton;
    QCheckBox *fullCheckBox;
    QCheckBox *regexpCheckBox;
    QHBoxLayout *horizontalLayout;
    QPushButton *cancel;
    QSpacerItem *horizontalSpacer;
    QPushButton *submit;

    void setupUi(QDialog *SearchForm)
    {
        if (SearchForm->objectName().isEmpty())
            SearchForm->setObjectName(QString::fromUtf8("SearchForm"));
        SearchForm->resize(400, 224);
        SearchForm->setLayoutDirection(Qt::LeftToRight);
        verticalLayout = new QVBoxLayout(SearchForm);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        lineEdit = new QLineEdit(SearchForm);
        lineEdit->setObjectName(QString::fromUtf8("lineEdit"));

        verticalLayout->addWidget(lineEdit);

        groupBox = new QGroupBox(SearchForm);
        groupBox->setObjectName(QString::fromUtf8("groupBox"));
        verticalLayout_2 = new QVBoxLayout(groupBox);
        verticalLayout_2->setObjectName(QString::fromUtf8("verticalLayout_2"));
        horizontalLayout_3 = new QHBoxLayout();
        horizontalLayout_3->setObjectName(QString::fromUtf8("horizontalLayout_3"));
        horizontalLayout_3->setContentsMargins(-1, 0, -1, -1);
        radioButton_2 = new QRadioButton(groupBox);
        radioButton_2->setObjectName(QString::fromUtf8("radioButton_2"));
        radioButton_2->setChecked(true);

        horizontalLayout_3->addWidget(radioButton_2);

        radioButton = new QRadioButton(groupBox);
        radioButton->setObjectName(QString::fromUtf8("radioButton"));

        horizontalLayout_3->addWidget(radioButton);


        verticalLayout_2->addLayout(horizontalLayout_3);

        fullCheckBox = new QCheckBox(groupBox);
        fullCheckBox->setObjectName(QString::fromUtf8("fullCheckBox"));

        verticalLayout_2->addWidget(fullCheckBox);


        verticalLayout->addWidget(groupBox);

        regexpCheckBox = new QCheckBox(SearchForm);
        regexpCheckBox->setObjectName(QString::fromUtf8("regexpCheckBox"));

        verticalLayout->addWidget(regexpCheckBox);

        horizontalLayout = new QHBoxLayout();
#ifndef Q_OS_MAC
        horizontalLayout->setSpacing(-1);
#endif
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        cancel = new QPushButton(SearchForm);
        cancel->setObjectName(QString::fromUtf8("cancel"));
        cancel->setFocusPolicy(Qt::NoFocus);

        horizontalLayout->addWidget(cancel);

        horizontalSpacer = new QSpacerItem(160, 20, QSizePolicy::Maximum, QSizePolicy::Minimum);

        horizontalLayout->addItem(horizontalSpacer);

        submit = new QPushButton(SearchForm);
        submit->setObjectName(QString::fromUtf8("submit"));

        horizontalLayout->addWidget(submit);


        verticalLayout->addLayout(horizontalLayout);

        QWidget::setTabOrder(regexpCheckBox, lineEdit);
        QWidget::setTabOrder(lineEdit, fullCheckBox);
        QWidget::setTabOrder(fullCheckBox, radioButton);
        QWidget::setTabOrder(radioButton, radioButton_2);

        retranslateUi(SearchForm);

        QMetaObject::connectSlotsByName(SearchForm);
    } // setupUi

    void retranslateUi(QDialog *SearchForm)
    {
        SearchForm->setWindowTitle(QApplication::translate("SearchForm", "Advanced search", 0, QApplication::UnicodeUTF8));
        groupBox->setTitle(QApplication::translate("SearchForm", "String search", 0, QApplication::UnicodeUTF8));
        radioButton_2->setText(QApplication::translate("SearchForm", "Find all words", 0, QApplication::UnicodeUTF8));
        radioButton->setText(QApplication::translate("SearchForm", "Find any of the words", 0, QApplication::UnicodeUTF8));
        fullCheckBox->setText(QApplication::translate("SearchForm", "Find full words only", 0, QApplication::UnicodeUTF8));
        regexpCheckBox->setText(QApplication::translate("SearchForm", "Find regular expression", 0, QApplication::UnicodeUTF8));
        cancel->setText(QApplication::translate("SearchForm", "Cancel", 0, QApplication::UnicodeUTF8));
        submit->setText(QApplication::translate("SearchForm", "Search", 0, QApplication::UnicodeUTF8));
        Q_UNUSED(SearchForm);
    } // retranslateUi

};

namespace Ui {
    class SearchForm: public Ui_SearchForm {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_SEARCHFORM_H
