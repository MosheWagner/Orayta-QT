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
* Author: Moshe Wagner. <moshe.wagner@gmail.com>
*/

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QCoreApplication>
#include <QTextStream>
#include <QIcon>
#include <QUrl>
#include <QtGui/QTreeWidget>
#include <QtWebKit>
#include <QtGui/QApplication>
#include <QtGui/QVBoxLayout>
#include <QtGui/QWidget>
#include <QtWebKit/QWebView>
#include <QtGui/QMainWindow>
#include <QtGui/QDirModel>
#include <QtGui/QTreeWidgetItem>
#include <QString>
#include <QFile>
#include <QMenu>
#include <QPrinter>
#include <QTextBrowser>
#include <QPrintDialog>
#include <qevent.h>
#include <qprocess.h>
#include <QMessageBox>
#include <QDesktopServices>
#include <QInputDialog>
#include <QMouseEvent>
#include <QSplashScreen>
#include <QDesktopWidget>
#include <QShortcut>
#include <QWebSettings>
#include <QFuture>
#include "functions.h"
#include "htmlgen.h"
#include "ui_mainwindow.h"
#include "book.h"
#include "addcomment.h"
#include "bookmarktitle.h"
#include "booklist.h"
#include "about.h"
#include "settings.h"
#include "errorreport.h"
#include "bookfind.h"
#include "bookdisplayer.h"
#include "mytreetab.h"
#include "importbook.h"


class bookDisplayer;

using namespace std;

namespace Ui
{
    class MainWindowClass;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = 0);
    ~MainWindow();

protected:
    //Overrides the normal "closeEvent", so it can save the program's confs before quiting
    void closeEvent(QCloseEvent *event);
    //Restores the program's confs
    void restoreConfs();
    void restoreBookConfs();

private:
    Ui::MainWindowClass *ui;

    //Remove all temporary html files the program created
    void ClearTmp();
    void connectMenuActions();

    //List of all the books
    BookList bookList;


    bookDisplayer* bookdisplayer( int index );
    bookDisplayer* CurrentBookdisplayer();

    //Load the generated Html file of the given book into the WebView widget
    void LoadHtmlBook(Book *, QString markString = "");

    //Searches for the given phrase in all books that have SearchMe as true,
    // creates a html file with the results, and loads it a new tab (unless current tab is empty)
    void SearchInBooks (const QRegExp& regexp, QString disp);

    void SearchGuematria (QString txt);

    //Builds the tree of books and folders in the treeWidget, based on the global book list
    void BuildBookTree();

    //Deals with keyboard events
    void keyPressEvent( QKeyEvent *keyEvent );

    //Switch GUI direction to RTL
    void setDirection(bool rtl);

    void addBookToSearch(Book*);
    void removeBookFromSearch(Book*);

    //Set to true if the current search in books should be stopped
    bool stopSearchFlag ;

private slots:

#ifdef POPPLER
    void updatePdfPage(int current, int max);
    void on_pdfDropBox_currentIndexChanged(QString );
#endif
    void on_addBookMark_clicked();
    void on_cancelSearchBTN_clicked();
    void on_SearchInBooksBTN_clicked();
    void on_radioButton_2_toggled(bool checked);
    void on_groupBox_toggled(bool checked);
    void on_guematriaCheckBox_toggled(bool checked);
    void on_radioButton_3_toggled(bool checked);

    void on_treeTab_currentChanged(int index);

    void on_searchInBooksLine_returnPressed();
    void on_hideSearchButton_clicked();
    void on_showSearchBarButton_clicked(bool checked);
    void on_refreshButton_clicked();

    void on_showaloneCBX_clicked(bool checked);

    void on_treeWidget_itemClicked(QTreeWidgetItem* current, int column);
    void on_openMixed_clicked();

    void weavedCheckBoxClicked(int btnIndex);

    void on_viewTab_tabCloseRequested(int index);

    void treeWidgetSelectionChanged(QTreeWidgetItem*,QTreeWidgetItem*);

    void closeCurrentTab();

    void on_viewTab_currentChanged(int index);
    //Open the given book (by it's id in the booklist)
    void openBook( Book* book );
    //Calls "openBook", but uses the currently selected book in the tree
    void openSelectedBook();
    //Opens a new tab and Calls "openBook" (using the currently selected book in the tree).
    void openSelectedBookInNewTab();
    //Delete selected book
    void deleteSelectedBook();

    void changeFont();
    //Collapse the currently selected item in the treewidget
    void collapseItem();
    //Expand the currently selected item in the treewidget
    void expandItem();

    void on_bookmarkWidget_customContextMenuRequested(QPoint pos);
    QString bookMarkTitle(QString lnk);
    //Opens the bookmark dialog
    void addBookMark(QString link, QString title);
    //Adds the given book position to the bookmark list and file
    void bookMarkPosition(QString lnk);
    //Adds a bookmark to the bookmark list, (giving it's encoded link)
    void addBookMarkToList(QString lnk, QString title="");
    //Move the selected bookmark down in the list by one position
    void on_bookmarkDown_clicked();
    //Move the selected bookmark up in the list by one position
    void on_bookmarkUp_clicked();
    //Remove the selected bookmark from the list and file
    void on_removeBookmark_clicked();
    //Build the bookmark list from the bookmark file
    void buildBookMarkList();
    //Open the clicked bookmark
    void on_bookmarkWidget_itemDoubleClicked(QListWidgetItem* item);
    //Opens a dialog allowing to edit the selected bookmark's title
    void on_bookmarkEdit_clicked();
    //Changes the title of the given bookmark (by it's index) to the given one
    void editBookMarkTitle(int index, QString newtitle);

    void menuBookMark();


    //Show the comment adding / editing dialog
    void openCommentDialog(QString str);
    //Add the given comment at the given book position (called from the "add comment" dialog's signal)
    void addCommentAtPosition(QString, QString);
    //Remove the comment at the given link
    void removeComment(QString link);

    void menuComment();
    void menuErrorReport();

    void on_treeWidget_customContextMenuRequested(QPoint pos);
    void on_searchBackward_clicked();
    void on_searchForward_clicked();
    void on_topButton_clicked();
    void on_lineEdit_returnPressed();
    void on_zoomoutButton_clicked();
    void on_zoominButton_clicked();

    void on_removeAllFromSearchButton_clicked();
    void on_addAllToSearchButton_clicked();
    void removeFromSearch();
    void addToSearch();

    void on_newTabButton_clicked();

    void openExternalLink(QString link);

    //void printBook();

    //Called when an item at the treeView is double clicked, and if it's a book, loads it the current tab
    void on_treeWidget_itemDoubleClicked(QTreeWidgetItem* item, int column);

    //Creats a new tab, including it's webView and hBox, and connects the webView's signalls to it.
    void addViewTab(bool);

    //Shows the search tab
    void showSearchTab();

    void findBookForm();

    //TODO: ///
    void menuOpenBook(int uid);

    //Shows the about form
    void about();

    //Shows the settings form
    void settingsForm();

    //Shows the import form
    void importForm();

    void updateBookTree();

    void toggleNikud(bool showNikud );
    void toggleTeamim(bool showTeamim);

    void ToggleSearchBar();

    //Translate to the given lang
    void translate(QString newlang);

    // Show/hide buttons and menus depending on the currently visible book
    void adjustMenus();

    void searchGuematria();

};

//void buidSearchDBinBG(BookList * bl);

#endif // MAINWINDOW_H
