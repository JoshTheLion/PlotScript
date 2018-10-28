#include "output_widget.hpp"

#include <QWidget>
#include <QDebug>
#include <QLayout>


OutputWidget::OutputWidget(QWidget * parent){
	
	setObjectName("output");
    
    frame = new QGraphicsScene(parent);
    frame->setObjectName("myScene");
    //data = new QGraphicsTextItem(expression); // Add new Text graphic to scene
    //text = frame->addText(expression);

    image = new QGraphicsView(frame, parent);
    image->setObjectName("myView");

    auto layout = new QGridLayout;

    layout->addWidget(image, 0, 0);
    
    this->setLayout(layout);
}

void OutputWidget::getResult(QString resultExp){
  
  qDebug() << "Result: " << resultExp << "\n";
  frame->clear();
  text = frame->addText(resultExp);
}


