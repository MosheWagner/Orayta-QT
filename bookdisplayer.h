#ifndef BOOKDISP_H
#define BOOKDISP_H

#include "mainwindow.h"
#include "khtmlwidget.h"
#include "mywebview.h"
#include <QStackedWidget>
/*
  Class holding all widgets and objects needed to display a book.

*/

class myWebView;
class MainWindow;

class bookDisplayer : public QWidget
{
    //Very important. Without it, it won't create signalls or slots.
    Q_OBJECT

public:
    bookDisplayer(QWidget *parent, QTabWidget * tabviewptr);
    ~bookDisplayer();
    //Set the html that should be shown in the htmlviewer
    void setHtml(QString html);
    //Load given url
    void load(QUrl url);

    //The book displayed here
    void setBook( Book * );
    Book * book();

    bool isNikudShown();
    bool areTeamimShown();

    void showNikud(bool show);
    void showTeamim(bool show);

    QString title();
    QUrl url();
    QString activeLink();

    void execScript(QString script);
    QString getJSVar(QString var);

    void setTextSizeMultiplier(qreal factor);
    qreal textSizeMultiplier();

    void setInternalLocation(QString location);

    QWidget * htmlQWidget();
    
    QString htmlSource();

private:

    //Html viewer the book is shown within
#ifndef KHTML
    myWebView * htmlview;
#else
    KHTMLWidget * htmlview;
#endif

    MainWindow * MW;
    QTabWidget * TW;
    
    //Layout holding the widgets
    QVBoxLayout *vbox;

    //Wait movie
    QMovie *mov;

    //Label displaying the wait movie
    QLabel *waitlbl;

    //Location in html file next load should use
    QString InternalLocationInHtml;

    //Pointer to the book that's displayed here
    Book *myBook;

    bool shownikud;
    bool showteamim;

    QUrl myurl;
    QString mActiveLink;

    KHTMLWidget * waitView;

    QStackedWidget *stackedWidget;
private slots:
    void htmlView_linkClicked(QUrl url);
#ifdef KHTML
    void htmlView_loadFinished();
#else
    void htmlView_loadFinished(bool);
#endif


};

#endif // BOOKDISP_H
