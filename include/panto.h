#ifndef PANTO_H
#define PANTO_H

#include <QDeclarativeView>
#include <QDeclarativeItem>
#include <QMap>

#include "loop-recognizer.h"


class Panto : public QDeclarativeView
{
Q_OBJECT
public:
  explicit Panto(QWidget *parent = 0);
  
  void start ();

signals:

  void quit ();
  
protected:

  bool eventFilter(QObject *watched, QEvent * evt);
  bool event (QEvent *evt);  

public slots:

  void allDone ();
  
private:

  bool handleLoopGesture (geuzen::LoopGesture * gesture, QObject * target);
  
  geuzen::LoopRecognizer              looper;
  Qt::GestureType                     loopType;
  QDeclarativeItem                   *bottom;

};

#endif // PANTO_H
