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
  LoopGesture (int tag, QObject * parent = 0);

  Qt::GestureState state () const;

  Qt::GestureType gestureType () const;

  void reset ();
  
  int tag () { return myTag; }
  void setTag (int t) { myTag = t; }

private:

  QPointF         lastPos;
  LoopSegmentList sequence;
  
  bool haveStarted;
  bool triggered;
  
  int  myTag;
  
  
friend class LoopRecognizer;

};

} // namespace


#endif
