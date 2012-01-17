#ifndef BOOKVIEW_BASEHTML_H
#define BOOKVIEW_BASEHTML_H


#if defined(_MSC_VER) && _MSC_VER >= 1500
# include <functional>
#else
# include <tr1/functional>
#endif

#include "bookviewinterface.h"

#include <QWebView>
#include <QAction>


typedef std::tr1::function< void(void) > fn_obj;


class BaseHtmlView : public QWebView, public BookViewInterface
{
    Q_OBJECT

public:
    BaseHtmlView(BookDisplayer * parent);
    ~BaseHtmlView();

    virtual NodeBook::Booktype booktype() const = 0;
    virtual void searchText(const QString& text, bool backwards);
    virtual void ZoomIn();
    virtual void ZoomOut();
    virtual void normalZoom();
    virtual void setInternalLocation(const QString& location);

    void loadUrl(const QUrl&);
    void execScript(const QString& script);
    QString getJSVar(const QString& var);

public slots:
    void setTextSizeMultiplier(qreal factor);
    void on_loadFinished(bool);

signals:

protected:
    void addDelayedAction(fn_obj func);

    QAction *copyAction;
    QAction* reloadAction;

    bool loading;

private:
    QList<fn_obj> delayedActions;
};


#endif // BOOKVIEW_H
