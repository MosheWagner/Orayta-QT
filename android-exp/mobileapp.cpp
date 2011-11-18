#include "mobileapp.h"
#include "ui_mobileapp.h"

#include "functions.h"
#include "booklist.h"
#include <QMessageBox>
#include <QDebug>
#include <QFile>

#define MAIN_PAGE 0
#define ABOUT_PAGE 1
#define DISPLAY_PAGE 2
#define LIST_PAGE 3
#define SEARCH_PAGE 4
#define BOOKMARK_PAGE 5

MobileApp::MobileApp(QWidget *parent) :QDialog(parent), ui(new Ui::MobileApp)
{
    ui->setupUi(this);


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
    ui->treeWidget->expand(index);
}


void MobileApp::on_openMixed_clicked()
{
    Book *b = bookList.findBookByTWI(ui->treeWidget->currentItem());
    showBook(b);
}


void MobileApp::on_treeWidget_itemDoubleClicked(QTreeWidgetItem *item, int column)
{
    Book *b = bookList.findBookByTWI(item);
    showBook(b);
}


void MobileApp::showBook(Book *book)
{
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

        ui->stackedWidget->setCurrentIndex(DISPLAY_PAGE);
        ui->webView->load(QUrl(p));
    }
}

void MobileApp::on_toolButton_clicked()
{
    if (ui->stackedWidget->currentIndex() == DISPLAY_PAGE) ui->stackedWidget->setCurrentIndex(LIST_PAGE);
    else ui->stackedWidget->setCurrentIndex(MAIN_PAGE);
}
