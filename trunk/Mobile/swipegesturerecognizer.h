#ifndef SWIPEGESTURERECOGNIZER_H
#define SWIPEGESTURERECOGNIZER_H

#include <QGestureRecognizer>
#include <QElapsedTimer>

class SwipeGestureRecognizer : public QGestureRecognizer
{
private:
   static const int MINIMUM_DISTANCE = 100;

   typedef QGestureRecognizer parent;

   bool IsValidMove(int dx, int dy);

   qreal ComputeAngle(int dx, int dy);

   virtual QGesture* create(QObject* pTarget);

   virtual QGestureRecognizer::Result recognize(QGesture* pGesture, QObject *pWatched, QEvent *pEvent);

   //A timer to make sure we don't get double events. It's a small hack to prevent an android/emulator bug.
   QElapsedTimer timer;

   void reset (QGesture *pGesture);
};

#endif // SWIPEGESTURERECOGNIZER_H
