#ifndef BOOKVIEW_HTML_H
#define BOOKVIEW_HTML_H

#include "bookview_basehtml.h"

#include <QToolButton>
#include <QHBoxLayout>

class HtmlBookView : public BaseHtmlView
{
public:

    HtmlBookView(BookDisplayer* parent);

    virtual NodeBook::Booktype booktype() const;
    virtual void loadBook(const NodeBook*);
    virtual QWidget* additionalButtons();

private:

    QWidget* mAdditionalButtons;
    QHBoxLayout* mAdditionalLayout;
    QToolButton* backBtn;
    QToolButton* frwdBtn;

    QAction* backAction;
    QAction* forwardAction;

    //HtmlBookItem* mInternalBook;
};


#endif // BOOKVIEW_HTML_H
