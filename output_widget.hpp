#ifndef OUTPUT_WIDGET_H
#define OUTPUT_WIDGET_H

#include "settings.h" // Access to result settings struct

#include <QWidget>
#include <QLayout>
#include <QGraphicsView>
#include <QGraphicsScene>
#include <QGraphicsItem>

class OutputWidget : public QWidget {
  Q_OBJECT
  
  public:
    
    OutputWidget(QWidget * parent = nullptr);
    
  public slots:
    
    void getResult(Settings result);
    
  private:
    
    // Composite objects
    QGraphicsScene * m_scene;
    QGraphicsView * m_view;
    QGraphicsItem * m_item;
    
    void drawItem(Settings data);
};

#endif // OUTPUT_WIDGET_H
