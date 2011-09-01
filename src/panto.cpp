#include "panto.h"
#include "loop-recognizer.h"
#include <QGestureEvent>

#include <QTimer>

Panto::Panto(QWidget *parent)
  :QDeclarativeView(parent),
   looper (),
   bottom (0)
{
  loopType = QGestureRecognizer::registerRecognizer (&looper);
  grabGesture(loopType);
  qDebug () << __PRETTY_FUNCTION__ << " recognizer type " << hex << loopType << dec;
}

void
Panto::start ()
{    

  showFullScreen ();
  setSource (QUrl("qrc:/qml/main.qml"));
  show ();
  
  QDeclarativeItem * root = qobject_cast <QDeclarativeItem*> (rootObject());
  qDebug () << __PRETTY_FUNCTION__ << " root " << root;
  if (root) {
    #if 1
    QDeclarativeItem * mouseArea = root->findChild<QDeclarativeItem*> ("GestureTrap");
    if (mouseArea) {
      mouseArea->grabGesture (loopType);
      mouseArea->installEventFilter (this);
    }
    #endif
    QDeclarativeItem * bigButton = root->findChild<QDeclarativeItem*> ("BigButton");
    if (bigButton) {
      bigButton->grabGesture (loopType);
      bigButton->installEventFilter (this);
    }
  }

  //QTimer::singleShot (10*1000, this, SLOT(allDone()));
}

bool
Panto::eventFilter(QObject * watched, QEvent *evt)
{
  qDebug () << __PRETTY_FUNCTION__ <<  " Panto filter " << watched << " event " << evt;
  bool weHandledIt (false);
  if (evt) {
    QEvent::Type tipo = evt->type ();
    if (tipo ==QEvent::Gesture ) {
      qDebug () << __PRETTY_FUNCTION__ << "GESTURE event for " << watched ;
      QGestureEvent * gev = dynamic_cast <QGestureEvent*> (evt);
      geuzen::LoopGesture * loGe = qobject_cast<geuzen::LoopGesture*>(gev->gesture(loopType));
      if (loGe) {
        bool done = handleLoopGesture (loGe,watched);
        gev->setAccepted (done);
        return done;
      }
      if (gev) {
         qDebug () << " gev " << gev;
         return true;
      }
    } else if (tipo == QEvent::GestureOverride) {
      QGestureEvent * gev = dynamic_cast <QGestureEvent*> (evt);
      geuzen::LoopGesture * loGe = qobject_cast<geuzen::LoopGesture*>(gev->gesture(loopType));
      if (loGe) {
        qDebug() << "    gesture " << loGe << " state " <<  loGe->state();
      }
      if (gev) {
         qDebug ()<< __PRETTY_FUNCTION__  << " override what? gev " << gev;
      }
      qDebug () << "          override event " << evt << " for " << watched;
      if (loGe) {
        bool done = handleLoopGesture (loGe,watched);
        gev->setAccepted (done);
        return done;
      }
      QVariant retVar;
      QMetaObject::invokeMethod(watched,"handleLoopGesture",
         Q_RETURN_ARG (QVariant,retVar));
      qDebug () << "          object says " << retVar;
      if (retVar.isValid()) {
        bool finished = retVar.toBool();
        if (finished) {
          loGe->reset();
        }
        return finished;
      } else {
        return false;
      }
    }
 
  }
  return weHandledIt;
}

bool
Panto::handleLoopGesture (geuzen::LoopGesture * gesture, QObject * target)
{
  QVariant retVar;
  QMetaObject::invokeMethod (target,"handleLoopGesture",
    Q_RETURN_ARG (QVariant, retVar));
  if (retVar.isValid()) {
    bool done = retVar.toBool();
    if (done) {
      gesture->reset();
      return true;
    }
  }
  return false;
}
  
bool
Panto::event (QEvent *evt)
{
  qDebug () << __PRETTY_FUNCTION__ << evt;
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
  qDebug () << __PRETTY_FUNCTION__ << " returning " << handled;
  return handled;
}
 


void
Panto::allDone ()
{
  emit quit();
}
