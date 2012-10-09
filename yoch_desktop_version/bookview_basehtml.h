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
* Author: Yoch Melka. <yoch.melka@gmail.com>
*/

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
    virtual QWidget* widget();
    virtual void searchText(const QString& text, bool backwards);
    virtual void ZoomIn();
    virtual void ZoomOut();
    virtual void normalZoom();
    virtual void setInternalLocation(const QString& location);
    virtual void print();

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
