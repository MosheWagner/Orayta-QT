#include "mobileapp.h"
#include "ui_mobileapp.h"

#include "functions.h"
#include "booklist.h"
#include <QMessageBox>
#include <QDebug>
#include <QFile>
#include <QCloseEvent>
#include <QWebFrame>

#define MAIN_PAGE 0
#define ABOUT_PAGE 1
#define DISPLAY_PAGE 2
#define LIST_PAGE 3
#define SEARCH_PAGE 4
#define BOOKMARK_PAGE 5



//TODO: Get rid of horrible text over text bug
//TODO: Test landscape - portrait switching
//TODO: Improve look & feel
//TODO: Make kinetic scrolling work (webview + booklist)
//TODO: Search
//TODO: Bookmarks
//TODO: Improve book loading speed (gradual loading)
//TODO: Fix 4 buttons alignment


#include <QKinetic/qtscroller.h>
//#include <QScroller>

MobileApp::MobileApp(QWidget *parent) :QDialog(parent), ui(new Ui::MobileApp)
{
    ui->setupUi(this);


    wview = new myWebView(this);
    ui->displaypage->layout()->addWidget(wview);

    QString noselect = "body {-webkit-user-select: none;}";
    writetofile(TMPPATH + "/noselect.css", noselect, "UTF-8");
    QUrl q = QUrl::fromLocalFile(TMPPATH + "/noselect.css");
    wview->page()->settings()->setUserStyleSheetUrl(q);
    wview->page()->setContentEditable(false);

    QtScroller::grabGesture(wview, QtScroller::LeftMouseButtonGesture);

    wview->show();



    ui->stackedWidget->setCurrentIndex(MAIN_PAGE);


    ui->treeWidget->setColumnWidth(0,800);

    bookList.BuildFromFolder(BOOKPATH);

    if (bookList.empty())
    {
        QMessageBox msgBox;
        msgBox.setText(tr("No books found! \nCheck your installation, or contact the developer."));
        msgBox.exec();

        //No books found
        exit(2);
    }

    // Check alls uids
    bookList.CheckUid();

    bookList.displayInTree(ui->treeWidget, false);

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
    ui->stackedWidget->setCurrentIndex(DISPLAY_PAGE);
    ui->titlelbl->setText("Loading...");

    //ui->stackedWidget->setCurrentIndex(DISPLAY_PAGE);
    //QApplication::processEvents();

    //Generate filename representing this file, the commentreis that should open, and it's nikud (or teamim) mode
    //  This way the file is rendered again only if it needs to be shown differently (if other commenteries were requested)
    QString htmlfilename = book->HTMLFileName() + ".html";

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
    else ui->stackedWidget->setCurrentIndex(MAIN_PAGE);
}

/*
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
    }
    else
    {
        ui->stackedWidget->setCurrentIndex(MAIN_PAGE);
        event->ignore();
    }
}
*/

void MobileApp::on_webView_loadFinished(bool arg1)
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
