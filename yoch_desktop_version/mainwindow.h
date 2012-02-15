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
#include <QFontDialog>
#include <QStackedLayout>

#include "functions.h"
#include "htmlgen.h"
#include "ui_mainwindow.h"

#include "addbookmark.h"
#include "booktree.h"
#include "about.h"
#include "settings.h"
#include "errorreport.h"
#include "bookfind.h"
#include "importbook.h"
#include "searchresults.h"


class BookDisplayer;


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

    void adjustVtabCornerLayout (BookDisplayer*);

protected:
    //Overrides the normal "closeEvent", so it can save the program's confs before quiting
    void closeEvent(QCloseEvent *event);

    //Restores the program's confs
    void restoreConfs();

private:
    Ui::MainWindowClass *ui;

    //Remove all temporary html files the program created
    void ClearTmp();
    void connectMenuActions();

    int currentTab();

    BookDisplayer* bookdisplayer( int index );
    BookDisplayer* CurrentBookdisplayer();


    //Load the generated Html file of the given book into the WebView widget
    //void LoadHtmlBook(OraytaBookItem *, QString markString = "");

    //Searches for the given phrase in all books that have SearchMe as true,
    // creates a html file with the results, and loads it a new tab (unless current tab is empty)
    void SearchInSelectedBooks (const QRegExp& regexp, QString disp);

    void SearchGuematriaInTanach (const QString& txt);

    //Deals with keyboard events
    void keyPressEvent( QKeyEvent *keyEvent );

    //Switch GUI direction to RTL
    void setDirection(bool rtl);

    void removeNodeFromSearch(BaseNodeItem* node);
    void addNodeToSearch(BaseNodeItem* node);


private slots:

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
    void on_showSearchBarButton_clicked();

    void on_showaloneCBX_clicked(bool checked);

    void on_treeWidget_itemClicked(QTreeWidgetItem* current, int column);
    void on_treeWidget_currentItemChanged(QTreeWidgetItem*,QTreeWidgetItem*);

    void on_openMixed_clicked();

    void weavedCheckBoxClicked(QWidget*);

    void on_viewTab_tabCloseRequested(int index);
    void on_viewTab_currentChanged(int index);

    void closeCurrentTab();

    //Open the given book (by it's id in the booklist)
    void openBook( NodeBook* book, bool newTab = false );


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

    void on_bookmarkWidget_customContextMenuRequested(QPoint pos);
    //Opens a dialog allowing to edit the selected bookmark's title
    void on_bookmarkEdit_clicked();
    //Changes the title of the given bookmark (by it's index) to the given one
    void editBookMarkTitle(int index, QString newtitle);

    void menuBookMark();

    // #############
    void menuComment();
    void menuErrorReport();

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

    void printBook();

    //Called when an item at the treeView is double clicked, and if it's a book, loads it the current tab
    void on_treeWidget_itemDoubleClicked(QTreeWidgetItem* item, int column);

    //Creats a new tab, including it's webView and hBox, and connects the webView's signalls to it.
    BookDisplayer* addViewTab(bool set_current = true);

    //Shows the search tab
    void showSearchTab();

    void findBookForm();

    void menuOpenBook(NodeBook* book);

    //Shows the about form
    void about();

    //Shows the settings form
    void settingsForm();

    //Shows the import form
    void importForm();

    void toggleNikud(bool showNikud);
    void toggleTeamim(bool showTeamim);

    void toggleSearchBar();

    //Translate to the given lang
    void translate(QString newlang);

    // Show/hide buttons and menus depending on the currently visible book
    void adjustMenus();

    void searchGuematria();

private:
    //Set to true if the current search in books should be stopped
    bool stopSearchFlag ;

    QGroupBox* mixedCheckBox;

    QHBoxLayout* cornerWidgetLayout;

    QWidget* ptrToAdditionalWidget;
};


class MixedCheckBox : public QCheckBox
{
    Q_OBJECT
public:
    MixedCheckBox( QWidget* parent = 0 ) : QCheckBox(parent), mData(-1) {}
    MixedCheckBox( const QString & text, QWidget * parent = 0 ) : QCheckBox(text, parent), mData(-1) {}
    void setData(int data) { mData = data; }
    int getData() const    { return mData; }
private:
    int mData;
};

#endif // MAINWINDOW_H
