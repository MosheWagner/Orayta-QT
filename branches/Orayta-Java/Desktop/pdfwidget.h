/*
* This file is taken from the "poppler" exapmle from Qt's documantation,
* ( Copyright info here below), But has changes done me.
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

#ifndef PDF_H
#define PDF_H

#include "OraytaBase/functions.h"
#include <QtGui>
#include <QPainter>
#include <poppler-qt4.h>
#include <QLabel>
//#include <QRectF>

class QRubberBand;

class PdfWidget : public QScrollArea
{
    Q_OBJECT

public:
    PdfWidget(QWidget *parent = 0);
    ~PdfWidget();

    Poppler::Document *document();
    QMatrix matrix() const;
    qreal scale() const;

    int numPages() const;
    int currentPage() const;


    void DrawRects(QList <QRect> sel, QColor color);

    //Returns a list of QRects, marking of all text selected between the two given points
    // (RTL affects only multiple line selections.)
    QList <QRect> SelectText(QPoint p1, QPoint p2, bool RTL);

    //Returns a QLine representing the size and position
    //  of the text line that is at the given point.
    //
    // If the point is not in the middle of any line -
    //  a height of 4 pixels, and the position of the given point are returned.
    QLine TextLine(QPoint p);

public slots:
    QRectF searchBackwards(const QString &text);
    QRectF searchForwards(const QString &text);
    bool setDocument(const QString &filePath);
    void setPage(int page = -1);
    void setScale(qreal scale);


    //Copy the text in the selected rectangle to the clipboard
    void copyText();

    void sliderValueChanged(int);
    void sliderRangeChanged(int, int);

    //Goes to the next page of the book (if it exists);
    void nextPage();
    //Goes to the previous page of the book (if it exists);
    void previousPage();

    //Prints the given page to the given Qprinter
    //void printPage(QPrinter, int);


    /*
    void mPressEvent(QMouseEvent *event);
    void mMoveEvent(QMouseEvent *event);
    void mReleaseEvent(QMouseEvent *event);
    */

protected:
    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);

signals:
    void pageChanged(int currentPage, int max);

    //void textSelected(const QString &text);

private:
    QRectF selectedRect;
    QLabel * viewlbl;

    QAction *copy;
    QMenu *menu;

    QImage Image;

    QList <QRect> selected;
    QString selectedText;

    void showPage(int page = -1);

    Poppler::Document *doc;

    QPoint dragPosition;
    QRubberBand *rubberBand;
    QRectF searchLocation;
    qreal scaleFactor;
    int current_page;

    bool useRTL;

    int mSliderTop;
    bool mSTO;
    int mSliderBottem;
    bool mSBO;
};

#endif // PDF_H

#endif
