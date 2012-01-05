#ifndef MOBILEAPP_H
#define MOBILEAPP_H

#include <QDialog>
#include <QModelIndex>
#include <QTreeWidgetItem>
#include "filedownloader.h"
#include "booklist.h"

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

private slots:

    void on_openBTN_clicked();

    void on_searchBTN_clicked();

    void on_getbooksBTN_clicked();

    void on_aboutBTN_clicked();

    void on_treeWidget_clicked(const QModelIndex &index);

    void on_openMixed_clicked();

    void on_treeWidget_itemDoubleClicked(QTreeWidgetItem *item, int column);

    void on_toolButton_clicked();

    void wvloadFinished(bool);
    void wvlinkClicked(const QUrl &);

    void on_toolButton_3_clicked();

    void on_toolButton_2_clicked();

    void on_toolButton_6_clicked();

    void on_title_clicked();

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
    //hack- remove at will
    void listDownloadDoneOverride();

    //IZAR
    void reloadBooklist();

    //Downloads the next book in the booksToDownload list.
    void downloadDWList();

    void on_horizontalSlider_sliderMoved(int position);

    void on_downloadListWidget_itemDoubleClicked(QListWidgetItem *item);

    void markDownloadedBooks();

private:
    Ui::MobileApp *ui;

    void showBook(Book *);

    BookList bookList;

    QString booktitle;

    QString InternalLocationInHtml;

    QWebView * wview;


    #define BOOKLISTURL "http://orayta.googlecode.com/files/Android-Books"

    FileDownloader *listdownload;
    FileDownloader *downloader;

    //Holds a list of books that the user already downloaded, so they won't be downloaded again.
    QStringList downloadedBooks;
    //Holds a list of books that the user requested to download.
    // These will be processed by the FileDownloader object.
    QStringList booksToDownload;


};

#endif // MOBILEAPP_H
