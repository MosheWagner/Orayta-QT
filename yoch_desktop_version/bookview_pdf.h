#ifndef BOOKVIEW_PDF_H
#define BOOKVIEW_PDF_H

#include "pdfwidget.h"
#include "bookviewinterface.h"

class PdfBookView : public PdfWidget, public BookViewInterface
{
    Q_OBJECT
public:
    PdfBookView(BookDisplayer * parent);

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
