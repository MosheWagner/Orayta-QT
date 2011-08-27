/*
* This file is  based on the "poppler" exapmle from Qt's documantation,
* ( Copyright info here below), but has many changes done me.
*
* Moshe Wagner. <moshe.wagner@gmail.com>
*/


/****************************************************************************
**
** Copyright (C) 2008 Nokia Corporation and/or its subsidiary(-ies).
** Contact: Qt Software Information (qt-info@nokia.com)
**
** This file is part of the documentation of Qt. It was originally
** published as part of Qt Quarterly.
**
** Commercial Usage
** Licensees holding valid Qt Commercial licenses may use this file in
** accordance with the Qt Commercial License Agreement provided with the
** Software or, alternatively, in accordance with the terms contained in
** a written agreement between you and Nokia.
**
**
** GNU General Public License Usage
** Alternatively, this file may be used under the terms of the GNU
** General Public License versions 2.0 or 3.0 as published by the Free
** Software Foundation and appearing in the file LICENSE.GPL included in
** the packaging of this file.  Please review the following information
** to ensure GNU General Public Licensing requirements will be met:
** http://www.fsf.org/licensing/licenses/info/GPLv2.html and
** http://www.gnu.org/copyleft/gpl.html.  In addition, as a special
** exception, Nokia gives you certain additional rights. These rights
** are described in the Nokia Qt GPL Exception version 1.3, included in
** the file GPL_EXCEPTION.txt in this package.
**
** Qt for Windows(R) Licensees
** As a special exception, Nokia, as the sole copyright holder for Qt
** Designer, grants users of the Qt/Eclipse Integration plug-in the
** right for the Qt/Eclipse Integration to link to functionality
** provided by Qt Designer and its related libraries.
**
** If you are unsure which license is appropriate for your use, please
** contact the sales department at qt-sales@nokia.com.
**
****************************************************************************/



#ifdef  POPPLER

#include "pdfwidget.h"

//TODO: Document

// Pdfwidget constructor
PdfWidget::PdfWidget(QWidget *parent) : QScrollArea(parent)
{
    viewlbl = new QLabel();

    viewlbl->setMouseTracking(false);

    setWidget(viewlbl);
    setWidgetResizable(true);
    verticalScrollBar()->setTracking(true);

    current_page = -1;
    doc = 0;
    rubberBand = 0;
    scaleFactor = 1.0;
    viewlbl->setAlignment(Qt::AlignCenter);


    //Init "copy" menu
    menu = new QMenu(this);
    copy = new QAction(QIcon(":/Icons/edit-copy.png"), tr("Copy text"), this);
    menu->addAction(copy);
    connect (copy, SIGNAL(triggered()), this, SLOT(copyText()));

    connect(verticalScrollBar(), SIGNAL(valueChanged(int)), this, SLOT(sliderValueChanged(int)));
    connect(verticalScrollBar(), SIGNAL(rangeChanged(int,int)), this, SLOT(sliderRangeChanged(int,int)));

    setCursor(Qt::IBeamCursor);

    useRTL = false;
}

PdfWidget::~PdfWidget()
{
    delete doc;
}

//Returns the PdfWidget's document
Poppler::Document *PdfWidget::document()
{
    return doc;
}

QMatrix PdfWidget::matrix() const
{
    return QMatrix(scaleFactor * viewlbl->physicalDpiX() / 72.0, 0,
                   0, scaleFactor * viewlbl->physicalDpiY() / 72.0,
                   0, 0);
}


void PdfWidget::DrawRects(QList <QRect> rectlist, QColor color)
{
    QImage i(Image.size(), Image.format());

    foreach (QRect rect, rectlist)
    {
        QRect highlightRect = matrix().mapRect(rect);
        highlightRect.adjust(0, -1, 0, 1);

        QPainter painter;
        painter.begin(&i);

        painter.setCompositionMode(QPainter::CompositionMode_Source);

        QColor c;
        c.setRgba(color.rgba());

        QBrush  b(c);
        painter.fillRect(highlightRect, b);

        painter.end();
    }

    QImage image(Image);

    QPainter p;
    p.begin(&image);
    p.drawImage(QPoint(0,0), i);
    p.end();

    viewlbl->setPixmap(QPixmap::fromImage(image));
}

//Returns a QLine representing the size and position
//  of the text line that is at the given point.
//
// If the point is not in the middle of any line -
//  a height of 4 pixels, and the position of the given point are returned.
QLine PdfWidget::TextLine(QPoint p)
{
    int h = 4; //Minimal size

    //Create a thin rect going across the whole page
    QRect r(0,p.y(),10000,1);
    r = matrix().inverted().mapRect(r);

    p = matrix().inverted().map(p);
    int top = p.y();

    foreach (Poppler::TextBox *box, doc->page(current_page)->textList())
    {
        //See if the rect intersects any word on the page.
        // Find the biggest word, and remember it's top and height.
        if (box->boundingBox().intersects(r))
        {
            int nh = box->boundingBox().height();

            if (nh > h)
            {
                h = nh;
                top = box->boundingBox().top();
            }
        }

    }

    return QLine(QPoint(p.x(), top), QPoint(p.x(), top + h));
}


//Returns a list of QRects, marking of all text selected between the two given points
QList <QRect> PdfWidget::SelectedText(QPoint p1, QPoint p2, bool RTL)
{
    selectedWords.clear();

    QList <QRect> hits;

    //Make sure p1 is the top point
    if (p1.y() > p2.y()) swap(p1, p2);

    //Determine top and bottem line height
    QLine l1 = TextLine(p1);
    QLine l2 = TextLine(p2);

    //One line mode (if l1 and l2's heights intersect)
    if ( l2.y1() <= l1.y2())
    {
        QRect textRect(p1, p2);
        textRect = matrix().inverted().mapRect(textRect);

        foreach (Poppler::TextBox *box, doc->page(current_page)->textList())
        {
            if (box->boundingBox().intersects(textRect))
            {
            selectedWords << box;
            for (int i=0; i<box->text().length(); i++)
            {
                QRect rect = box->charBoundingBox(i).toAlignedRect();
                if (rect.intersects(textRect))
                {
                    rect.adjust(1,0,0,0);
                    hits << rect;
                }
            }
            }
        }
    }
    else
    {
        p1 = matrix().inverted().map(p1);
        p2 = matrix().inverted().map(p2);
        QRect topline;
        QRect bottemline;

        if (RTL)
        {
            topline.setCoords(p1.x(), l1.p1().y(), 0, l1.p2().y() );
            bottemline.setCoords(p2.x(), l2.p1().y(), 1000, l2.p2().y());
        }
        else
        {
            topline.setCoords(p1.x(), l1.p1().y(), 1000, l1.p2().y() );
            bottemline.setCoords(p2.x(), l2.p1().y(), 0, l2.p2().y() );
        }

        QRect rest( QPoint(0, l1.p2().y() + 5), QPoint(1000, l2.p1().y() - 5) );

        if (rest.top() <= topline.bottom() || rest.bottom() >= bottemline.top()) rest = topline;
        if (rest.intersects(topline) || rest.intersects(bottemline)) rest = topline;

        if (bottemline.top() <= topline.bottom() ) bottemline = topline;


        foreach (Poppler::TextBox *box, doc->page(current_page)->textList())
        {
            if (box->boundingBox().intersects(topline) || box->boundingBox().intersects(bottemline) || box->boundingBox().intersects(rest))
            {
                selectedWords << box;
                for (int i=0; i<box->text().length(); i++)
                {
                    QRect rect = box->charBoundingBox(i).toAlignedRect();
                    if (rect.intersects(topline) || rect.intersects(bottemline) || rect.intersects(rest))
                    {
                        rect.adjust(-1,0,1,0);
                        hits << rect;
                    }
                }
            }
        }
    }


    return hits;
}


void PdfWidget::mousePressEvent(QMouseEvent *event)
{
    if (!doc)
        return;

    if (event->button() == Qt::LeftButton)
    {
        dragPosition = event->globalPos();
        dragPosition = viewlbl->mapFromGlobal(dragPosition);

        //Clean
        selected.clear();
        DrawRects(selected, QColor());
        selectedWords.clear();
    }
}

void PdfWidget::mouseMoveEvent(QMouseEvent *event)
{
    if (!doc)
        return;

    QPoint newPosition = event->globalPos();
    newPosition = viewlbl->mapFromGlobal(newPosition); 

    selected = SelectedText(dragPosition, newPosition, useRTL);
    QColor c("Blue"); c.setAlpha(160);
    DrawRects(selected, c);
}

void PdfWidget::mouseReleaseEvent(QMouseEvent * event)
{
    if (!doc)
        return;

    if (event->button() == Qt::RightButton)
    {
        menu->setLayoutDirection(Qt::RightToLeft);
        menu->exec(event->globalPos());
    }
}

qreal PdfWidget::scale() const
{
    return scaleFactor;
}

void PdfWidget::showPage(int page)
{

    if (page != -1 && page != current_page + 1) {
        current_page = page - 1;
        emit pageChanged(page, doc->numPages());
    }

    Poppler::Page* pdfPage = doc->page(current_page);
    if (pdfPage == 0) {
        qDebug() << "Couldn't open page " << current_page;
        return;
    }

    Image = pdfPage->renderToImage(scaleFactor * viewlbl->physicalDpiX(), scaleFactor * viewlbl->physicalDpiY());

    if (Image.isNull()) {
        qDebug() << "Poppler::Page::renderToImage fail...";
        return;
    }

    if (!searchLocation.isEmpty()) {
        QRect highlightRect = matrix().mapRect(searchLocation).toRect();
        highlightRect.adjust(-1, -1, 1, 1);

        QPainter painter;
        painter.begin(&Image);

        QColor c;
        c.setRgb(QColor("Yellow").rgb());
        c.setAlpha(130);

        QBrush  b(c);
        painter.fillRect(highlightRect, b);

        painter.end();
    }

    viewlbl->setPixmap(QPixmap::fromImage(Image));

    //Decide if selection should be RTL or not:
    int count = 0;
    foreach (Poppler::TextBox *box, doc->page(current_page)->textList())
    {
        if (QTextCodec::codecForName("latin1")->canEncode(box->text())) count ++;
    }
    if (count < (doc->page(current_page)->textList().size()) / 2) useRTL = true;

}

QRectF PdfWidget::searchBackwards(const QString &stext)
{
    //Invert text to visual (flips hebrew chars).
    QString text = ToBidiText(stext);

    QRectF oldLocation = searchLocation;

    int page = current_page;
    if (oldLocation.isNull())
        page -= 1;

    while (page > -1) {

        QList<QRectF> locations;
        searchLocation = QRectF();

        while (doc->page(page)->search(text, searchLocation, Poppler::Page::NextResult, Poppler::Page::CaseInsensitive))
        {
            if (searchLocation != oldLocation)
                locations.append(searchLocation);
            else
                break;
        }

        int index = locations.indexOf(oldLocation);
        if (index == -1 && !locations.isEmpty()) {
            searchLocation = locations.last();
            showPage(page + 1);
            return searchLocation;
        } else if (index > 0) {
            searchLocation = locations[index - 1];
            showPage(page + 1);
            return searchLocation;
        }

        oldLocation = QRectF();
        page -= 1;
    }

    if (current_page == doc->numPages() - 1)
        return QRectF();

    oldLocation = QRectF();
    page = doc->numPages() - 1;

    while (page > current_page) {

        QList<QRectF> locations;
        searchLocation = QRectF();

        while (doc->page(page)->search(text, searchLocation,
                                       Poppler::Page::NextResult, Poppler::Page::CaseInsensitive)) {

            locations.append(searchLocation);
        }

        if (!locations.isEmpty()) {
            searchLocation = locations.last();
            showPage(page + 1);
            return searchLocation;
        }
        page -= 1;
    }

    return QRectF();
}

QRectF PdfWidget::searchForwards(const QString &stext)
{

    //Invert text to visual (flips hebrew chars).
    QString text = ToBidiText(stext);

    int page = current_page;
    while (page < doc->numPages()) {

        if (doc->page(page)->search(text, searchLocation,
                                    Poppler::Page::NextResult, Poppler::Page::CaseInsensitive)) {
            if (!searchLocation.isNull()) {
                showPage(page + 1);
                return searchLocation;
            }
        }
        page += 1;
        searchLocation = QRectF();
    }

    page = 0;

    while (page < current_page) {

        searchLocation = QRectF();

        if (doc->page(page)->search(text, searchLocation,
                                    Poppler::Page::NextResult, Poppler::Page::CaseInsensitive)) {
            if (!searchLocation.isNull()) {
                showPage(page + 1);
                return QRectF();
                return searchLocation;
            }
        }
        page += 1;
    }

    return QRectF();
}

void PdfWidget::copyText()
{

    QString text = "";
    QPointF center;
    bool hadSpace = false;

    foreach (Poppler::TextBox *box, selectedWords)
    {
        if (hadSpace)
            text += " ";
        if (!text.isEmpty() && box->boundingBox().top() > center.y())
            text += "\n";
        text += box->text();
        hadSpace = box->hasSpaceAfter();
        center = box->boundingBox().center();
    }

    if (!text.isEmpty())
    {
        //Invert text to visual (flips hebrew chars).
        text = ToBidiText(text);

        //Copy to clipboard:
        QClipboard *clipboard = QApplication::clipboard();
        clipboard->setText(text, QClipboard::Clipboard);
    }
}

bool PdfWidget::setDocument(const QString &filePath)
{
    Poppler::Document *oldDocument = doc;

    /*
    //Don't work on some MS-Windows systems
    doc = Poppler::Document::load(filePath);
    */
    QFile file(filePath);
    if (!file.open(QIODevice::ReadOnly))
    {
        qDebug() << "couldn't open " << filePath;
        return false;
    }

    QByteArray data = file.read( file.size() );
    file.close();

    doc = Poppler::Document::loadFromData(data);
    if (doc) {
        delete oldDocument;
        doc->setRenderHint(Poppler::Document::Antialiasing);
        doc->setRenderHint(Poppler::Document::TextAntialiasing);
        searchLocation = QRectF();
        current_page = -1;
        setPage(1);
    }
    else
    {
        qDebug() << "Couldn't open " << filePath;
    }

    return doc != 0;
}

void PdfWidget::setPage(int page)
{
    if (page != -1 && page != current_page + 1) {
        searchLocation = QRectF();
        showPage(page);
    }
}

void PdfWidget::setScale(qreal scale)
{
    if (scaleFactor != scale) {
        scaleFactor = scale;
        showPage();
    }
}

int PdfWidget::numPages() const
{
    if (doc)
        return doc->numPages();
    else
        return 0;
}

int PdfWidget::currentPage() const
{
    return current_page + 1;
}

void PdfWidget::sliderValueChanged(int val)
{
    if (val == mSliderTop)
    {
        if (mSTO == true)
        {
            previousPage();
            mSTO = false;
        }
        else
        {
            mSTO = true;
        }
    }
    else if (val == mSliderBottem)
    {
        if (mSBO == true)
        {
            nextPage();
            mSBO = false;
        }
        else
        {
            mSBO = true;
        }
    }
}

void PdfWidget::sliderRangeChanged(int top, int bottem)
{
    mSliderTop = top;
    mSliderBottem = bottem;
}

void PdfWidget::nextPage()
{
    if (numPages() > current_page)
    {
        setPage(current_page + 2);
        verticalScrollBar()->setValue(0);
        mSTO = false;
    }
}

void PdfWidget::previousPage()
{
    if (current_page > 0)
    {
        setPage(current_page);
        verticalScrollBar()->setValue(0);
        mSTO = false;
    }
}

#endif
