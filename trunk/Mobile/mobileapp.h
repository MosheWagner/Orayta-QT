/* This program is free software; you can redistribute it and/or modify
* it under the terms of the GNU General Public License version 2
* as published by the Free Software Foundation.
*
* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
* GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License
* along with this program; if not, write to the Free Software
* Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307 USA
*
* Author: Izar <izar00@gmail.com>
*/

#ifndef MOBILEAPP_H
#define MOBILEAPP_H

#include <QDialog>
#include <QModelIndex>
#include <QTreeWidgetItem>
#include <QListWidgetItem>
#include <QMovie>
#include <QUrl>
#include <QSignalMapper>
#include <QElapsedTimer>
#include "../OraytaBase/filedownloader.h"
#include "../OraytaBase/booklist.h"
#include "../OraytaBase/book.h"
#include "../OraytaBase/bookfind.h"

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
#define BOOKFIND_PAGE 9
#define KUKAYTA_PAGE 10


class MiniBMark;

namespace Ui {
    class MobileApp;
}

class MobileApp : public QDialog
{
    Q_OBJECT

public:
    explicit MobileApp(QWidget *parent = 0);

    void downloadNext();
    ~MobileApp();

protected:
    void closeEvent(QCloseEvent *pEvent);
    void ClearTmp();

    virtual void resizeEvent(QResizeEvent *);

    //IZAR
    void keyReleaseEvent(QKeyEvent *keyEvent);
    void mouseReleaseEvent(QMouseEvent *ev);

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


    //All of these are implemented in 'booksDownload.cpp':
        //Downloads the next book in the booksToDownload list.
        void downloadBookList();
        void downloadStart();
        void downloadProgress(int val);
        void downloadError();
        void downloadDone();
        void listDownloadDone();
        //Parse the booklist file
        void parseDLFile(QList <QString> dl);


    //IZAR
    void reloadBooklist();
    void resetSearchBookTree();

    void saveSettings();

    // switch the view from normal to in search mode
    void showHideSearch(bool inSearch);

    // stacked widget currnet view canged.
    void viewChanged(int index);
    //go to previos view of stacked widget.
    void goBack();

    void showMenu();

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

    //void BMClicked(QListWidgetItem *item);
    //int bmClickTimer;
    //void BMReleased(QListWidgetItem *item);
    void BMLongClicked(QListWidgetItem *item);
    void BMShortClicked(QListWidgetItem *item);
    void loadBookFromBM(QListWidgetItem *item);
    void removeBM();

    void on_bookMarksBTN_clicked();

    void on_helpBTN_clicked();

    void addStaticBMhere();


    void on_findBookBTN_clicked();

    void showBook(Book *);
    void showBook(Book *, BookIter);
    void showBook(int);

    void on_customFontRDBTN_toggled(bool checked);

    void on_autoResumeCKBX_stateChanged(int arg1);

    void on_NightModeCKBX_clicked(bool checked);


    void on_dlKukaytaBooksBTN_clicked();
    void on_installKukaytaBTN_clicked();
    void displayKukaytaMessage();

    void on_copyTextBTN_clicked();

private:
    Ui::MobileApp *ui;  

    BookList bookList, booksInSearch;

    QString booktitle;

    bool autoResume;

    bool nightMode;

    QMenu * menu;
    QAction * action;
    MiniBMark * bm;

    // true if the user wants to use custom font for all books.
    bool useCustomFontForAll;

    //TODO - create a global settings object
    //QSettings settings;


    struct DownloadbleBookObject
    {
        QString URL;
        QString UnpackPath;
        double fileSize;
        QDate dateModified;
        QString hash;

        bool needToDownload;
    };

    struct DownloadbleBookGroup
    {
        QString name;
        double fullSize;
        double downloadSize;
        QList <DownloadbleBookObject> books;
        bool hidden;

        //0 - Installed, 1 - Needs update, 2 - Not installed
        int downloadState;
    };

    QList <DownloadbleBookGroup> groups;

    #define BOOKLISTURL "http://orayta.googlecode.com/svn/books/OraytaBookList1"

    #define SAVEDBOOKLIST   TMPPATH + "Android-Books"

//    #define KUK_BOOKLIST_URL "http://orayta.googlecode.com/svn/books/kukaytaBookList"
//    #define KUK_SAVEDBOOKLIST   TMPPATH + "Kuk-Books"

    FileDownloader *listdownload;
    FileDownloader *downloader;

    //Holds a list of books that the user requested to download.
    // These will be processed by the FileDownloader object.
    QStringList downloadsList;
    QStringList hashs;
    int downloadNum;

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
    bookfind *bookFindDialog;

    //Holds the offset of the viewposition before the program was shut
    int lastViewPosition;

    //flag to let auto install of kukayta books
    bool autoInstallKukBooksFlag;

    //A timer to make sure we don't get double events. It's a small hack to prevent an android/emulator bug.
    QElapsedTimer timer;


};

#endif // MOBILEAPP_H
