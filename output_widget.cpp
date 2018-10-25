#include "output_widget.hpp"

//#include <QGraphicsView>
//#include <QGraphicsScene>
//#include <QLineEdit>
//#include <QDebug>
#include <QWidget>
#include <QDebug>
#include <QLayout>


// public:
OutputWidget::OutputWidget(QWidget * parent){
	
	image = new QGraphicsView;
	//QObject::connect();
	
	
	display = new QGraphicsScene;
	//QObject::connect();
	
	// Display
    auto layout = new QGridLayout();
    // layout->addWidget(widget, row, column)
    layout->addWidget(image, 0, 0);

    setLayout(layout);
}

//public slots:
void OutputWidget::showResult(QString res){
  //qDebug() << "Hello World!";
  qDebug() << res;
  result = res;
  
}
/*
void OutputWidget::resultEvaluated(Expression res){
	qDebug() << s;
	inputExp = s;
    qDebug() << "Hello World!";
}
*/

