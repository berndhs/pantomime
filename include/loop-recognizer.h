#ifndef GEUZEN_LOOP_REGOGNIZER_H
#define GEUZEN_LOOP_REGOGNIZER_H

#include <QEvent>
#include <QObject>
#include <QGestureRecognizer>
#include <QDeclarativeItem>
#include <QGraphicsSceneMouseEvent>
#include <QPointF>
#include "loop-gesture.h"

namespace geuzen
{

class LoopRecognizer : public QGestureRecognizer
{
public:

  LoopRecognizer ();

  QGesture * create (QObject *target);

  Result recognize (QGesture * gesture, QObject *watched, QEvent * evt);
  void reset (QGesture * gesture);
  
private:
  
  Result handleCursorMove (QGesture * gesture, QPointF scenePoint);
  bool chopTailCircle (LoopSegmentList & sequence);
  
  LoopSegmentList  circle;
  int          circleLen;


};

} // namespace

#endif
