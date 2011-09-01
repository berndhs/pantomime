#ifndef GEUZEN_LOOP_GESTURE_H
#define GEUZEN_LOOP_GESTURE_H

#include <QGesture>
#include <QObject>
#include <QPointF>
#include <QDeclarativeItem>
#include <QList>

namespace geuzen
{
  
  
typedef QList <quint32>  LoopSegmentList;

class LoopGesture : public QGesture
{
Q_OBJECT
public:
  LoopGesture (QObject * parent = 0);

  Qt::GestureState state () const;

  Qt::GestureType gestureType () const;

  void reset ();

private:

  QPointF         lastPos;
  LoopSegmentList sequences;
  
  bool started;
  bool triggered;
  bool finished;
  
  
friend class LoopRecognizer;

};

} // namespace


#endif
