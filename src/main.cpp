#include <QApplication>
#include <QDeclarativeEngine>
#include <QDesktopWidget>
#include <QSystemDeviceInfo>

#include <QDebug>

#include "panto.h"
#include "loop-area.h"
#include "orientation.h"

QTM_USE_NAMESPACE

int
main (int argc, char*argv[])
{
  QApplication app (argc, argv);

  char staticUri[] = "moui.geuzen.utils.static";
  qmlRegisterType <geuzen::LoopArea> (staticUri,1,0,"GeuzenLoopArea");
  qmlRegisterType<geuzen::OrientationWatcher>(staticUri, 1, 0,
               "GeuzenOrientation");
  
  QSystemDeviceInfo sdi;

  QString imsi = sdi.imsi();
  QString imei = sdi.imei();
  bool isPhone (!(imsi.isEmpty() || imei.isEmpty()));
  qDebug () << __PRETTY_FUNCTION__ << " phone ? " << isPhone;

  Panto panto (0, isPhone);
  QDeclarativeEngine *engine = panto.engine();
  
  QObject::connect (engine, SIGNAL (quit()),&app, SLOT (quit()));
  QObject::connect (&panto, SIGNAL (quit()), &app, SLOT (quit()));
  
  panto.setResizeMode (QDeclarativeView::SizeRootObjectToView);
  if (isPhone) {
    panto.setGeometry (app.desktop()->screenGeometry());
  } else {
    panto.setSize (600,400);
    panto.setGeometry (QRect (0,0,600,400));
  }
  panto.start ();
  
  app.exec ();
  
  return 0;
}
