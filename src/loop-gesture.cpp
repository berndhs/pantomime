#include "loop-gesture.h"
#include <QDebug>

namespace geuzen
{
LoopGesture::LoopGesture (QObject *parent)
  :QGesture (parent)
{
  sequences.clear();
  qDebug () << __PRETTY_FUNCTION__;
}

Qt::GestureState
LoopGesture::state () const
{
  qDebug () << __PRETTY_FUNCTION__;
  return QGesture::state();
}

Qt::GestureType
LoopGesture::gestureType () const
{
  qDebug () << __PRETTY_FUNCTION__;
  return Qt::CustomGesture;
}

void
LoopGesture::reset ()
{
  qDebug () << __PRETTY_FUNCTION__;
  sequences.clear();
  triggered = false;
  finished = false;
  started = false;
}

} // namespace
