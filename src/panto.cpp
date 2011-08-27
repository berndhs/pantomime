#include "panto.h"

#include <QTimer>

Panto::Panto(QWidget *parent)
  :QDeclarativeView(parent)
{
}

void
Panto::start ()
{    

  showFullScreen ();
  setSource (QUrl("qrc:/qml/main.qml"));
  show ();

  //QTimer::singleShot (10*1000, this, SLOT(allDone()));
}


void
Panto::allDone ()
{
  emit quit();
}
