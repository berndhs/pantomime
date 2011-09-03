#ifndef PANTO_H
#define PANTO_H

#include <QDeclarativeView>
#include <QDeclarativeItem>
#include <QSize>
#include <QMap>


class Panto : public QDeclarativeView
{
Q_OBJECT
public:
  explicit Panto(QWidget *parent = 0, bool isPhone = false);
  
  void start ();

  void setSize (int width, int height);

signals:

  void quit ();
  
protected:

  bool eventFilter(QObject *watched, QEvent * evt);
  bool event (QEvent *evt);  

public slots:

  void allDone ();
  
private:
  
  bool                                isProbablyPhone;
  bool                                sizeSet;
  QSize                               desiredSize;
  QDeclarativeItem                   *bottom;

};

#endif // PANTO_H
