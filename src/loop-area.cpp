#include "loop-area.h"
#include <QTimer>
#include <QTouchEvent>

#define GEUZEN_PRETTY_DEBUG qDebug() << "__geuzen__" << __PRETTY_FUNCTION__

namespace geuzen
{
LoopArea::LoopArea (QDeclarativeItem *parent)
  :QDeclarativeItem (parent),
   currentGesture ()
{
  circle.clear();
  circle << 0x0101 << 0x01ff << 0xffff << 0xff01;
  circleLen = circle.length();
  setAcceptTouchEvents(true);
}

bool
LoopArea::sceneEvent (QEvent * evt)
{
  GEUZEN_PRETTY_DEBUG << evt << evt->type();
  QTouchEvent * tev = static_cast<QTouchEvent*> (evt);
  QEvent::Type  tipo = evt->type();
  switch (tipo) {
  case QEvent::TouchBegin:
    if (tev->touchPoints().count() != 1) {
      break;
    }
    handleTouchMove (currentGesture, tev->touchPoints().at(0).lastScreenPos());
    currentGesture.reset ();
    return true;
    break;
  case QEvent::TouchUpdate:
    if (tev->touchPoints().count() != 1) {
      break;
    }
    handleTouchMove (currentGesture, tev->touchPoints().at(0).lastScreenPos());
    return true;
    break;
  case QEvent::TouchEnd:
    currentGesture.reset ();
    return true;
    break;
  default:
    break;
  }
  return QDeclarativeItem::sceneEvent (evt);
}

void
LoopArea::recognizerFoundLoop()
{
  emit looped();
}

void
LoopArea::handleTouchMove (LoopGesture & gesture, const QPointF &  screenPoint)
{
  QPointF last = gesture.lastPos;
  gesture.lastPos = screenPoint;
  if (!gesture.haveStarted) {
    gesture.reset ();
    gesture.haveStarted = true;
    gesture.setHotSpot (screenPoint);
    GEUZEN_PRETTY_DEBUG << " ____maybbe " << gesture.sequence;
    return;
  }
  quint8 leftRight (0);
  quint8 upDown (0);
 
  leftRight = last.x() < screenPoint.x() ? 1
                : last.x() > screenPoint.x() ? 0xff : 0;
  upDown = last.y() < screenPoint.y() ? 1
                : last.y() > screenPoint.y() ? 0xff : 0;
  quint32 segment = (upDown << 8) | leftRight;
  qDebug () << hex << " new seg " << segment << dec;
  if (leftRight != 0 && upDown != 0) {
    if (gesture.sequence.isEmpty()
        || gesture.sequence.last() != segment) {
     gesture.sequence.append (segment);
    }
  }
  qDebug () << hex << gesture.sequence << dec;
  if (chopTailCircle(gesture.sequence)) {
    qDebug () << " detected CIRCLE";
    GEUZEN_PRETTY_DEBUG << " fer SURE " << gesture.sequence;
    gesture.sequence.clear();
    
    recognizerFoundLoop();
    
  }
  
}

bool
LoopArea::chopTailCircle (LoopSegmentList & sequence)
{
  if (sequence.length() < circleLen) {
    return false;
  }
  bool found(true);
  int start = sequence.length() - circleLen;
  for (int matchPoint=0; matchPoint<circleLen; matchPoint++) {
    int i(0);
    int p(0);
    for (i=0, p=matchPoint; i<circleLen; i++) {
      p+= 1;
      if (p >= circleLen) { 
        p = 0;
      }
      if (sequence[start+i] != circle[p]) {
        found = false;
        break;
      }
    }
    if (found) {
      GEUZEN_PRETTY_DEBUG << " found circle";
      for (int end=0;end<circleLen;end++) {
        qDebug () << "    chop " << sequence.last();
        sequence.removeLast();
      }
      return true;
    }
  }
  return false;
}


#undef GEUZEN_PRETTY_DEBUG

} // namespace
