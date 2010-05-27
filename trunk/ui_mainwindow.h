/********************************************************************************
** Form generated from reading ui file 'mainwindow.ui'
**
** Created: Thu May 27 16:46:15 2010
**      by: Qt User Interface Compiler version 4.5.2
**
** WARNING! All changes made in this file will be lost when recompiling ui file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QLocale>
#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QComboBox>
#include <QtGui/QDockWidget>
#include <QtGui/QFontComboBox>
#include <QtGui/QFrame>
#include <QtGui/QGroupBox>
#include <QtGui/QHBoxLayout>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>
#include <QtGui/QLineEdit>
#include <QtGui/QListWidget>
#include <QtGui/QMainWindow>
#include <QtGui/QMenu>
#include <QtGui/QMenuBar>
#include <QtGui/QProgressBar>
#include <QtGui/QPushButton>
#include <QtGui/QScrollArea>
#include <QtGui/QSpacerItem>
#include <QtGui/QSpinBox>
#include <QtGui/QTabWidget>
#include <QtGui/QToolButton>
#include <QtGui/QTreeWidget>
#include <QtGui/QVBoxLayout>
#include <QtGui/QWidget>
#include <mytreetab.h>

QT_BEGIN_NAMESPACE

class Ui_MainWindowClass
{
public:
    QAction *exitAction;
    QAction *aboutAction;
    QAction *zoominAction;
    QAction *zoomoutAction;
    QAction *jumptotopAction;
    QAction *printAction;
    QAction *closetabAction;
    QAction *opentabAction;
    QAction *showNikudAction;
    QAction *showTeamimAction;
    QAction *bookMarkAction;
    QAction *commentAction;
    QAction *reportErrorAction;
    QAction *advancedSearchAction;
    QAction *searchInBooksAction;
    QAction *searchForwardAction;
    QAction *searchBackwardsAction;
    QAction *removeFromSearchAction;
    QAction *addToSearchAction;
    QAction *addAllToSearchAction;
    QAction *removeAllFromSearchAction;
    QAction *findBookAction;
    QWidget *centralWidget;
    QVBoxLayout *verticalLayout_2;
    QHBoxLayout *horizontalLayout;
    QTabWidget *viewTab;
    QWidget *tab1;
    QPushButton *newTabButton;
    QDockWidget *searchDockWidget;
    QWidget *dockWidgetContents;
    QVBoxLayout *verticalLayout_4;
    QHBoxLayout *horizontalLayout_3;
    QLabel *label;
    QLineEdit *lineEdit;
    QToolButton *searchButton;
    QToolButton *searchBackwords;
    QToolButton *searchForward;
    QSpacerItem *horizontalSpacer;
    QToolButton *zoomoutButton;
    QToolButton *zoominButton;
    QToolButton *topButton;
    QDockWidget *treeDockWidget;
    QWidget *dockWidgetContents_2;
    QVBoxLayout *verticalLayout_5;
    QVBoxLayout *verticalLayout_9;
    myTreeTab *treeTab;
    QWidget *tab;
    QHBoxLayout *horizontalLayout_4;
    QVBoxLayout *verticalLayout;
    QHBoxLayout *horizontalLayout_2;
    QToolButton *addAllToSearchButton;
    QToolButton *removeAllFromSearchButton;
    QSpacerItem *horizontalSpacer_4;
    QTreeWidget *treeWidget;
    QWidget *tab_2;
    QVBoxLayout *verticalLayout_3;
    QHBoxLayout *horizontalLayout_5;
    QToolButton *removeBookmark;
    QToolButton *bookmarkUp;
    QToolButton *bookmarkDown;
    QToolButton *bookmarkEdit;
    QSpacerItem *horizontalSpacer_3;
    QListWidget *bookmarkWidget;
    QWidget *tab_3;
    QVBoxLayout *verticalLayout_6;
    QScrollArea *scrollArea;
    QWidget *scrollAreaWidgetContents;
    QVBoxLayout *verticalLayout_7;
    QLabel *label_9;
    QFontComboBox *fontComboBox;
    QLabel *label_4;
    QSpinBox *fonSizeSpinBox;
    QFrame *line_2;
    QLabel *label_6;
    QLabel *label_3;
    QLineEdit *fontPreview;
    QFrame *line_3;
    QHBoxLayout *horizontalLayout_6;
    QSpacerItem *horizontalSpacer_2;
    QPushButton *saveConf;
    QSpacerItem *horizontalSpacer_5;
    QLabel *label_5;
    QSpacerItem *verticalSpacer;
    QLabel *label_7;
    QComboBox *langComboBox;
    QLabel *label_8;
    QHBoxLayout *horizontalLayout_7;
    QSpacerItem *horizontalSpacer_6;
    QPushButton *changeLangButton;
    QSpacerItem *horizontalSpacer_7;
    QFrame *mixedGroup;
    QVBoxLayout *verticalLayout_8;
    QGroupBox *mixedFrame;
    QVBoxLayout *verticalLayout_10;
    QVBoxLayout *verticalLayout_11;
    QHBoxLayout *horizontalLayout_8;
    QSpacerItem *horizontalSpacer_8;
    QPushButton *openMixed;
    QSpacerItem *horizontalSpacer_9;
    QProgressBar *progressBar;
    QMenuBar *menuBar;
    QMenu *menu;
    QMenu *menu_2;
    QMenu *menu_3;
    QMenu *menu_5;
    QMenu *menu_4;

    void setupUi(QMainWindow *MainWindowClass)
    {
        if (MainWindowClass->objectName().isEmpty())
            MainWindowClass->setObjectName(QString::fromUtf8("MainWindowClass"));
        MainWindowClass->resize(1097, 725);
        QSizePolicy sizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(MainWindowClass->sizePolicy().hasHeightForWidth());
        MainWindowClass->setSizePolicy(sizePolicy);
        MainWindowClass->setMinimumSize(QSize(0, 0));
        QIcon icon;
        icon.addFile(QString::fromUtf8(":/Icons/Orayta.png"), QSize(), QIcon::Normal, QIcon::Off);
        MainWindowClass->setWindowIcon(icon);
        MainWindowClass->setLayoutDirection(Qt::LeftToRight);
        MainWindowClass->setLocale(QLocale(QLocale::Hebrew, QLocale::Israel));
        exitAction = new QAction(MainWindowClass);
        exitAction->setObjectName(QString::fromUtf8("exitAction"));
        QIcon icon1;
        icon1.addFile(QString::fromUtf8(":/Icons/edit-delete.png"), QSize(), QIcon::Normal, QIcon::Off);
        exitAction->setIcon(icon1);
        aboutAction = new QAction(MainWindowClass);
        aboutAction->setObjectName(QString::fromUtf8("aboutAction"));
        QIcon icon2;
        icon2.addFile(QString::fromUtf8(":/Icons/help-about.png"), QSize(), QIcon::Normal, QIcon::Off);
        aboutAction->setIcon(icon2);
        zoominAction = new QAction(MainWindowClass);
        zoominAction->setObjectName(QString::fromUtf8("zoominAction"));
        QIcon icon3;
        icon3.addFile(QString::fromUtf8(":/Icons/zoom-in.png"), QSize(), QIcon::Normal, QIcon::Off);
        zoominAction->setIcon(icon3);
        zoomoutAction = new QAction(MainWindowClass);
        zoomoutAction->setObjectName(QString::fromUtf8("zoomoutAction"));
        QIcon icon4;
        icon4.addFile(QString::fromUtf8(":/Icons/zoom-out.png"), QSize(), QIcon::Normal, QIcon::Off);
        zoomoutAction->setIcon(icon4);
        jumptotopAction = new QAction(MainWindowClass);
        jumptotopAction->setObjectName(QString::fromUtf8("jumptotopAction"));
        QIcon icon5;
        icon5.addFile(QString::fromUtf8(":/Icons/arrow-up.png"), QSize(), QIcon::Normal, QIcon::Off);
        jumptotopAction->setIcon(icon5);
        printAction = new QAction(MainWindowClass);
        printAction->setObjectName(QString::fromUtf8("printAction"));
        QIcon icon6;
        icon6.addFile(QString::fromUtf8(":/Icons/document-print.png"), QSize(), QIcon::Normal, QIcon::Off);
        printAction->setIcon(icon6);
        closetabAction = new QAction(MainWindowClass);
        closetabAction->setObjectName(QString::fromUtf8("closetabAction"));
        QIcon icon7;
        icon7.addFile(QString::fromUtf8(":/Icons/tab-close.png"), QSize(), QIcon::Normal, QIcon::Off);
        closetabAction->setIcon(icon7);
        opentabAction = new QAction(MainWindowClass);
        opentabAction->setObjectName(QString::fromUtf8("opentabAction"));
        QIcon icon8;
        icon8.addFile(QString::fromUtf8(":/Icons/tab-new.png"), QSize(), QIcon::Normal, QIcon::Off);
        opentabAction->setIcon(icon8);
        showNikudAction = new QAction(MainWindowClass);
        showNikudAction->setObjectName(QString::fromUtf8("showNikudAction"));
        showNikudAction->setCheckable(true);
        showNikudAction->setChecked(true);
        showTeamimAction = new QAction(MainWindowClass);
        showTeamimAction->setObjectName(QString::fromUtf8("showTeamimAction"));
        showTeamimAction->setCheckable(true);
        showTeamimAction->setChecked(true);
        bookMarkAction = new QAction(MainWindowClass);
        bookMarkAction->setObjectName(QString::fromUtf8("bookMarkAction"));
        QIcon icon9;
        icon9.addFile(QString::fromUtf8(":/Icons/bookmarks.png"), QSize(), QIcon::Normal, QIcon::Off);
        bookMarkAction->setIcon(icon9);
        commentAction = new QAction(MainWindowClass);
        commentAction->setObjectName(QString::fromUtf8("commentAction"));
        QIcon icon10;
        icon10.addFile(QString::fromUtf8(":/Icons/edit.png"), QSize(), QIcon::Normal, QIcon::Off);
        commentAction->setIcon(icon10);
        reportErrorAction = new QAction(MainWindowClass);
        reportErrorAction->setObjectName(QString::fromUtf8("reportErrorAction"));
        QIcon icon11;
        icon11.addFile(QString::fromUtf8(":/Icons/bug.png"), QSize(), QIcon::Normal, QIcon::Off);
        reportErrorAction->setIcon(icon11);
        advancedSearchAction = new QAction(MainWindowClass);
        advancedSearchAction->setObjectName(QString::fromUtf8("advancedSearchAction"));
        QIcon icon12;
        icon12.addFile(QString::fromUtf8(":/Icons/search.png"), QSize(), QIcon::Normal, QIcon::Off);
        advancedSearchAction->setIcon(icon12);
        searchInBooksAction = new QAction(MainWindowClass);
        searchInBooksAction->setObjectName(QString::fromUtf8("searchInBooksAction"));
        QIcon icon13;
        icon13.addFile(QString::fromUtf8(":/Icons/edit-find.png"), QSize(), QIcon::Normal, QIcon::Off);
        searchInBooksAction->setIcon(icon13);
        searchForwardAction = new QAction(MainWindowClass);
        searchForwardAction->setObjectName(QString::fromUtf8("searchForwardAction"));
        QIcon icon14;
        icon14.addFile(QString::fromUtf8(":/Icons/search-right.png"), QSize(), QIcon::Normal, QIcon::Off);
        searchForwardAction->setIcon(icon14);
        searchBackwardsAction = new QAction(MainWindowClass);
        searchBackwardsAction->setObjectName(QString::fromUtf8("searchBackwardsAction"));
        QIcon icon15;
        icon15.addFile(QString::fromUtf8(":/Icons/search-left.png"), QSize(), QIcon::Normal, QIcon::Off);
        searchBackwardsAction->setIcon(icon15);
        removeFromSearchAction = new QAction(MainWindowClass);
        removeFromSearchAction->setObjectName(QString::fromUtf8("removeFromSearchAction"));
        QIcon icon16;
        icon16.addFile(QString::fromUtf8(":/Icons/greyv.png"), QSize(), QIcon::Normal, QIcon::Off);
        removeFromSearchAction->setIcon(icon16);
        addToSearchAction = new QAction(MainWindowClass);
        addToSearchAction->setObjectName(QString::fromUtf8("addToSearchAction"));
        QIcon icon17;
        icon17.addFile(QString::fromUtf8(":/Icons/greenv.png"), QSize(), QIcon::Normal, QIcon::Off);
        addToSearchAction->setIcon(icon17);
        addAllToSearchAction = new QAction(MainWindowClass);
        addAllToSearchAction->setObjectName(QString::fromUtf8("addAllToSearchAction"));
        QIcon icon18;
        icon18.addFile(QString::fromUtf8(":/Icons/doublegreenv.png"), QSize(), QIcon::Normal, QIcon::Off);
        addAllToSearchAction->setIcon(icon18);
        removeAllFromSearchAction = new QAction(MainWindowClass);
        removeAllFromSearchAction->setObjectName(QString::fromUtf8("removeAllFromSearchAction"));
        QIcon icon19;
        icon19.addFile(QString::fromUtf8(":/Icons/doublegreyv.png"), QSize(), QIcon::Normal, QIcon::Off);
        removeAllFromSearchAction->setIcon(icon19);
        findBookAction = new QAction(MainWindowClass);
        findBookAction->setObjectName(QString::fromUtf8("findBookAction"));
        QIcon icon20;
        icon20.addFile(QString::fromUtf8(":/Icons/bookcase.png"), QSize(), QIcon::Normal, QIcon::Off);
        findBookAction->setIcon(icon20);
        centralWidget = new QWidget(MainWindowClass);
        centralWidget->setObjectName(QString::fromUtf8("centralWidget"));
        QSizePolicy sizePolicy1(QSizePolicy::Expanding, QSizePolicy::Expanding);
        sizePolicy1.setHorizontalStretch(1);
        sizePolicy1.setVerticalStretch(1);
        sizePolicy1.setHeightForWidth(centralWidget->sizePolicy().hasHeightForWidth());
        centralWidget->setSizePolicy(sizePolicy1);
        centralWidget->setLayoutDirection(Qt::LeftToRight);
        verticalLayout_2 = new QVBoxLayout(centralWidget);
        verticalLayout_2->setSpacing(6);
        verticalLayout_2->setMargin(11);
        verticalLayout_2->setObjectName(QString::fromUtf8("verticalLayout_2"));
        verticalLayout_2->setContentsMargins(-1, -1, -1, 7);
        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setSpacing(6);
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        horizontalLayout->setSizeConstraint(QLayout::SetDefaultConstraint);
        horizontalLayout->setContentsMargins(-1, -1, 0, -1);
        viewTab = new QTabWidget(centralWidget);
        viewTab->setObjectName(QString::fromUtf8("viewTab"));
        QPalette palette;
        QBrush brush(QColor(0, 0, 0, 255));
        brush.setStyle(Qt::SolidPattern);
        palette.setBrush(QPalette::Active, QPalette::WindowText, brush);
        QBrush brush1(QColor(255, 255, 255, 255));
        brush1.setStyle(Qt::SolidPattern);
        palette.setBrush(QPalette::Active, QPalette::Button, brush1);
        palette.setBrush(QPalette::Active, QPalette::Light, brush1);
        palette.setBrush(QPalette::Active, QPalette::Midlight, brush1);
        QBrush brush2(QColor(127, 127, 127, 255));
        brush2.setStyle(Qt::SolidPattern);
        palette.setBrush(QPalette::Active, QPalette::Dark, brush2);
        QBrush brush3(QColor(170, 170, 170, 255));
        brush3.setStyle(Qt::SolidPattern);
        palette.setBrush(QPalette::Active, QPalette::Mid, brush3);
        palette.setBrush(QPalette::Active, QPalette::Text, brush);
        palette.setBrush(QPalette::Active, QPalette::BrightText, brush1);
        palette.setBrush(QPalette::Active, QPalette::ButtonText, brush);
        palette.setBrush(QPalette::Active, QPalette::Base, brush1);
        palette.setBrush(QPalette::Active, QPalette::Window, brush1);
        palette.setBrush(QPalette::Active, QPalette::Shadow, brush);
        palette.setBrush(QPalette::Active, QPalette::AlternateBase, brush1);
        QBrush brush4(QColor(255, 255, 220, 255));
        brush4.setStyle(Qt::SolidPattern);
        palette.setBrush(QPalette::Active, QPalette::ToolTipBase, brush4);
        palette.setBrush(QPalette::Active, QPalette::ToolTipText, brush);
        palette.setBrush(QPalette::Inactive, QPalette::WindowText, brush);
        palette.setBrush(QPalette::Inactive, QPalette::Button, brush1);
        palette.setBrush(QPalette::Inactive, QPalette::Light, brush1);
        palette.setBrush(QPalette::Inactive, QPalette::Midlight, brush1);
        palette.setBrush(QPalette::Inactive, QPalette::Dark, brush2);
        palette.setBrush(QPalette::Inactive, QPalette::Mid, brush3);
        palette.setBrush(QPalette::Inactive, QPalette::Text, brush);
        palette.setBrush(QPalette::Inactive, QPalette::BrightText, brush1);
        palette.setBrush(QPalette::Inactive, QPalette::ButtonText, brush);
        palette.setBrush(QPalette::Inactive, QPalette::Base, brush1);
        palette.setBrush(QPalette::Inactive, QPalette::Window, brush1);
        palette.setBrush(QPalette::Inactive, QPalette::Shadow, brush);
        palette.setBrush(QPalette::Inactive, QPalette::AlternateBase, brush1);
        palette.setBrush(QPalette::Inactive, QPalette::ToolTipBase, brush4);
        palette.setBrush(QPalette::Inactive, QPalette::ToolTipText, brush);
        palette.setBrush(QPalette::Disabled, QPalette::WindowText, brush2);
        palette.setBrush(QPalette::Disabled, QPalette::Button, brush1);
        palette.setBrush(QPalette::Disabled, QPalette::Light, brush1);
        palette.setBrush(QPalette::Disabled, QPalette::Midlight, brush1);
        palette.setBrush(QPalette::Disabled, QPalette::Dark, brush2);
        palette.setBrush(QPalette::Disabled, QPalette::Mid, brush3);
        palette.setBrush(QPalette::Disabled, QPalette::Text, brush2);
        palette.setBrush(QPalette::Disabled, QPalette::BrightText, brush1);
        palette.setBrush(QPalette::Disabled, QPalette::ButtonText, brush2);
        palette.setBrush(QPalette::Disabled, QPalette::Base, brush1);
        palette.setBrush(QPalette::Disabled, QPalette::Window, brush1);
        palette.setBrush(QPalette::Disabled, QPalette::Shadow, brush);
        palette.setBrush(QPalette::Disabled, QPalette::AlternateBase, brush1);
        palette.setBrush(QPalette::Disabled, QPalette::ToolTipBase, brush4);
        palette.setBrush(QPalette::Disabled, QPalette::ToolTipText, brush);
        viewTab->setPalette(palette);
        viewTab->setLayoutDirection(Qt::LeftToRight);
        viewTab->setTabsClosable(true);
        viewTab->setMovable(false);
        tab1 = new QWidget();
        tab1->setObjectName(QString::fromUtf8("tab1"));
        newTabButton = new QPushButton(tab1);
        newTabButton->setObjectName(QString::fromUtf8("newTabButton"));
        newTabButton->setGeometry(QRect(235, 30, 31, 31));
        newTabButton->setIcon(icon8);
        newTabButton->setIconSize(QSize(21, 21));
        newTabButton->setFlat(true);
        viewTab->addTab(tab1, QString());

        horizontalLayout->addWidget(viewTab);


        verticalLayout_2->addLayout(horizontalLayout);

        MainWindowClass->setCentralWidget(centralWidget);
        searchDockWidget = new QDockWidget(MainWindowClass);
        searchDockWidget->setObjectName(QString::fromUtf8("searchDockWidget"));
        searchDockWidget->setMaximumSize(QSize(524287, 45));
        searchDockWidget->setFloating(false);
        searchDockWidget->setFeatures(QDockWidget::DockWidgetVerticalTitleBar);
        searchDockWidget->setAllowedAreas(Qt::TopDockWidgetArea);
        dockWidgetContents = new QWidget();
        dockWidgetContents->setObjectName(QString::fromUtf8("dockWidgetContents"));
        dockWidgetContents->setLayoutDirection(Qt::LeftToRight);
        verticalLayout_4 = new QVBoxLayout(dockWidgetContents);
        verticalLayout_4->setSpacing(6);
        verticalLayout_4->setMargin(0);
        verticalLayout_4->setObjectName(QString::fromUtf8("verticalLayout_4"));
        horizontalLayout_3 = new QHBoxLayout();
        horizontalLayout_3->setSpacing(2);
        horizontalLayout_3->setObjectName(QString::fromUtf8("horizontalLayout_3"));
        horizontalLayout_3->setContentsMargins(10, 2, 10, 2);
        label = new QLabel(dockWidgetContents);
        label->setObjectName(QString::fromUtf8("label"));

        horizontalLayout_3->addWidget(label);

        lineEdit = new QLineEdit(dockWidgetContents);
        lineEdit->setObjectName(QString::fromUtf8("lineEdit"));

        horizontalLayout_3->addWidget(lineEdit);

        searchButton = new QToolButton(dockWidgetContents);
        searchButton->setObjectName(QString::fromUtf8("searchButton"));
        searchButton->setIcon(icon13);
        searchButton->setIconSize(QSize(19, 19));
        searchButton->setCheckable(false);
        searchButton->setToolButtonStyle(Qt::ToolButtonIconOnly);
        searchButton->setAutoRaise(true);

        horizontalLayout_3->addWidget(searchButton);

        searchBackwords = new QToolButton(dockWidgetContents);
        searchBackwords->setObjectName(QString::fromUtf8("searchBackwords"));
        searchBackwords->setIcon(icon15);
        searchBackwords->setIconSize(QSize(19, 19));
        searchBackwords->setCheckable(false);
        searchBackwords->setToolButtonStyle(Qt::ToolButtonIconOnly);
        searchBackwords->setAutoRaise(true);

        horizontalLayout_3->addWidget(searchBackwords);

        searchForward = new QToolButton(dockWidgetContents);
        searchForward->setObjectName(QString::fromUtf8("searchForward"));
        searchForward->setIcon(icon14);
        searchForward->setIconSize(QSize(19, 19));
        searchForward->setCheckable(false);
        searchForward->setToolButtonStyle(Qt::ToolButtonIconOnly);
        searchForward->setAutoRaise(true);

        horizontalLayout_3->addWidget(searchForward);

        horizontalSpacer = new QSpacerItem(20, 20, QSizePolicy::Maximum, QSizePolicy::Minimum);

        horizontalLayout_3->addItem(horizontalSpacer);

        zoomoutButton = new QToolButton(dockWidgetContents);
        zoomoutButton->setObjectName(QString::fromUtf8("zoomoutButton"));
        zoomoutButton->setIcon(icon4);
        zoomoutButton->setIconSize(QSize(19, 19));
        zoomoutButton->setCheckable(false);
        zoomoutButton->setToolButtonStyle(Qt::ToolButtonIconOnly);
        zoomoutButton->setAutoRaise(true);

        horizontalLayout_3->addWidget(zoomoutButton);

        zoominButton = new QToolButton(dockWidgetContents);
        zoominButton->setObjectName(QString::fromUtf8("zoominButton"));
        zoominButton->setIcon(icon3);
        zoominButton->setIconSize(QSize(19, 19));
        zoominButton->setCheckable(false);
        zoominButton->setToolButtonStyle(Qt::ToolButtonIconOnly);
        zoominButton->setAutoRaise(true);

        horizontalLayout_3->addWidget(zoominButton);

        topButton = new QToolButton(dockWidgetContents);
        topButton->setObjectName(QString::fromUtf8("topButton"));
        topButton->setIcon(icon5);
        topButton->setIconSize(QSize(19, 19));
        topButton->setCheckable(false);
        topButton->setToolButtonStyle(Qt::ToolButtonIconOnly);
        topButton->setAutoRaise(true);

        horizontalLayout_3->addWidget(topButton);


        verticalLayout_4->addLayout(horizontalLayout_3);

        searchDockWidget->setWidget(dockWidgetContents);
        MainWindowClass->addDockWidget(static_cast<Qt::DockWidgetArea>(4), searchDockWidget);
        treeDockWidget = new QDockWidget(MainWindowClass);
        treeDockWidget->setObjectName(QString::fromUtf8("treeDockWidget"));
        treeDockWidget->setFloating(false);
        treeDockWidget->setFeatures(QDockWidget::NoDockWidgetFeatures);
        treeDockWidget->setAllowedAreas(Qt::LeftDockWidgetArea);
        dockWidgetContents_2 = new QWidget();
        dockWidgetContents_2->setObjectName(QString::fromUtf8("dockWidgetContents_2"));
        dockWidgetContents_2->setBaseSize(QSize(200, 0));
        dockWidgetContents_2->setLayoutDirection(Qt::LeftToRight);
        verticalLayout_5 = new QVBoxLayout(dockWidgetContents_2);
        verticalLayout_5->setSpacing(6);
        verticalLayout_5->setMargin(11);
        verticalLayout_5->setObjectName(QString::fromUtf8("verticalLayout_5"));
        verticalLayout_5->setContentsMargins(0, 2, 5, 8);
        verticalLayout_9 = new QVBoxLayout();
        verticalLayout_9->setSpacing(6);
        verticalLayout_9->setObjectName(QString::fromUtf8("verticalLayout_9"));
        verticalLayout_9->setContentsMargins(0, -1, -1, -1);
        treeTab = new myTreeTab(dockWidgetContents_2);
        treeTab->setObjectName(QString::fromUtf8("treeTab"));
        treeTab->setBaseSize(QSize(200, 0));
        tab = new QWidget();
        tab->setObjectName(QString::fromUtf8("tab"));
        horizontalLayout_4 = new QHBoxLayout(tab);
        horizontalLayout_4->setSpacing(6);
        horizontalLayout_4->setMargin(11);
        horizontalLayout_4->setObjectName(QString::fromUtf8("horizontalLayout_4"));
        horizontalLayout_4->setContentsMargins(0, 3, 0, 0);
        verticalLayout = new QVBoxLayout();
        verticalLayout->setSpacing(6);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        verticalLayout->setContentsMargins(-1, -1, 0, -1);
        horizontalLayout_2 = new QHBoxLayout();
        horizontalLayout_2->setSpacing(15);
        horizontalLayout_2->setObjectName(QString::fromUtf8("horizontalLayout_2"));
        addAllToSearchButton = new QToolButton(tab);
        addAllToSearchButton->setObjectName(QString::fromUtf8("addAllToSearchButton"));
        addAllToSearchButton->setIcon(icon18);

        horizontalLayout_2->addWidget(addAllToSearchButton);

        removeAllFromSearchButton = new QToolButton(tab);
        removeAllFromSearchButton->setObjectName(QString::fromUtf8("removeAllFromSearchButton"));
        removeAllFromSearchButton->setIcon(icon19);

        horizontalLayout_2->addWidget(removeAllFromSearchButton);

        horizontalSpacer_4 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_2->addItem(horizontalSpacer_4);


        verticalLayout->addLayout(horizontalLayout_2);

        treeWidget = new QTreeWidget(tab);
        QTreeWidgetItem *__qtreewidgetitem = new QTreeWidgetItem();
        __qtreewidgetitem->setText(0, QString::fromUtf8("1"));
        treeWidget->setHeaderItem(__qtreewidgetitem);
        treeWidget->setObjectName(QString::fromUtf8("treeWidget"));
        QSizePolicy sizePolicy2(QSizePolicy::Expanding, QSizePolicy::Expanding);
        sizePolicy2.setHorizontalStretch(0);
        sizePolicy2.setVerticalStretch(0);
        sizePolicy2.setHeightForWidth(treeWidget->sizePolicy().hasHeightForWidth());
        treeWidget->setSizePolicy(sizePolicy2);
        treeWidget->setContextMenuPolicy(Qt::CustomContextMenu);
        treeWidget->setLayoutDirection(Qt::LeftToRight);
        treeWidget->setAutoFillBackground(false);
        treeWidget->setFrameShape(QFrame::StyledPanel);
        treeWidget->setFrameShadow(QFrame::Plain);
        treeWidget->setHorizontalScrollBarPolicy(Qt::ScrollBarAsNeeded);
        treeWidget->setAlternatingRowColors(false);
        treeWidget->setRootIsDecorated(true);
        treeWidget->setAllColumnsShowFocus(false);
        treeWidget->setColumnCount(1);
        treeWidget->header()->setVisible(false);
        treeWidget->header()->setStretchLastSection(false);

        verticalLayout->addWidget(treeWidget);


        horizontalLayout_4->addLayout(verticalLayout);

        treeTab->addTab(tab, icon20, QString());
        tab_2 = new QWidget();
        tab_2->setObjectName(QString::fromUtf8("tab_2"));
        verticalLayout_3 = new QVBoxLayout(tab_2);
        verticalLayout_3->setSpacing(6);
        verticalLayout_3->setMargin(11);
        verticalLayout_3->setObjectName(QString::fromUtf8("verticalLayout_3"));
        horizontalLayout_5 = new QHBoxLayout();
        horizontalLayout_5->setSpacing(6);
        horizontalLayout_5->setObjectName(QString::fromUtf8("horizontalLayout_5"));
        horizontalLayout_5->setContentsMargins(-1, 0, -1, -1);
        removeBookmark = new QToolButton(tab_2);
        removeBookmark->setObjectName(QString::fromUtf8("removeBookmark"));
        removeBookmark->setIcon(icon1);
        removeBookmark->setIconSize(QSize(19, 19));
        removeBookmark->setCheckable(false);
        removeBookmark->setToolButtonStyle(Qt::ToolButtonIconOnly);
        removeBookmark->setAutoRaise(true);

        horizontalLayout_5->addWidget(removeBookmark);

        bookmarkUp = new QToolButton(tab_2);
        bookmarkUp->setObjectName(QString::fromUtf8("bookmarkUp"));
        bookmarkUp->setIcon(icon5);
        bookmarkUp->setIconSize(QSize(19, 19));
        bookmarkUp->setCheckable(false);
        bookmarkUp->setToolButtonStyle(Qt::ToolButtonIconOnly);
        bookmarkUp->setAutoRaise(true);

        horizontalLayout_5->addWidget(bookmarkUp);

        bookmarkDown = new QToolButton(tab_2);
        bookmarkDown->setObjectName(QString::fromUtf8("bookmarkDown"));
        QIcon icon21;
        icon21.addFile(QString::fromUtf8(":/Icons/arrow-down.png"), QSize(), QIcon::Normal, QIcon::Off);
        bookmarkDown->setIcon(icon21);
        bookmarkDown->setIconSize(QSize(19, 19));
        bookmarkDown->setCheckable(false);
        bookmarkDown->setToolButtonStyle(Qt::ToolButtonIconOnly);
        bookmarkDown->setAutoRaise(true);

        horizontalLayout_5->addWidget(bookmarkDown);

        bookmarkEdit = new QToolButton(tab_2);
        bookmarkEdit->setObjectName(QString::fromUtf8("bookmarkEdit"));
        bookmarkEdit->setIcon(icon10);
        bookmarkEdit->setIconSize(QSize(19, 19));
        bookmarkEdit->setCheckable(false);
        bookmarkEdit->setToolButtonStyle(Qt::ToolButtonIconOnly);
        bookmarkEdit->setAutoRaise(true);

        horizontalLayout_5->addWidget(bookmarkEdit);

        horizontalSpacer_3 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_5->addItem(horizontalSpacer_3);


        verticalLayout_3->addLayout(horizontalLayout_5);

        bookmarkWidget = new QListWidget(tab_2);
        bookmarkWidget->setObjectName(QString::fromUtf8("bookmarkWidget"));
        bookmarkWidget->setContextMenuPolicy(Qt::CustomContextMenu);
        bookmarkWidget->setFrameShape(QFrame::StyledPanel);
        bookmarkWidget->setLineWidth(1);
        bookmarkWidget->setHorizontalScrollBarPolicy(Qt::ScrollBarAsNeeded);
        bookmarkWidget->setAutoScroll(true);
        bookmarkWidget->setAutoScrollMargin(0);
        bookmarkWidget->setUniformItemSizes(false);

        verticalLayout_3->addWidget(bookmarkWidget);

        treeTab->addTab(tab_2, icon9, QString());
        tab_3 = new QWidget();
        tab_3->setObjectName(QString::fromUtf8("tab_3"));
        verticalLayout_6 = new QVBoxLayout(tab_3);
        verticalLayout_6->setSpacing(6);
        verticalLayout_6->setMargin(0);
        verticalLayout_6->setObjectName(QString::fromUtf8("verticalLayout_6"));
        scrollArea = new QScrollArea(tab_3);
        scrollArea->setObjectName(QString::fromUtf8("scrollArea"));
        scrollArea->setLayoutDirection(Qt::LeftToRight);
        scrollArea->setWidgetResizable(true);
        scrollAreaWidgetContents = new QWidget();
        scrollAreaWidgetContents->setObjectName(QString::fromUtf8("scrollAreaWidgetContents"));
        scrollAreaWidgetContents->setGeometry(QRect(0, 0, 222, 477));
        verticalLayout_7 = new QVBoxLayout(scrollAreaWidgetContents);
        verticalLayout_7->setSpacing(-1);
        verticalLayout_7->setMargin(11);
        verticalLayout_7->setObjectName(QString::fromUtf8("verticalLayout_7"));
        verticalLayout_7->setContentsMargins(-1, 24, -1, -1);
        label_9 = new QLabel(scrollAreaWidgetContents);
        label_9->setObjectName(QString::fromUtf8("label_9"));

        verticalLayout_7->addWidget(label_9);

        fontComboBox = new QFontComboBox(scrollAreaWidgetContents);
        fontComboBox->setObjectName(QString::fromUtf8("fontComboBox"));
        fontComboBox->setLayoutDirection(Qt::LeftToRight);
        fontComboBox->setCurrentIndex(124);
        fontComboBox->setSizeAdjustPolicy(QComboBox::AdjustToContentsOnFirstShow);
        fontComboBox->setWritingSystem(QFontDatabase::Any);
        QFont font;
        font.setFamily(QString::fromUtf8("Nachlieli CLM"));
        fontComboBox->setCurrentFont(font);

        verticalLayout_7->addWidget(fontComboBox);

        label_4 = new QLabel(scrollAreaWidgetContents);
        label_4->setObjectName(QString::fromUtf8("label_4"));

        verticalLayout_7->addWidget(label_4);

        fonSizeSpinBox = new QSpinBox(scrollAreaWidgetContents);
        fonSizeSpinBox->setObjectName(QString::fromUtf8("fonSizeSpinBox"));

        verticalLayout_7->addWidget(fonSizeSpinBox);

        line_2 = new QFrame(scrollAreaWidgetContents);
        line_2->setObjectName(QString::fromUtf8("line_2"));
        line_2->setFrameShape(QFrame::HLine);
        line_2->setFrameShadow(QFrame::Sunken);

        verticalLayout_7->addWidget(line_2);

        label_6 = new QLabel(scrollAreaWidgetContents);
        label_6->setObjectName(QString::fromUtf8("label_6"));

        verticalLayout_7->addWidget(label_6);

        label_3 = new QLabel(scrollAreaWidgetContents);
        label_3->setObjectName(QString::fromUtf8("label_3"));
        label_3->setEnabled(false);
        QFont font1;
        font1.setPointSize(7);
        label_3->setFont(font1);
        label_3->setWordWrap(true);

        verticalLayout_7->addWidget(label_3);

        fontPreview = new QLineEdit(scrollAreaWidgetContents);
        fontPreview->setObjectName(QString::fromUtf8("fontPreview"));
        fontPreview->setEnabled(false);
        QFont font2;
        font2.setFamily(QString::fromUtf8("Nachlieli CLM"));
        font2.setPointSize(16);
        fontPreview->setFont(font2);
        fontPreview->setLayoutDirection(Qt::RightToLeft);

        verticalLayout_7->addWidget(fontPreview);

        line_3 = new QFrame(scrollAreaWidgetContents);
        line_3->setObjectName(QString::fromUtf8("line_3"));
        line_3->setFrameShape(QFrame::HLine);
        line_3->setFrameShadow(QFrame::Sunken);

        verticalLayout_7->addWidget(line_3);

        horizontalLayout_6 = new QHBoxLayout();
        horizontalLayout_6->setSpacing(6);
        horizontalLayout_6->setObjectName(QString::fromUtf8("horizontalLayout_6"));
        horizontalLayout_6->setContentsMargins(-1, 0, -1, -1);
        horizontalSpacer_2 = new QSpacerItem(50, 20, QSizePolicy::Maximum, QSizePolicy::Minimum);

        horizontalLayout_6->addItem(horizontalSpacer_2);

        saveConf = new QPushButton(scrollAreaWidgetContents);
        saveConf->setObjectName(QString::fromUtf8("saveConf"));
        saveConf->setEnabled(false);
        saveConf->setLayoutDirection(Qt::RightToLeft);
        QIcon icon22;
        icon22.addFile(QString::fromUtf8(":/Icons/configure.png"), QSize(), QIcon::Normal, QIcon::Off);
        saveConf->setIcon(icon22);

        horizontalLayout_6->addWidget(saveConf);

        horizontalSpacer_5 = new QSpacerItem(50, 20, QSizePolicy::Maximum, QSizePolicy::Minimum);

        horizontalLayout_6->addItem(horizontalSpacer_5);


        verticalLayout_7->addLayout(horizontalLayout_6);

        label_5 = new QLabel(scrollAreaWidgetContents);
        label_5->setObjectName(QString::fromUtf8("label_5"));
        label_5->setEnabled(false);
        QFont font3;
        font3.setPointSize(8);
        label_5->setFont(font3);

        verticalLayout_7->addWidget(label_5);

        verticalSpacer = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout_7->addItem(verticalSpacer);

        label_7 = new QLabel(scrollAreaWidgetContents);
        label_7->setObjectName(QString::fromUtf8("label_7"));

        verticalLayout_7->addWidget(label_7);

        langComboBox = new QComboBox(scrollAreaWidgetContents);
        langComboBox->setObjectName(QString::fromUtf8("langComboBox"));

        verticalLayout_7->addWidget(langComboBox);

        label_8 = new QLabel(scrollAreaWidgetContents);
        label_8->setObjectName(QString::fromUtf8("label_8"));
        label_8->setEnabled(false);
        label_8->setFont(font3);
        label_8->setScaledContents(false);
        label_8->setWordWrap(true);

        verticalLayout_7->addWidget(label_8);

        horizontalLayout_7 = new QHBoxLayout();
        horizontalLayout_7->setSpacing(6);
        horizontalLayout_7->setObjectName(QString::fromUtf8("horizontalLayout_7"));
        horizontalLayout_7->setContentsMargins(-1, 0, -1, -1);
        horizontalSpacer_6 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_7->addItem(horizontalSpacer_6);

        changeLangButton = new QPushButton(scrollAreaWidgetContents);
        changeLangButton->setObjectName(QString::fromUtf8("changeLangButton"));
        changeLangButton->setFont(font3);

        horizontalLayout_7->addWidget(changeLangButton);

        horizontalSpacer_7 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_7->addItem(horizontalSpacer_7);


        verticalLayout_7->addLayout(horizontalLayout_7);

        scrollArea->setWidget(scrollAreaWidgetContents);

        verticalLayout_6->addWidget(scrollArea);

        treeTab->addTab(tab_3, icon22, QString());

        verticalLayout_9->addWidget(treeTab);

        mixedGroup = new QFrame(dockWidgetContents_2);
        mixedGroup->setObjectName(QString::fromUtf8("mixedGroup"));
        mixedGroup->setFont(font3);
        mixedGroup->setFrameShape(QFrame::NoFrame);
        mixedGroup->setFrameShadow(QFrame::Plain);
        verticalLayout_8 = new QVBoxLayout(mixedGroup);
        verticalLayout_8->setSpacing(6);
        verticalLayout_8->setMargin(11);
        verticalLayout_8->setObjectName(QString::fromUtf8("verticalLayout_8"));
        verticalLayout_8->setContentsMargins(4, 4, 4, 6);
        mixedFrame = new QGroupBox(mixedGroup);
        mixedFrame->setObjectName(QString::fromUtf8("mixedFrame"));
        verticalLayout_10 = new QVBoxLayout(mixedFrame);
        verticalLayout_10->setSpacing(6);
        verticalLayout_10->setMargin(0);
        verticalLayout_10->setObjectName(QString::fromUtf8("verticalLayout_10"));
        verticalLayout_11 = new QVBoxLayout();
        verticalLayout_11->setSpacing(6);
        verticalLayout_11->setObjectName(QString::fromUtf8("verticalLayout_11"));
        verticalLayout_11->setContentsMargins(-1, 14, -1, -1);
        horizontalLayout_8 = new QHBoxLayout();
        horizontalLayout_8->setSpacing(6);
        horizontalLayout_8->setObjectName(QString::fromUtf8("horizontalLayout_8"));
        horizontalSpacer_8 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_8->addItem(horizontalSpacer_8);

        openMixed = new QPushButton(mixedFrame);
        openMixed->setObjectName(QString::fromUtf8("openMixed"));

        horizontalLayout_8->addWidget(openMixed);

        horizontalSpacer_9 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_8->addItem(horizontalSpacer_9);


        verticalLayout_11->addLayout(horizontalLayout_8);


        verticalLayout_10->addLayout(verticalLayout_11);


        verticalLayout_8->addWidget(mixedFrame);


        verticalLayout_9->addWidget(mixedGroup);

        progressBar = new QProgressBar(dockWidgetContents_2);
        progressBar->setObjectName(QString::fromUtf8("progressBar"));
        progressBar->setMaximumSize(QSize(16777215, 16));
        progressBar->setBaseSize(QSize(200, 0));
        progressBar->setValue(21);
        progressBar->setTextVisible(false);

        verticalLayout_9->addWidget(progressBar);


        verticalLayout_5->addLayout(verticalLayout_9);

        treeDockWidget->setWidget(dockWidgetContents_2);
        MainWindowClass->addDockWidget(static_cast<Qt::DockWidgetArea>(1), treeDockWidget);
        menuBar = new QMenuBar(MainWindowClass);
        menuBar->setObjectName(QString::fromUtf8("menuBar"));
        menuBar->setGeometry(QRect(0, 0, 1097, 28));
        menuBar->setLayoutDirection(Qt::LeftToRight);
        menu = new QMenu(menuBar);
        menu->setObjectName(QString::fromUtf8("menu"));
        menu->setLayoutDirection(Qt::LeftToRight);
        menu_2 = new QMenu(menuBar);
        menu_2->setObjectName(QString::fromUtf8("menu_2"));
        menu_2->setLayoutDirection(Qt::LeftToRight);
        menu_3 = new QMenu(menuBar);
        menu_3->setObjectName(QString::fromUtf8("menu_3"));
        menu_3->setLayoutDirection(Qt::LeftToRight);
        menu_5 = new QMenu(menuBar);
        menu_5->setObjectName(QString::fromUtf8("menu_5"));
        menu_5->setLayoutDirection(Qt::LeftToRight);
        menu_4 = new QMenu(menuBar);
        menu_4->setObjectName(QString::fromUtf8("menu_4"));
        menu_4->setLayoutDirection(Qt::LeftToRight);
        MainWindowClass->setMenuBar(menuBar);

        menuBar->addAction(menu->menuAction());
        menuBar->addAction(menu_3->menuAction());
        menuBar->addAction(menu_4->menuAction());
        menuBar->addAction(menu_5->menuAction());
        menuBar->addAction(menu_2->menuAction());
        menu->addAction(opentabAction);
        menu->addAction(closetabAction);
        menu->addAction(printAction);
        menu->addSeparator();
        menu->addAction(exitAction);
        menu_2->addAction(aboutAction);
        menu_3->addAction(jumptotopAction);
        menu_3->addAction(zoominAction);
        menu_3->addAction(zoomoutAction);
        menu_3->addAction(showNikudAction);
        menu_3->addAction(showTeamimAction);
        menu_5->addAction(findBookAction);
        menu_5->addSeparator();
        menu_5->addAction(advancedSearchAction);
        menu_5->addAction(addToSearchAction);
        menu_5->addAction(addAllToSearchAction);
        menu_5->addAction(removeFromSearchAction);
        menu_5->addAction(removeAllFromSearchAction);
        menu_5->addSeparator();
        menu_5->addAction(searchForwardAction);
        menu_5->addAction(searchInBooksAction);
        menu_5->addAction(searchBackwardsAction);
        menu_5->addSeparator();
        menu_4->addAction(bookMarkAction);
        menu_4->addAction(commentAction);
        menu_4->addAction(reportErrorAction);

        retranslateUi(MainWindowClass);
        QObject::connect(exitAction, SIGNAL(triggered()), MainWindowClass, SLOT(close()));

        viewTab->setCurrentIndex(0);
        treeTab->setCurrentIndex(0);
        bookmarkWidget->setCurrentRow(-1);


        QMetaObject::connectSlotsByName(MainWindowClass);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindowClass)
    {
        MainWindowClass->setWindowTitle(QApplication::translate("MainWindowClass", "Orayta - Hebrew books", 0, QApplication::UnicodeUTF8));
        exitAction->setText(QApplication::translate("MainWindowClass", "Exit", 0, QApplication::UnicodeUTF8));
        aboutAction->setText(QApplication::translate("MainWindowClass", "About", 0, QApplication::UnicodeUTF8));
        zoominAction->setText(QApplication::translate("MainWindowClass", "Zoom in", 0, QApplication::UnicodeUTF8));
        zoomoutAction->setText(QApplication::translate("MainWindowClass", "Zoom out", 0, QApplication::UnicodeUTF8));
        jumptotopAction->setText(QApplication::translate("MainWindowClass", "Jump to top", 0, QApplication::UnicodeUTF8));
        printAction->setText(QApplication::translate("MainWindowClass", "Print book", 0, QApplication::UnicodeUTF8));
        closetabAction->setText(QApplication::translate("MainWindowClass", "Close tab", 0, QApplication::UnicodeUTF8));
        opentabAction->setText(QApplication::translate("MainWindowClass", "Open new tab", 0, QApplication::UnicodeUTF8));
        showNikudAction->setText(QApplication::translate("MainWindowClass", "Show nikud", 0, QApplication::UnicodeUTF8));
        showTeamimAction->setText(QApplication::translate("MainWindowClass", "Show teamim", 0, QApplication::UnicodeUTF8));
        bookMarkAction->setText(QApplication::translate("MainWindowClass", "Add bookmark", 0, QApplication::UnicodeUTF8));
        commentAction->setText(QApplication::translate("MainWindowClass", "Add/edit comment", 0, QApplication::UnicodeUTF8));
        reportErrorAction->setText(QApplication::translate("MainWindowClass", "Report typo", 0, QApplication::UnicodeUTF8));
        advancedSearchAction->setText(QApplication::translate("MainWindowClass", "Advanced search", 0, QApplication::UnicodeUTF8));
        searchInBooksAction->setText(QApplication::translate("MainWindowClass", "Search in book", 0, QApplication::UnicodeUTF8));
        searchForwardAction->setText(QApplication::translate("MainWindowClass", "Search forward", 0, QApplication::UnicodeUTF8));
        searchBackwardsAction->setText(QApplication::translate("MainWindowClass", "Search backwards", 0, QApplication::UnicodeUTF8));
        removeFromSearchAction->setText(QApplication::translate("MainWindowClass", "Remove book from search", 0, QApplication::UnicodeUTF8));
        addToSearchAction->setText(QApplication::translate("MainWindowClass", "Add book to search", 0, QApplication::UnicodeUTF8));
        addAllToSearchAction->setText(QApplication::translate("MainWindowClass", "Add all to search", 0, QApplication::UnicodeUTF8));
        removeAllFromSearchAction->setText(QApplication::translate("MainWindowClass", "Remove all from search", 0, QApplication::UnicodeUTF8));
        findBookAction->setText(QApplication::translate("MainWindowClass", "Find book by name", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_TOOLTIP
        newTabButton->setToolTip(QApplication::translate("MainWindowClass", "Open new tab", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
        newTabButton->setText(QString());
        viewTab->setTabText(viewTab->indexOf(tab1), QApplication::translate("MainWindowClass", "Orayta", 0, QApplication::UnicodeUTF8));
        label->setText(QApplication::translate("MainWindowClass", "Search:", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_TOOLTIP
        searchButton->setToolTip(QApplication::translate("MainWindowClass", "Search in selected books", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
        searchButton->setText(QString());
#ifndef QT_NO_TOOLTIP
        searchBackwords->setToolTip(QApplication::translate("MainWindowClass", "Search backwards in book", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
        searchBackwords->setText(QString());
#ifndef QT_NO_TOOLTIP
        searchForward->setToolTip(QApplication::translate("MainWindowClass", "Search forward in book", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
        searchForward->setText(QString());
#ifndef QT_NO_TOOLTIP
        zoomoutButton->setToolTip(QApplication::translate("MainWindowClass", "Zoom out", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
        zoomoutButton->setText(QString());
#ifndef QT_NO_TOOLTIP
        zoominButton->setToolTip(QApplication::translate("MainWindowClass", "Zoom in", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
        zoominButton->setText(QString());
#ifndef QT_NO_TOOLTIP
        topButton->setToolTip(QApplication::translate("MainWindowClass", "Jump to top", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
        topButton->setText(QString());
#ifndef QT_NO_TOOLTIP
        addAllToSearchButton->setToolTip(QApplication::translate("MainWindowClass", "Add all books to search", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
        addAllToSearchButton->setText(QApplication::translate("MainWindowClass", "...", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_TOOLTIP
        removeAllFromSearchButton->setToolTip(QApplication::translate("MainWindowClass", "Remove all books from search", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
        removeAllFromSearchButton->setText(QApplication::translate("MainWindowClass", "...", 0, QApplication::UnicodeUTF8));
        treeTab->setTabText(treeTab->indexOf(tab), QApplication::translate("MainWindowClass", "Books", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_TOOLTIP
        removeBookmark->setToolTip(QApplication::translate("MainWindowClass", "Remove bookmark", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
        removeBookmark->setText(QApplication::translate("MainWindowClass", "Erase bookmark", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_TOOLTIP
        bookmarkUp->setToolTip(QApplication::translate("MainWindowClass", "Move bookmark up", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
        bookmarkUp->setText(QString());
#ifndef QT_NO_TOOLTIP
        bookmarkDown->setToolTip(QApplication::translate("MainWindowClass", "Move bookmark down", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
        bookmarkDown->setText(QString());
#ifndef QT_NO_TOOLTIP
        bookmarkEdit->setToolTip(QApplication::translate("MainWindowClass", "Edit bookmark's title", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
        bookmarkEdit->setText(QString());
        treeTab->setTabText(treeTab->indexOf(tab_2), QApplication::translate("MainWindowClass", "Bookmarks", 0, QApplication::UnicodeUTF8));
        label_9->setText(QApplication::translate("MainWindowClass", "Default font:", 0, QApplication::UnicodeUTF8));
        fontComboBox->setStyleSheet(QApplication::translate("MainWindowClass", "font: 8pt \"Sans Serif\";", 0, QApplication::UnicodeUTF8));
        label_4->setText(QApplication::translate("MainWindowClass", "Base font size:", 0, QApplication::UnicodeUTF8));
        label_6->setText(QApplication::translate("MainWindowClass", "font preview:", 0, QApplication::UnicodeUTF8));
        label_3->setText(QApplication::translate("MainWindowClass", "(Make sure the font can handle Nikud and Teamim)", 0, QApplication::UnicodeUTF8));
        fontPreview->setText(QApplication::translate("MainWindowClass", "\327\220\326\270\326\243\327\226 \327\231\326\270\327\251\327\201\326\264\326\275\327\231\327\250\326\276\327\236\326\271\327\251\327\201\326\266\327\224\326\251", 0, QApplication::UnicodeUTF8));
        saveConf->setText(QApplication::translate("MainWindowClass", "Save changes", 0, QApplication::UnicodeUTF8));
        label_5->setText(QApplication::translate("MainWindowClass", "(Confs will apply as of next book)", 0, QApplication::UnicodeUTF8));
        label_7->setText(QApplication::translate("MainWindowClass", "Gui language:", 0, QApplication::UnicodeUTF8));
        label_8->setText(QApplication::translate("MainWindowClass", "Changing the language will restart the application, and close all open books", 0, QApplication::UnicodeUTF8));
        changeLangButton->setText(QApplication::translate("MainWindowClass", "Change language", 0, QApplication::UnicodeUTF8));
        treeTab->setTabText(treeTab->indexOf(tab_3), QApplication::translate("MainWindowClass", "Configure", 0, QApplication::UnicodeUTF8));
        mixedFrame->setTitle(QApplication::translate("MainWindowClass", "Display with:", 0, QApplication::UnicodeUTF8));
        openMixed->setText(QApplication::translate("MainWindowClass", "Open", 0, QApplication::UnicodeUTF8));
        menu->setTitle(QApplication::translate("MainWindowClass", "File", 0, QApplication::UnicodeUTF8));
        menu_2->setTitle(QApplication::translate("MainWindowClass", "Help", 0, QApplication::UnicodeUTF8));
        menu_3->setTitle(QApplication::translate("MainWindowClass", "Display", 0, QApplication::UnicodeUTF8));
        menu_5->setTitle(QApplication::translate("MainWindowClass", "Search", 0, QApplication::UnicodeUTF8));
        menu_4->setTitle(QApplication::translate("MainWindowClass", "Location", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class MainWindowClass: public Ui_MainWindowClass {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
