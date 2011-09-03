#include "loop-recognizer.h"
#include "loop-gesture.h"
#include "loop-area.h"
#include <QDebug>
#include <QGestureEvent>
#include <QTouchEvent>

#define GEUZEN_PRETTY_DEBUG qDebug() <<  "__geuzen__" << __PRETTY_FUNCTION__

namespace geuzen
{
LoopRecognizer::LoopRecognizer (LoopArea * userItem)
  :QGestureRecognizer(),
   user (userItem),
   myType (Qt::GestureType(0))
{
  circle.clear();
  circle << 0x0101 << 0x01ff << 0xffff << 0xff01;
  circleLen = circle.length();
  GEUZEN_PRETTY_DEBUG;
  qDebug () << " circle is " << hex << circle << dec;
}

QGesture*
LoopRecognizer::create (QObject *target)
{
  GEUZEN_PRETTY_DEBUG;
  return new LoopGesture (int(myType), target);
}

QGestureRecognizer::Result
LoopRecognizer::recognize (QGesture * gesture, QObject *watched, QEvent *evt)
{ 
  geuzen::LoopGesture * leGo = static_cast<geuzen::LoopGesture*> (gesture);
  if (!leGo || leGo->tag() != myType) {
    return QGestureRecognizer::Ignore;
  }
  QEvent::Type tipo = evt->type();
  qDebug () << " event type " << tipo << "    leGo " << leGo << " tag " << myType 
            <<  " state " << leGo->state();
  qDebug () << "         flags st " << leGo->haveStarted << " tr " << leGo->triggered;
  
  QGestureRecognizer::Result result (QGestureRecognizer::Ignore);
  QTouchEvent         * tev  = static_cast<QTouchEvent*> (evt);
  QMouseEvent         * mev  = static_cast<QMouseEvent*> (evt);
  QGraphicsSceneMouseEvent *gev = static_cast<QGraphicsSceneMouseEvent*> (evt);

  switch (tipo) {
  case QEvent::Gesture:
    result = QGestureRecognizer::Ignore;
    break;
  case QEvent::GraphicsSceneMouseMove:
    qDebug () << " GMGM gfx mouse move " << gev;
    result = handleCursorMove (gesture,gev->scenePos());
    break;
  case QEvent::MouseMove:
    qDebug () << " MMMM mouse move " << mev;
    result = handleCursorMove (gesture,QPointF(mev->globalPos()));
    break;
  case QEvent::TouchBegin:
    qDebug () << " TTTT touch begin " << tev;
    leGo->reset();
    result = handleCursorMove (gesture,tev->touchPoints().at(0).scenePos());
    break;
  case QEvent::TouchUpdate:
    qDebug() << " TTTT touch update " << tev;
    result = handleCursorMove (gesture,tev->touchPoints().at(0).scenePos());
    break;
  case QEvent::TouchEnd:
    qDebug() << " TTTT touch end " << tev;
    result = handleCursorMove (gesture,tev->touchPoints().at(0).scenePos());
    break;
  case QEvent::Leave:
    qDebug () << " MMMM leave " << mev;
    result = handleCursorMove (gesture,QPointF(mev->globalPos()));
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
  //return result;
  qDebug () << "result " << result << " but ignoring anyway";
 
  return QGestureRecognizer::Ignore;
}

void
LoopRecognizer::reset (QGesture *gesture)
{
  GEUZEN_PRETTY_DEBUG;
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
  GEUZEN_PRETTY_DEBUG << gesture << scenePoint;
  if (gesture == 0) {
    return QGestureRecognizer::Ignore;
  }
  LoopGesture * loGe = qobject_cast<LoopGesture*>(gesture);
  if (!loGe) {
    qDebug () << " wrong gesture " << gesture;
    return QGestureRecognizer::Ignore;
  }
  if (!(user && user->contains(user->mapFromScene(scenePoint)))) {
      return QGestureRecognizer::Ignore; // not our point
  }
  QPointF last = loGe->lastPos;
  loGe->lastPos = scenePoint;
  if (!loGe->haveStarted) {
    loGe->reset ();
    loGe->haveStarted = true;
    loGe->setHotSpot (scenePoint);
    GEUZEN_PRETTY_DEBUG << " ____maybbe " << gesture << gesture->state();
    return QGestureRecognizer::Ignore;
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
    GEUZEN_PRETTY_DEBUG << " fer SURE " << gesture << gesture->state();
    
    notifyUser();
    
  }
  return QGestureRecognizer::Ignore;
}

void
LoopRecognizer::notifyUser()
{
  if (user) {
    user->recognizerFoundLoop();
  }
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
