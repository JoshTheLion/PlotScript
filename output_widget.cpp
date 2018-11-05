#include "output_widget.hpp"

#include <QWidget>
#include <QDebug>
#include <QLayout>

#include <QGraphicsWidget>
#include <QGraphicsView>
#include <QGraphicsScene>

#include <QPainter>
#include <QBrush>
#include <QPen>

#include <QPointF>
#include <QRectF>

#include <QGraphicsItem>
#include <QGraphicsTextItem>
#include <QGraphicsEllipseItem>

OutputWidget::OutputWidget(QWidget * parent){
	
	setObjectName("output");
    
    m_scene = new QGraphicsScene(parent);
    m_scene->setObjectName("myScene");

    m_view = new QGraphicsView(m_scene, parent);
    m_view->setObjectName("myView");

    auto layout = new QGridLayout;

    layout->addWidget(m_view, 0, 0);
    
    this->setLayout(layout);
}

void OutputWidget::getResult(Settings result){
  
  // Remove old graphic item first
  m_scene->clear();
  m_item = nullptr;
  
  // Display new graphic item
  if(result.itemType != Settings::Type::None_Type){
    drawItem(result);
  }
}

void OutputWidget::drawItem(Settings data){

  qDebug() << "Item Settings Data:";
  qDebug() << "Type: " << data.itemType;
  qDebug() << "Pos: " << data.pos;
  
  // Setup painter tools
  //QPen pen;
  //pen.setColor(Qt::black);
  //pen.setBrush(Qt::SolidPattern);
  
  Settings::Type name = data.itemType;//QString label = item->data(ObjectType).toString();
  
  switch(name){
    case Settings::Type::Text_Type:
      //QGraphicsTextItem *QGraphicsScene::addText(const QString &text, const QFont &font = QFont())
      m_item = m_scene->addText(data.text);
    
      //void QGraphicsItem::setPos(const QPointF &pos)
      m_item->setPos(data.pos);
      //m_item->setPos(50, 50);

      qDebug() << "Text Data:";
      qDebug() << "Rect Corner: " << m_item->sceneBoundingRect().topLeft() << m_item->childrenBoundingRect().topLeft();
      qDebug() << "Item Pos: " << m_item->pos();
      qDebug() << "Scene Pos: " << m_item->scenePos();
      qDebug() << "View Pos: " << m_view->itemAt(0, 0) << m_view->itemAt(data.pos);
      break;

    case Settings::Type::Point_Type:
      //QRectF(qreal x, qreal y, qreal width, qreal height)
      QRectF frame(0, 0, data.size, data.size);

      //void QRectF::moveCenter(const QPointF &position)
      frame.moveCenter(data.pos);

      //QGraphicsEllipseItem *QGraphicsScene::addEllipse(const QRectF &rect, const QPen &pen = QPen(), const QBrush &brush = QBrush())
      m_item = m_scene->addEllipse(frame, QPen(Qt::black), QBrush(Qt::black));
      //m_item->sceneBoundingRect().moveCenter(data.pos);

      qDebug() << "Ellipse Data:";
      qDebug() << "Rect Center: " << frame.center();
      qDebug() << m_item->sceneBoundingRect().center() << m_item->childrenBoundingRect().center();
      qDebug() << "Item Pos: " << m_item->pos();
      qDebug() << "Scene Pos: " << m_item->scenePos();
      qDebug() << "View Pos: " << m_view->itemAt(0, 0) << m_view->itemAt(data.pos);
      break;

    //case Settings::Type::Line_Type:
      // TO-DO:
      //pen.setWidth(thickness);
      //scene->addLine(x1, y1, x2, y2, pen);
      //break;
  }
}


