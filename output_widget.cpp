#include "output_widget.hpp"

#include <QWidget>
#include <QDebug>
#include <QLayout>

#include <QGraphicsWidget>
#include <QGraphicsView>
#include <QGraphicsScene>

#include <QGraphicsItem>
#include <QGraphicsTextItem>


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

void OutputWidget::getItem(QGraphicsItem * item){
  
  // Remove old graphic item first
  frame->clear();
  
  // Store pointer to new item (or null)
  m_item = item;
  
  // Display graphic
  if(item != nullptr){
    drawItem(item);
  }
  
  // Place new graphic item and store reference to it
  //drawItem(*m_item);
  //drawItem(resultExp);
}

void OutputWidget::drawItem(QGraphicsItem * item){
  
  // Sort and handle different item types
  if(item == nullptr){
    // Display nothing
    return;
  }

  // Get data label
  QString label = item->data(ObjectType).toString();
  if(label == "text"){
    frame->addItem(item);
    //qDebug() << QGraphicsScene::itemAt(0.0, 0.0);
  }
  else if(label == "Point Type"){
    frame->addItem(item);
  }
  else if (label == "Line Type") {
    frame->addItem(item);
  }
  else if (label == "List Type") {
    //frame->addItem(item);
  }
  else if(label == "TUI Type"){
    frame->addItem(item);
  }

}


