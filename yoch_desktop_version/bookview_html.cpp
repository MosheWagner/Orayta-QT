#include "bookview_html.h"



HtmlBookView::HtmlBookView( BookDisplayer * parent ) :
    BaseHtmlView(parent),
    mAdditionalButtons(new QWidget(this)),
    mAdditionalLayout(new QHBoxLayout),
    backBtn(new QToolButton(this)),
    frwdBtn(new QToolButton(this))
{
    backAction = pageAction(QWebPage::Back);
    backAction->setShortcut(QKeySequence::Back);
    forwardAction = pageAction(QWebPage::Forward);
    forwardAction->setShortcut(QKeySequence::Forward);

    addAction(backAction);
    addAction(forwardAction);

    backBtn->setDefaultAction(backAction);
    frwdBtn->setDefaultAction(forwardAction);

    mAdditionalLayout->setContentsMargins(0,0,0,0);
    mAdditionalLayout->setDirection(QBoxLayout::RightToLeft);
    mAdditionalLayout->addWidget(backBtn);
    mAdditionalLayout->addWidget(frwdBtn);
    mAdditionalButtons->setLayout(mAdditionalLayout);
}

NodeBook::Booktype HtmlBookView::booktype() const
{  return NodeBook::Html;  }

/* ########## VERY IMPORTANT
A partir du moment ou l'on clique sur un lien externe, il faut impérativement supprimer l'association entre
le BookDisplayer et le NodeBook, sinon on ne peut plus ouvrir le livre par clic sur le TreeWidget.
--- fait pour l'instant exterieurement ---
*/
void HtmlBookView::loadBook(const NodeBook* book)
{
    if (book->booktype() != NodeBook::Html) return;  // sanity check

    loadUrl(QUrl::fromLocalFile(book->getLoadableFile()));
}

/*
void HtmlBookView::reload()
{
    page()->triggerAction(QWebPage::Reload);
}
*/

QWidget* HtmlBookView::additionalButtons()
{  return mAdditionalButtons;  }
