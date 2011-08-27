#ifndef PANTO_H
#define PANTO_H

#include <QDeclarativeView>

class Panto : public QDeclarativeView
{
Q_OBJECT
public:
  explicit Panto(QWidget *parent = 0);
  
  void start ();

signals:

  void quit ();

public slots:

  void allDone ();

};

#endif // PANTO_H
