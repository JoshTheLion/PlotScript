#ifndef OUTPUT_WIDGET_H
#define OUTPUT_WIDGET_H

#include <QWidget>
#include <QGraphicsView>
#include <QGraphicsScene>

#include <QGraphicsGridLayout>
#include <QGraphicsLayout>
#include <QGraphicsLayoutItem>
#include <QLayout>

#include <QGraphicsItem>
#include <QGraphicsTextItem>
#include <QGraphicsWidget>


class OutputWidget: public QWidget
{
Q_OBJECT

public:
	OutputWidget(QWidget * parent = nullptr);
	
public slots:
	void getResult(QString resultExp);
	
private:
    QGraphicsScene * frame;
    QGraphicsView * image;
    QGraphicsGridLayout * layout;

    //QGraphicsItem * data; // Points to graphics object for data
    //enum GraphicType { TEXT, POINT, LINE };
    //GraphicType graphic = TEXT;
    QGraphicsTextItem * text;
    QGraphicsEllipseItem * point;
    QGraphicsLineItem * line;
	
};
#endif // OUTPUT_WIDGET_H
