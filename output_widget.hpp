#ifndef OUTPUT_WIDGET_H
#define OUTPUT_WIDGET_H

#include <QWidget>
#include <QGraphicsView>
#include <QGraphicsScene>
#include <QGraphicsLayout>
#include <QLayout>
#include <QBoxLayout>

//class GraphicsView;
//class GraphicsScene;

class OutputWidget: public QWidget	// Extends QWidget base class
{
Q_OBJECT // Necessary macro in class definition

public:
	OutputWidget(QWidget * parent = nullptr);
	
public slots:
	//void resultEvaluated(Expression res); // Catch the result of evaluating input
	void showResult(QString res);

//private slots:
	
private:
	QGraphicsView * image;
	QGraphicsScene * display;
	
    QString result;
    //std::string result;
	
//signals: // Neither public nor private
	
};
#endif // OUTPUT_WIDGET_H
