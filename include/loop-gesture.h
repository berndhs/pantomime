#ifndef GEUZEN_LOOP_GESTURE_H
#define GEUZEN_LOOP_GESTURE_H

#include <QPointF>
#include <QList>

namespace geuzen
{
  
  
typedef QList <quint32>  LoopSegmentList;

class LoopGesture 
{
public:
  LoopGesture ();

  void reset ();
  QPointF hotSpot () const;
  void    setHotSpot (const QPointF hotSpot);
  
private:

  QPointF         lastPos;
  QPointF         theHotSpot;
  LoopSegmentList sequence;
  
  bool haveStarted;
  bool triggered;
  
friend class LoopArea;

};

} // namespace


#endif
