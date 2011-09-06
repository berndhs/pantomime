#include "loop-gesture.h"
#include <QDeclarativeItem>
#include <QPointF>

namespace geuzen
{

class LoopArea : public QDeclarativeItem {
Q_OBJECT
public:

  LoopArea (QDeclarativeItem *parent=0);

private slots:
  
  void recognizerFoundLoop ();

signals:

  void looped ();

protected:

  bool sceneEvent (QEvent * evt);
  
private:
 
  void handleTouchMove (LoopGesture & gesture, const QPointF & screenPoint);
  bool chopTailCircle (LoopSegmentList & sequence);
  
  LoopGesture         currentGesture;

  LoopSegmentList  circle;
  int              circleLen;
  
};

} // namespace
