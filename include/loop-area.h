#include "loop-recognizer.h"


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

  LoopRecognizer      recognizer;

  Qt::GestureType     loopType;

  int     theWidth;
  int     theHeight;
  
  friend void LoopRecognizer::notifyUser();
};

} // namespace
