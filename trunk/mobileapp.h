#ifndef MOBILEAPP_H
#define MOBILEAPP_H

#include <QDialog>
#include <QModelIndex>
#include <QTreeWidgetItem>
#include <QListWidgetItem>
#include <QMovie>
#include <QUrl>
#include "filedownloader.h"
#include "booklist.h"
#include "book.h"

#include "textdisplayer.h"


#define MAIN_PAGE 0
#define ABOUT_PAGE 1
#define DISPLAY_PAGE 2
#define LIST_PAGE 3
#define SEARCH_PAGE 4
#define GET_BOOKS_PAGE 5
#define SETTINGS_PAGE 6
#define MIXED_SELECTION_PAGE 7
#define HISTORY_PAGE 8

namespace Ui {
    class MobileApp;
}

class MobileApp : public QDialog
{
    Q_OBJECT

public:
    explicit MobileApp(QWidget *parent = 0);
    void resizeEvent();

    QStringList downloadsList;
    void downloadNext();
    ~MobileApp();

protected:
    void closeEvent(QCloseEvent *pEvent);
    void ClearTmp();

    //IZAR
    void keyReleaseEvent(QKeyEvent *keyEvent);

    //Adjust UI properties depending on device screen size
    void adjustToScreenSize();

    //set global font size to ui.
    void adjustFontSize();

private slots:

    void on_toIndexMenuBTN_clicked();

    void on_openBTN_clicked();
    void on_searchBTN_clicked();
    void on_getbooksBTN_clicked();
    void on_aboutBTN_clicked();
    void on_saveConf_clicked();
    void on_cancelBTN_clicked();
    void on_downloadBTN_clicked();

    void on_treeWidget_itemDoubleClicked(QTreeWidgetItem *item, int column);
    void on_treeWidget_itemClicked(QTreeWidgetItem *item, int column);

    void on_fontComboBox_currentIndexChanged(const QString &arg1);
    void on_fonSizeSpinBox_valueChanged(int arg1);

    void on_langComboBox_currentIndexChanged(const QString &arg1);

    void on_SearchInBooksBTN_released();

    void tdloadFinished(QUrl u, Book* book, BookIter iter);
    void tdloadStarted();


    void downloadProgress(int val);
    void downloadError();
    void downloadDone();
    void listDownloadDone();


    //IZAR
    void reloadBooklist();
    void resetSearchBookTree();

    // switch the view from normal to in search mode
    void showHideSearch(bool inSearch);

    // stacked widget currnet view canged.
    void viewChanged(int index);
    //go to previos view of stacked widget.
    void goBack();

    void showMenu();


    //Downloads the next book in the booksToDownload list.
    void downloadDWList();

    void on_downloadListWidget_itemClicked(QListWidgetItem *item);

    void on_settingsMenuBTN_clicked();

    void on_SearchTreeWidget_itemClicked(QTreeWidgetItem *item, int column);

    /* void on_systemLangCbox_clicked(bool checked); */
    void setupSettings();

    void resetSettingsPage();

    void setupBookmarkList();

    //copied from desktopapp
    void translate(QString newlang);


    void on_mixedSelectBTN_clicked();

    void setupMixedSelection();

    void on_openMixed_clicked();

    void on_markAllBTN_clicked();

    void on_unmarkAllBTN_clicked();

    void on_selectionArea_itemClicked(QListWidgetItem *item);

    void on_stopSearchBTN_clicked();

    /* void on_moreInfoBTN_clicked();
    void on_helpBTN_clicked(); */

    void on_resetBookListBTN_clicked();

    void on_lastBookBTN_clicked();

    void on_markAllBTN_2_clicked();

    void on_unmarkAllBTN_2_clicked();

    void on_searchInBooksLine_returnPressed();

    void on_markAllBTN_3_clicked();

    void on_unmarkAllBTN_3_clicked();

    void on_horizontalSlider_valueChanged(int value);


    void on_gtoHelp_clicked();

    void on_treeWidget_clicked(const QModelIndex &index);

    void on_openBook_clicked();

    void on_backBTN_clicked();

    void on_forwardBTN_clicked();

    void on_ZoomInBTN_clicked();

    void on_ZoomOutBTN_clicked();

    void titleUpdate(QUrl);

    void jumpToLastPos();
    
    void on_toMainMenuBTN_clicked();

    //book mark methods
    void addBookMark(Book *b, BookIter iter);

    //A maintenance function used to edit stuff in books. Should always be commented
    //void BookTool(int);

    void loadBookFromBM(QListWidgetItem *item);

    void on_bookMarksBTN_clicked();

    void on_helpBTN_clicked();

    void addStaticBMhere();


private:
    Ui::MobileApp *ui;

    void showBook(Book *);
    void showBook(Book *, BookIter);

    BookList bookList, booksInSearch;

    ///#####
    void parseDLFile(QList <QString> dl);



    QString booktitle;

    //TODO - create a global settings object
    //QSettings settings;


    struct DownloadbleBookObject
    {
        QString URL;
        QString UnpackPath;
        double fileSize;
        QDate dateModified;

        bool needToDownload;
    };

    struct DownloadbleBookGroup
    {
        QString name;
        double fullSize;
        double downloadSize;
        QList <DownloadbleBookObject> books;

        //0 - Installed, 1 - Needs update, 2 - Not installed
        int downloadState;
    };

    QList <DownloadbleBookGroup> groups;

    #define BOOKLISTURL "http://orayta.googlecode.com/files/OraytaBooksDownloadList"
    #define SAVEDBOOKLIST   TMPPATH + "Android-Books"
    FileDownloader *listdownload;
    FileDownloader *downloader;

    //Holds a list of books that the user already downloaded, so they won't be downloaded again.
    QStringList downloadedBooks;
    //Holds a list of books that the user requested to download.
    // These will be processed by the FileDownloader object.
    QStringList booksToDownload;

    void updateDownloadableList();

    //views from stacked widget:
//    QWidget *previousView;
//    QWidget *currentView;
//    QWidgetList *viewHistory;
    QList<int> *viewHistory;
    QList<int> exclude; //<< SETTINGS_PAGE<< MIXED_SELECTION_PAGE;

    QMovie * waitMovie;

    //----------------------------------------------------
    //copied from settings.h

    //Holds the available languages and the way they should be displayed (such as "עברית" for "Hebrew")
    QStringList langs, langsDisplay;

    textDisplayer *displayer;

};

#endif // MOBILEAPP_H
