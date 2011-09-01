#include "loop-recognizer.h"
#include "loop-gesture.h"
#include <QDebug>
#include <QGestureEvent>
#include <QTouchEvent>

#define GEUZEN_PRETTY_FUNCTION  "__geuzen__" << __PRETTY_FUNCTION__
namespace geuzen
{
LoopRecognizer::LoopRecognizer ()
  :QGestureRecognizer()
{
  circle.clear();
  circle << 0x0101 << 0x01ff << 0xffff << 0xff01;
  circleLen = circle.length();
  qDebug () << GEUZEN_PRETTY_FUNCTION;
  qDebug () << " circle is " << hex << circle << dec;
}

QGesture*
LoopRecognizer::create (QObject *target)
{
  qDebug () << GEUZEN_PRETTY_FUNCTION;
  return new LoopGesture (target);
}

QGestureRecognizer::Result
LoopRecognizer::recognize (QGesture * gesture, QObject *watched, QEvent *evt)
{ 
qDebug () << GEUZEN_PRETTY_FUNCTION << gesture << watched << evt;
  geuzen::LoopGesture * leGo = static_cast<geuzen::LoopGesture*> (gesture);
  if (!leGo) {
    return QGestureRecognizer::Ignore;
  }
  qDebug () << "    leGo " << leGo <<  " state " << leGo->state();
  qDebug () << "         flags st " << leGo->haveStarted << " tr " << leGo->triggered
                                    << " fin " << leGo->finished;
  QGestureRecognizer::Result result (QGestureRecognizer::Ignore);
  QEvent::Type tipo = evt->type();
  QTouchEvent         * tev  = static_cast<QTouchEvent*> (evt);
  QMouseEvent         * mev  = static_cast<QMouseEvent*> (evt);
  QGraphicsSceneMouseEvent *gev = static_cast<QGraphicsSceneMouseEvent*> (evt);
  QGestureEvent       * gestev = static_cast<QGestureEvent*> (evt);
  QList<QGesture*> childGestures;

  switch (tipo) {
  case QEvent::Gesture:
    qDebug () << GEUZEN_PRETTY_FUNCTION 
             << " Gesture event " << evt->type();
    qDebug() << "  gesture " << gesture;
    qDebug() << "  list " << dynamic_cast<QGestureEvent*>(evt)->gestures();
    childGestures = gestev->gestures();
    if (childGestures.contains (leGo)) {
      if (leGo->triggered) {
        result = QGestureRecognizer::FinishGesture;
      } 
    } else {
      result = QGestureRecognizer::Ignore | QGestureRecognizer::ConsumeEventHint;
    }
    break;
  case QEvent::GraphicsSceneMouseMove:
    result = handleCursorMove (gesture,gev->scenePos());
    break;
  case QEvent::MouseMove:
    qDebug () << " mouse move " << mev;
    result = handleCursorMove (gesture,QPointF(mev->globalPos()));
    break;
  case QEvent::TouchBegin:
    qDebug () << " touch begin " << tev;
    leGo->reset();
    result = handleCursorMove (gesture,tev->touchPoints().at(0).scenePos());
    break;
  case QEvent::TouchUpdate:
    qDebug() << " touch update " << tev;
    result = handleCursorMove (gesture,tev->touchPoints().at(0).scenePos());
    break;
  case QEvent::TouchEnd:
  case QEvent::Leave:
    qDebug() << " touch end " << tev;
    if (leGo->triggered) {
      leGo->finished = true;
      result = QGestureRecognizer::FinishGesture;
    } else {
      result = QGestureRecognizer::CancelGesture;
    }
    break;
  case QEvent::Move:
  case QEvent::GraphicsSceneMove:
    leGo->reset();
    result = QGestureRecognizer::CancelGesture;
    break;
  default:
    qDebug() << " other event " << tipo;
    break;
  }
  return result;
}

void
LoopRecognizer::reset (QGesture *gesture)
{
  qDebug () << GEUZEN_PRETTY_FUNCTION;
  LoopGesture *cg = qobject_cast <LoopGesture*>(gesture);
  if (cg) {
    cg->reset();
  } else if (gesture) {
    QGestureRecognizer::reset (gesture);
  }
}

QGestureRecognizer::Result
LoopRecognizer::handleCursorMove (QGesture *gesture,
                                  QPointF scenePoint)
{
  qDebug () << GEUZEN_PRETTY_FUNCTION << gesture << scenePoint;
  if (gesture == 0) {
    return QGestureRecognizer::Ignore;
  }
  LoopGesture * loGe = qobject_cast<LoopGesture*>(gesture);
  if (!loGe) {
    qDebug () << " wrong gesture " << gesture;
    return QGestureRecognizer::Ignore;
  }
  QPointF last = loGe->lastPos;
  loGe->lastPos = scenePoint;
  if (!loGe->haveStarted) {
    loGe->haveStarted = 2;
    loGe->finished = false;
    loGe->triggered = false;
    loGe->setHotSpot (scenePoint);
    qDebug () << GEUZEN_PRETTY_FUNCTION << " ____maybbe " << gesture << gesture->state();
    return QGestureRecognizer::MayBeGesture;
  }
  quint8 leftRight (0);
  quint8 upDown (0);
  leftRight = last.x() < scenePoint.x() ? 1
                : last.x() > scenePoint.x() ? 0xff : 0;
  upDown = last.y() < scenePoint.y() ? 0x1
                : last.y() > scenePoint.y() ? 0xff : 0;
  quint32 segment = (upDown << 8) | leftRight;
  qDebug () << hex << " new seg " << segment << dec;
  if (leftRight != 0 && upDown != 0) {
    if (loGe->sequences.isEmpty()
        || loGe->sequences.last() != segment) {
     loGe-> sequences.append (segment);
    }
  }
  qDebug () << hex << loGe->sequences << dec;
  if (chopTailCircle(loGe->sequences)) {
    qDebug () << " detected CIRCLE";
    loGe->sequences.clear();
    qDebug () << GEUZEN_PRETTY_FUNCTION << " fer SURE " << gesture << gesture->state();
    if (!loGe->triggered) {
      loGe->triggered = true;
      loGe->finished = false;
      qDebug () << "            ___triggered";
      return QGestureRecognizer::TriggerGesture;
    }
  }
  //qDebug () << " sequence " << hex  << loGe->sequences[watched] << dec;
  return QGestureRecognizer::Ignore;
}

bool
LoopRecognizer::chopTailCircle (LoopSegmentList & sequence)
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
      qDebug () << GEUZEN_PRETTY_FUNCTION << " found circle";
      for (int end=0;end<circleLen;end++) {
        qDebug () << "    chop " << sequence.last();
        sequence.removeLast();
      }
      return true;
    }
  }
  return false;
}
#undef GEUZEN_PRETTY_FUNCTION

  
} // namespace
