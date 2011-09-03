#include "panto.h"
#include <QGestureEvent>

#include <QTimer>

#define PANTO_PRETTY_FUNCTION "__panto__" << __PRETTY_FUNCTION__
Panto::Panto(QWidget *parent, bool isPhone)
  :QDeclarativeView(parent),
   isProbablyPhone (isPhone),
   sizeSet (false),
   desiredSize (QSize (4096,4096)),
   bottom (0)
{
}

void
Panto::setSize (int width, int height)
{
  desiredSize = QSize (width, height);
  sizeSet = true;
}

void
Panto::start ()
{    

  if (isProbablyPhone) {
    showFullScreen ();
  }
  setSource (QUrl("qrc:/qml/main.qml"));
  if (sizeSet) {
    resize (desiredSize);
  }
  show ();
  
  QDeclarativeItem * root = qobject_cast <QDeclarativeItem*> (rootObject());
  qDebug () << PANTO_PRETTY_FUNCTION << " root " << root;
  if (root) {
    #if 0
    QDeclarativeItem * mouseArea = root->findChild<QDeclarativeItem*> ("GestureTrap");
    if (mouseArea) {
      mouseArea->grabGesture (loopType);
      mouseArea->installEventFilter (this);
    }
    QDeclarativeItem * bigButton = root->findChild<QDeclarativeItem*> ("BigButton");
    if (bigButton) {
      bigButton->grabGesture (loopType);
      bigButton->installEventFilter (this);
    }
    #endif
  }

  //QTimer::singleShot (10*1000, this, SLOT(allDone()));
}

bool
Panto::eventFilter(QObject * watched, QEvent *evt)
{
  qDebug () << PANTO_PRETTY_FUNCTION <<  " Panto filter " << watched << " event " << evt;

  bool weHandledIt (false);
  
  return weHandledIt;
}

bool
Panto::event (QEvent *evt)
{
  //qDebug () << PANTO_PRETTY_FUNCTION << evt;
  if (evt) {
    QEvent::Type tipo = evt->type ();
    if (tipo ==QEvent::Gesture ) {
      qDebug () << "GESTURE event in " << this;
      QGestureEvent * gev = dynamic_cast <QGestureEvent*> (evt);
      if (gev) {
        qDebug () << " gev " << gev;
      }
      return true;
    } else if (tipo == QEvent::GestureOverride) {
      QGestureEvent * gev = dynamic_cast <QGestureEvent*> (evt);
      if (gev) {
        QDeclarativeItem * dItem = qobject_cast<QDeclarativeItem*>(gev->widget());
        qDebug () << " override what? gev " << gev << gev->widget() << dItem;
        if (dItem) {
          QVariant retVar;
          QMetaObject::invokeMethod (dItem,"handleLoopGesture",
             Q_RETURN_ARG(QVariant, retVar));
          if (retVar.isValid()) {
            return retVar.toBool();
          }
        }
      }
    }
  }
  bool handled = QDeclarativeView::event (evt);
  //qDebug () << PANTO_PRETTY_FUNCTION << " returning " << handled;
  return handled;
}
 


void
Panto::allDone ()
{
  emit quit();
}

#undef PANTO_PRETTY_FUNCTION
