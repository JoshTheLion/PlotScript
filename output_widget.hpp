#ifndef OUTPUT_WIDGET_H
#define OUTPUT_WIDGET_H

#include "interpreter.hpp"  // Access to PlotScript

#include <QWidget>
#include <QGraphicsView>
#include <QGraphicsScene>

//#include <QGraphicsGridLayout>
//#include <QGraphicsLayout>
//#include <QGraphicsLayoutItem>
#include <QLayout>

// Forward declare to allow pointer member variables & save compile time
class QGraphicsItem;
class QGraphicsTextItem;
class QGraphicsWidget;

class OutputWidget: public QWidget
{
Q_OBJECT

public:
	OutputWidget(QWidget * parent = nullptr);
	
public slots:
	//void getResult(QString resultExp);
    //void getResult(Expression resultExp);
    void getItem(QGraphicsItem * item);

private:
    QGraphicsScene * frame;
    QGraphicsView * image;
    // QGraphicsGridLayout * layout;
    
    QGraphicsItem * m_item; // Points to graphics object for data
    
    // The data tag key for different object type names
    static const int ObjectType = 0;

    void drawItem(QGraphicsItem * item);
    //void drawItem(QString item);
    //void drawItem(Expression exp);
};
#endif // OUTPUT_WIDGET_H
