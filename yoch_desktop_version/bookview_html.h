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
