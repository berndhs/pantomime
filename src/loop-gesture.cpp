#include "loop-gesture.h"
#include <QDebug>

namespace geuzen
{
LoopGesture::LoopGesture (int tag, QObject *parent)
  :QGesture (parent),
   haveStarted (false),
   triggered (false),
   myTag (tag)
{
  sequence.clear();
  qDebug () << __PRETTY_FUNCTION__;
}

Qt::GestureState
LoopGesture::state () const
{
  return QGesture::state();
}

Qt::GestureType
LoopGesture::gestureType () const
{
  return Qt::CustomGesture;
}

void
LoopGesture::reset ()
{
  qDebug () << __PRETTY_FUNCTION__;
  sequence.clear();
  triggered = false;
  haveStarted = false;
}

} // namespace
