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
    reloadAction = pageAction(QWebPage::Reload);
    reloadAction->setShortcut(QKeySequence::Refresh);

    addAction(backAction);
    addAction(forwardAction);
    addAction(reloadAction);

    backBtn->setDefaultAction(backAction);
    backBtn->setAutoRaise(true);
    frwdBtn->setDefaultAction(forwardAction);
    frwdBtn->setAutoRaise(true);

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
