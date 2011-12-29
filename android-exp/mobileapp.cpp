#include "mobileapp.h"
#include "ui_mobileapp.h"

#include "functions.h"
#include "booklist.h"
#include <QMessageBox>
#include <QDebug>
#include <QFile>
#include <QCloseEvent>
#include <QWebFrame>
 #include <QWebPage>

//#include <QScroller>

#define MAIN_PAGE 0
#define ABOUT_PAGE 1
#define DISPLAY_PAGE 2
#define LIST_PAGE 3
#define SEARCH_PAGE 4
#define BOOKMARK_PAGE 5
#define SETTINGS_PAGE 6



//TODO: Get rid of horrible text over text bug
//TODO: Test landscape - portrait switching
//TODO: Improve look & feel
//TODO: Make kinetic scrolling work (webview + booklist)
//TODO: Search
//TODO: Bookmarks
//TODO: Improve book loading speed (gradual loading)
//TODO: Fix 4 buttons alignment
//TODO: imply weaved view.

//IZAR-
//TODO: load page preview




#include <QKinetic/qtscroller.h>




MobileApp::MobileApp(QWidget *parent) :QDialog(parent), ui(new Ui::MobileApp)
{
    ui->setupUi(this);

    ui->stackedWidget->setCurrentIndex(ABOUT_PAGE);

    //IZAR- changed this to use dejavu sans free font in all versions (linux, windows and android)
    //QString gFontFamily = "Nachlieli CLM";
    gFontFamily = "Droid Sans Hebrew";
    gFontSize = 20;

    wview = new myWebView(this);

    connect (wview, SIGNAL(loadFinished(bool)), this, SLOT(on_wview_loadFinished(bool)));

    ui->displaypage->layout()->addWidget(wview);

    //QString noselect = "body {-webkit-user-select: none;}";
    //writetofile(TMPPATH + "/noselect.css", noselect, "UTF-8");
    QUrl q = QUrl::fromLocalFile(TMPPATH + "/noselect.css");
    //wview->page()->settings()->setUserStyleSheetUrl(q);
    wview->page()->setContentEditable(false);

    QtScroller::grabGesture(wview, QtScroller::LeftMouseButtonGesture);

    wview->show();

    wview->setHtml("loading page");

    QtScroller::grabGesture(ui->treeWidget);




    ui->treeWidget->setColumnWidth(0,800);

    bookList.BuildFromFolder(BOOKPATH);

    if (bookList.empty())
    {
        qDebug()<< "bookpath: " << BOOKPATH;

        QMessageBox msgBox;
        msgBox.setText(tr("No books found! \nCheck your installation, or contact the developer."));
        msgBox.exec();

        //No books found
        exit(2);
    }

    // Check all uids
    bookList.CheckUid();

    bookList.displayInTree(ui->treeWidget, false);

    //IZAR
    //TODO - use QMessageBox::aboutQt ?
    QString htmlLabl;

    htmlLabl += "Orayta 4 android: Version 1.1\n";
    htmlLabl += "Compiled with qt-version:"  ; htmlLabl +=QT_VERSION_STR  ; htmlLabl +="\n";
    htmlLabl += "Runtime qt-version:"  ; htmlLabl +=qVersion()  ; htmlLabl +="\n";
    htmlLabl += "Compiled qt-webkit-version:"  ; htmlLabl +=QTWEBKIT_VERSION_STR  ;


     ui->label->setText(htmlLabl);


    QApplication::processEvents();
    ui->stackedWidget->setCurrentIndex(MAIN_PAGE);
}

MobileApp::~MobileApp()
{
    delete ui;
}

void MobileApp::on_openBTN_clicked()
{
    ui->stackedWidget->setCurrentIndex(LIST_PAGE);
}

void MobileApp::on_searchBTN_clicked()
{
    ui->stackedWidget->setCurrentIndex(SEARCH_PAGE);
}



void MobileApp::on_bookmarksBTN_clicked()
{
    ui->stackedWidget->setCurrentIndex(BOOKMARK_PAGE);
}

void MobileApp::on_aboutBTN_clicked()
{
    ui->stackedWidget->setCurrentIndex(ABOUT_PAGE);

}

void MobileApp::on_treeWidget_clicked(const QModelIndex &index)
{
    if (ui->treeWidget->isExpanded(index)) ui->treeWidget->collapse(index);
    else ui->treeWidget->expand(index);
}


void MobileApp::on_openMixed_clicked()
{
    if ( ui->treeWidget->currentItem() == 0)
        return;
    Book *b = bookList.findBookByTWI(ui->treeWidget->currentItem());
    if (!b->IsDir()) showBook(b);
}


void MobileApp::on_treeWidget_itemDoubleClicked(QTreeWidgetItem *item, int column)
{
    Book *b = bookList.findBookByTWI(item);

    if (!b->IsDir()) showBook(b);
}


void MobileApp::showBook(Book *book)
{
    if (book == 0)
        return;
    ui->stackedWidget->setCurrentIndex(DISPLAY_PAGE);
    ui->titlelbl->setText("Loading...");

    //IZAR: temporary work-around. the problem is that orayta reads the global font settings ONLY on startup, and is careless if it is changed latter.
    //TODO: fix this.
    QFont font( gFontFamily, gFontSize );
    book->setFont(font);


    //ui->stackedWidget->setCurrentIndex(DISPLAY_PAGE);
    //QApplication::processEvents();

    //Generate filename representing this file, the commentreis that should open, and it's nikud (or teamim) mode
    //  This way the file is rendered again only if it needs to be shown differently (if other commenteries were requested)
    QString htmlfilename = book->HTMLFileName() + ".html";

    qDebug() << "rendering book; font: " << gFontFamily;

    //Check if file already exists. If not, make sure it renders ok.
    QFile f(htmlfilename);
    bool renderedOK = true;

    if (!f.exists())
    {
        renderedOK = book->htmlrender(htmlfilename, true, true, "");
    }

    if (renderedOK == true)
    {
        QString p =  absPath(htmlfilename);
        QUrl u = QUrl::fromLocalFile(p);

        ui->stackedWidget->setCurrentIndex(DISPLAY_PAGE);
        wview->load(u);

        booktitle = book->getNormallDisplayName();
    }
}

void MobileApp::on_toolButton_clicked()
{
    if (ui->stackedWidget->currentIndex() == DISPLAY_PAGE) ui->stackedWidget->setCurrentIndex(LIST_PAGE);
    else if (ui->stackedWidget->currentIndex() == MAIN_PAGE) exit(0);
    else ui->stackedWidget->setCurrentIndex(MAIN_PAGE);
}


//Catch android "back" button
void MobileApp::closeEvent(QCloseEvent *event)
{
    qDebug() << "Oh no!!!";

    if (ui->stackedWidget->currentIndex() == DISPLAY_PAGE)
    {
        ui->stackedWidget->setCurrentIndex(LIST_PAGE);
        event->ignore();
    }
    else if (ui->stackedWidget->currentIndex() == MAIN_PAGE)
    {
        event->accept();
        exit(0);
    }
    else
    {
        ui->stackedWidget->setCurrentIndex(MAIN_PAGE);
        event->ignore();
    }
}


void MobileApp::on_wview_loadFinished(bool arg1)
{
   ui->titlelbl->setText(booktitle);
}

void MobileApp::on_toolButton_3_clicked()
{
    wview->setZoomFactor(wview->zoomFactor() + 0.1);
}

void MobileApp::on_toolButton_2_clicked()
{
    if (wview->zoomFactor() > 0.3) wview->setZoomFactor(wview->zoomFactor() - 0.1);
}

void MobileApp::on_toolButton_6_clicked()
{
    wview->page()->mainFrame()->scrollToAnchor("Top");
}

void MobileApp::on_title_clicked()
{
    on_toolButton_clicked();
}

void MobileApp::on_settings_BTN_clicked()
{
     ui->stackedWidget->setCurrentIndex(SETTINGS_PAGE);
}

void MobileApp::on_saveConf_clicked()
{
    //Save font
    gFontFamily = ui->fontComboBox->currentFont().family();
    gFontSize = ui->fonSizeSpinBox->value();

    //IZAR: testing font problem
    qDebug() << "font set to: " << gFontFamily;

    ui->saveConf->setEnabled(false);

    //Change language if needed
    QSettings settings("Orayta", "SingleUser");
    settings.beginGroup("Confs");

    settings.setValue("systemLang",ui->systemLangCbox->isChecked());
    if (ui->systemLangCbox->isChecked())
    {
        LANG = QLocale::languageToString(QLocale::system().language());
    }
    //Use custom language only if "useSystemLang" is not checked
    else
    {
//        int i = langsDisplay.indexOf(ui->langComboBox->currentText());
//        if (i != -1)
//        {
//            settings.setValue("lang", langs[i]);
//            settings.endGroup();
//            LANG = langs[i];
//        }
    }

//    emit ChangeLang(LANG);
    ui->stackedWidget->setCurrentIndex(MAIN_PAGE);
}

void MobileApp::on_fontComboBox_currentIndexChanged(const QString &font)
{
    //Settings have changed, so the save button should be enabled
    ui->saveConf->setEnabled(true);

    //Show the new font in the preview box
    ui->fontPreview->setFont(QFont(font, ui->fonSizeSpinBox->value()));
}

void MobileApp::on_fonSizeSpinBox_valueChanged(int size)
{
    //Settings have changed, so the save button should be enabled
    ui->saveConf->setEnabled(true);

    //Show the new font in the preview box
    ui->fontPreview->setFont(QFont(ui->fontComboBox->currentFont().family(), size));
}

void MobileApp::on_cancelBTN_clicked()
{
    ui->stackedWidget->setCurrentIndex(MAIN_PAGE);
}
