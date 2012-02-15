#include "bookview_pdf.h"
#include "bookdisplayer.h"
#include "functions.h"


PdfBookView::PdfBookView( BookDisplayer * parent ) :
    PdfWidget(parent),
    BookViewInterface(parent),
    mAdditionalLayout(new QHBoxLayout),
    mAdditionalButtons(new QWidget(this)),
    pdfDropBox(new QSpinBox(this)),
    pagelbl(new QLabel(this))
{
    mAdditionalLayout->setDirection(QBoxLayout::RightToLeft);
    mAdditionalLayout->setContentsMargins(0,0,0,0);
    mAdditionalLayout->addWidget(pdfDropBox);
    mAdditionalLayout->addWidget(pagelbl);
    mAdditionalButtons->setLayout(mAdditionalLayout);

    QObject::connect(pdfDropBox, SIGNAL(valueChanged(int)), this, SLOT(setPdfPage(int)));
}

QWidget* PdfBookView::widget()
{  return this;  }

NodeBook::Booktype PdfBookView::booktype() const
{  return NodeBook::Pdf;  }

void PdfBookView::loadBook(const NodeBook* book)
{
    if (book->booktype() != NodeBook::Pdf) return;  // sanity check

    setDocument(book->getLoadableFile());
    pdfDropBox->setRange(1, numPages());
}

void PdfBookView::jumpToTop()
{
    ensureVisible(0,0);
}

void PdfBookView::ZoomIn()
{
    setScale(scale() * 1.2);
}

void PdfBookView::ZoomOut()
{
    if (scale() > 0.1)
    {
        setScale(scale() * 0.8);
    }
}

void PdfBookView::normalZoom()
{
    setScale(1);
}

void PdfBookView::searchText(const QString& text, bool backwards)
{
    QRectF location;
    if (!backwards)
        location = searchForwards(text);
    else
        location = searchBackwards(text);

    //Force scrollarea to show the found position
    QPoint target = matrix().mapRect(location).center().toPoint();
    ensureVisible(target.x(), target.y());
}

QWidget* PdfBookView::additionalButtons()
{  return mAdditionalButtons;  }

void PdfBookView::setPdfPage(int page)
{
    if (page > 0 && page <= numPages())
    {
        pagelbl->setText( tr("Page ") + QString::number(page) + " / " + QString::number(numPages()) );
        setPage(page);
    }
}
