#ifndef MOBILEAPP_H
#define MOBILEAPP_H

#include <QDialog>
#include <QModelIndex>
#include <QTreeWidgetItem>
#include "filedownloader.h"
#include "booklist.h"
#include "book.h"

#include "mywebview.h"

namespace Ui {
    class MobileApp;
}

class MobileApp : public QDialog
{
    Q_OBJECT

public:
    explicit MobileApp(QWidget *parent = 0);
     QStringList downloadsList;
      void downloadNext();
    ~MobileApp();

protected:
    void closeEvent(QCloseEvent *pEvent);

    //IZAR
    void keyReleaseEvent(QKeyEvent *keyEvent);

private slots:

    void on_openBTN_clicked();

    void on_searchBTN_clicked();

    void on_getbooksBTN_clicked();

    void on_aboutBTN_clicked();

    void on_treeWidget_clicked(const QModelIndex &index);

    void on_openBook_clicked();

    void on_treeWidget_itemDoubleClicked(QTreeWidgetItem *item, int column);
    void on_treeWidget_itemClicked(QTreeWidgetItem *item, int column);

    //void wvloadFinished(bool);
    //void wvloadStarted();

    void on_toolButton_3_clicked();

    void on_toolButton_2_clicked();

    void on_toolButton_6_clicked();

    void on_settings_BTN_clicked();

    void on_saveConf_clicked();

    void on_fontComboBox_currentIndexChanged(const QString &arg1);

    void on_fonSizeSpinBox_valueChanged(int arg1);

    void on_cancelBTN_clicked();

    void on_SearchInBooksBTN_clicked();

    void on_downloadBTN_clicked();


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

    void on_downloadListWidget_itemDoubleClicked(QListWidgetItem *item);

    void markDownloadedBooks();

    void on_settingsMenuBTN_clicked();

    void on_doneBTN_clicked();

    void on_selectBooksForSearchLink_clicked();

    void on_SearchTreeWidget_itemClicked(QTreeWidgetItem *item, int column);

    /* void on_systemLangCbox_clicked(bool checked); */
    void setupSettings();

    void resetSettingsPage();

    //copied from desktopapp
    void translate(QString newlang);


    void on_langComboBox_currentIndexChanged(const QString &arg1);

    // *copied from desktop app*
    //Remove all temporary html files the program created
    void ClearTmp();

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

//    void on_backBTN_clicked();

//    void on_forwardBTN_clicked();



    void on_markAllBTN_2_clicked();

    void on_unmarkAllBTN_2_clicked();

    void on_searchInBooksLine_returnPressed();

    void on_markAllBTN_3_clicked();

    void on_unmarkAllBTN_3_clicked();

    void on_toGetBooksBTN_clicked();

    // constructor continuation
    void continueConstructor();

    void on_menuHelpBTN_clicked();

    void on_horizontalSlider_valueChanged(int value);

    void on_gtoHelp_clicked();

    void on_textBrowser_anchorClicked(const QUrl &arg1);

private:
    Ui::MobileApp *ui;

    void showBook(Book *);

    // test if 'currnetBook' is displayed now
    bool currentBookDisplayed();

    BookList bookList, booksInSearch;

    Book *currentBook;
    QPoint currentPossision;

    QString booktitle;

    QString InternalLocationInHtml;

    //TODO - create a global settings object
    //QSettings settings;


    #define BOOKLISTURL "http://orayta.googlecode.com/files/Android-Books"
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

    //shows a moving wait image
    QMovie *waitMovie;

    //----------------------------------------------------
    //copied from settings.h

    //Holds the available languages and the way they should be displayed (such as "עברית" for "Hebrew")
    QStringList langs, langsDisplay;


};

#endif // MOBILEAPP_H
