#include "loop-gesture.h"
#include <QDebug>

namespace geuzen
{
LoopGesture::LoopGesture ()
  :haveStarted (false),
   triggered (false)
{
  qDebug () << __PRETTY_FUNCTION__;
  sequence.clear();
}

void
LoopGesture::reset ()
{
  qDebug () << __PRETTY_FUNCTION__;
  sequence.clear();
  haveStarted = false;
  triggered = false;
}

QPointF
LoopGesture::hotSpot () const
{
  return theHotSpot;
}

void
LoopGesture::setHotSpot(const QPointF hotSpot)
{
  theHotSpot = hotSpot;
}

} // namespace
