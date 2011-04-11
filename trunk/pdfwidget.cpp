/*
* This file is taken from the "poppler" exapmle from Qt's documantation,
* ( Copyright info here below),
* But has changes done me.
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

void PdfWidget::mousePressEvent(QMouseEvent *event)
{
    if (!doc)
        return;

    dragPosition = event->globalPos();
    dragPosition = viewlbl->mapFromGlobal(dragPosition);

    if (!rubberBand)
        rubberBand = new QRubberBand(QRubberBand::Rectangle, viewlbl);
    rubberBand->setGeometry(QRect(dragPosition, QSize()));
    rubberBand->show();
}

void PdfWidget::mouseMoveEvent(QMouseEvent *event)
{
    if (!doc)
        return;

    QPoint newPosition = event->globalPos();
    newPosition = viewlbl->mapFromGlobal(newPosition);

    rubberBand->setGeometry(QRect(dragPosition, newPosition).normalized());
}

void PdfWidget::mouseReleaseEvent(QMouseEvent * event)
{
    if (!doc)
        return;

    if (!rubberBand->size().isEmpty()) {
        // Correct for the margin around the image in the label.
        selectedRect = QRectF(rubberBand->pos(), rubberBand->size());
        selectedRect.moveLeft(selectedRect.left() - (viewlbl->width() - viewlbl->pixmap()->width()) / 2.0);
        selectedRect.moveTop(selectedRect.top() - (viewlbl->height() - viewlbl->pixmap()->height()) / 2.0);

        menu->setLayoutDirection(Qt::RightToLeft);
        menu->exec(event->globalPos());
    }

    rubberBand->hide();
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

    QImage image = pdfPage->renderToImage(scaleFactor * viewlbl->physicalDpiX(), scaleFactor * viewlbl->physicalDpiY());
    if (image.isNull()) {
        qDebug() << "Poppler::Page::renderToImage fail...";
        return;
    }

    if (!searchLocation.isEmpty()) {
        QRect highlightRect = matrix().mapRect(searchLocation).toRect();
        highlightRect.adjust(-1, -1, 1, 1);

        QPainter painter;
        painter.begin(&image);

        QColor c;
        c.setRgb(QColor("Yellow").rgb());
        c.setAlpha(130);

        QBrush  b(c);
        painter.fillRect(highlightRect, b);

        painter.end();
    }

    viewlbl->setPixmap(QPixmap::fromImage(image));
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
                return searchLocation;
            }
        }
        page += 1;
    }

    return QRectF();
}

void PdfWidget::copyText()
{
    QRectF textRect = matrix().inverted().mapRect(selectedRect);

    QString text = "";

    bool hadSpace = false;
    QPointF center;
    foreach (Poppler::TextBox *box, doc->page(current_page)->textList()) {
        if (textRect.intersects(box->boundingBox())) {
            if (hadSpace)
                text += " ";
            if (!text.isEmpty() && box->boundingBox().top() > center.y())
                text += "\n";
            text += box->text();
            hadSpace = box->hasSpaceAfter();
            center = box->boundingBox().center();
        }
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

#endif
