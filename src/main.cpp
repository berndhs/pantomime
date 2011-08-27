#include <QApplication>
#include <QDeclarativeEngine>
#include <QDesktopWidget>

#include <QDebug>

#include "panto.h"


int
main (int argc, char*argv[])
{
  QApplication app (argc, argv);
  
  Panto panto;
  QDeclarativeEngine *engine = panto.engine();
  
  QObject::connect (engine, SIGNAL (quit()),&app, SLOT (quit()));
  QObject::connect (&panto, SIGNAL (quit()), &app, SLOT (quit()));
  
  panto.setResizeMode (QDeclarativeView::SizeRootObjectToView);
  panto.setGeometry (app.desktop()->screenGeometry());
  panto.start ();
  
  app.exec ();
  
  return 0;
}

#if USE_BIT_LY
d4c0e61051d99ba4f9547383db2c641c
berndhs
#endif
