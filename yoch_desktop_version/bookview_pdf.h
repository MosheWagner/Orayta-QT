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

#ifndef BOOKVIEW_PDF_H
#define BOOKVIEW_PDF_H

#include "pdfwidget.h"
#include "bookviewinterface.h"

class PdfBookView : public PdfWidget, public BookViewInterface
{
    Q_OBJECT
public:
    PdfBookView(BookDisplayer * parent);

    virtual QWidget* widget();
    virtual NodeBook::Booktype booktype() const;
    virtual void loadBook(const NodeBook*);
    virtual void searchText(const QString& text, bool backwards);
    virtual void ZoomIn();
    virtual void ZoomOut();
    virtual void normalZoom();
    virtual void jumpToTop();
    virtual QWidget* additionalButtons();

private slots:
    void setPdfPage(int page);

private:
    QWidget* mAdditionalButtons;
    QHBoxLayout* mAdditionalLayout;
    QSpinBox* pdfDropBox;
    QLabel* pagelbl;
};


#endif // BOOKVIEW_PDF_H
