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

class LoopArea;

class LoopRecognizer : public QGestureRecognizer
{
public:

  LoopRecognizer (LoopArea * userItem=0);

  QGesture * create (QObject *target);
  
  LoopArea * userItem () { return user; }
  void setUserItem (LoopArea *userItem) { user = userItem; }

  Result recognize (QGesture * gesture, QObject *watched, QEvent * evt);
  void reset (QGesture * gesture);
  
  void setType (Qt::GestureType t) { myType = int(t); }
  
  void notifyUser ();
  
  
signals:
  
  void foundLoop ();
  
private:
  
  Result handleCursorMove (QGesture * gesture, QPointF scenePoint);
  bool chopTailCircle (LoopSegmentList & sequence);
  
  LoopArea        *user;
  LoopSegmentList  circle;
  int              circleLen;
  int              myType;


};

} // namespace

#endif
