#include "loop-area.h"
#include <QTimer>

#define GEUZEN_PRETTY_DEBUG qDebug() << "__geuzen__" << __PRETTY_FUNCTION__

namespace geuzen
{
LoopArea::LoopArea (QDeclarativeItem *parent)
  :QDeclarativeItem (parent),
   recognizer (this),
   theWidth (0),
   theHeight (0)
{
  loopType = QGestureRecognizer::registerRecognizer (&recognizer);
  recognizer.setType(loopType);
  grabGesture(loopType);
  GEUZEN_PRETTY_DEBUG << " recognizer type " << hex << loopType << dec;
  qDebug () << " width " << width();
  connect (this, SIGNAL (widthChanged(int)), this, SLOT (adjustWidth(int)));
}

bool
LoopArea::sceneEvent (QEvent * evt)
{
  QEvent::Type  tipo = evt->type();
  if (tipo == QEvent::GraphicsSceneResize) {
    qDebug () << " resize gfx " << evt;
  } else if (tipo == QEvent::Resize) {
    qDebug () << " resize plain " << evt;
  }
  return QDeclarativeItem::sceneEvent (evt);
}

void
LoopArea::recognizerFoundLoop()
{
  emit looped();
}


#undef GEUZEN_PRETTY_DEBUG

} // namespace
