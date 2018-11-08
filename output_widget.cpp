#include "output_widget.hpp"

#include <QWidget>
#include <QDebug>
#include <QLayout>

#include <QGraphicsWidget>
#include <QGraphicsView>
#include <QGraphicsScene>
#include <QFont>

#include <QPainter>
#include <QBrush>
#include <QPen>

#include <QPointF>
#include <QRectF>
#include <QLineF>

#include <QGraphicsItem>
#include <QGraphicsTextItem>
#include <QGraphicsEllipseItem>
#include <QGraphicsLineItem>

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
  m_view->centerOn(0, 0);

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
  QPen pen;
  pen.setColor(Qt::black);
  pen.setBrush(Qt::SolidPattern);
  QRectF box; // Had to declare this up here for some stupid "failed to initialize" reason

  Settings::Type name = data.itemType;//QString label = item->data(ObjectType).toString();

  switch (name) {
  case Settings::Type::TUI_Type:

    //QGraphicsTextItem *QGraphicsScene::addText(const QString &text, const QFont &font = QFont())
    m_item = m_scene->addText(data.text, QFont());
    m_view->centerOn(0, 0);

    break;

  case Settings::Type::Text_Type:

    //QGraphicsTextItem *QGraphicsScene::addText(const QString &text, const QFont &font = QFont())
    m_item = m_scene->addText(data.text, QFont());
    m_view->centerOn(0, 0);

    //void QGraphicsItem::setPos(const QPointF &pos)
    //m_item->moveBy(data.pos.x(), data.pos.y());
    m_item->setPos(data.pos);

    qDebug() << "Text Data: ";
    qDebug() << "Rect Corner: " << m_item->boundingRect().topLeft() << m_item->childrenBoundingRect().topLeft();
    qDebug() << "Item Pos: " << m_item->pos();
    qDebug() << "Scene Pos: " << m_item->scenePos();
    qDebug() << "View Pos: " << m_view->itemAt(0, 0) << m_view->itemAt(data.pos);
    break;

  case Settings::Type::Point_Type:

    //QRectF(qreal x, qreal y, qreal width, qreal height)
    box = QRectF(0.0, 0.0, data.size, data.size);

    // Center the rect at the origin of its local item coordinates
    //void QRectF::moveCenter(const QPointF &position)
    box.moveCenter(QPointF(0.0, 0.0));

    // Create an Ellipse bounded by the QRectF and add to scene
    //QGraphicsEllipseItem *QGraphicsScene::addEllipse(const QRectF &rect, const QPen &pen = QPen(), const QBrush &brush = QBrush())
    m_item = m_scene->addEllipse(box, pen, pen.brush());
    m_view->centerOn(0, 0);
    
    // Move item to specified location
    //void QGraphicsItem::prepareGeometryChange()
    //void QRectF::translate(qreal dx, qreal dy)
    //void QGraphicsItem::moveBy(qreal dx, qreal dy)
    //m_item->moveBy(data.pos.x(), data.pos.y());
    m_item->setPos(data.pos);

    qDebug() << "Ellipse Data: ";
    qDebug() << "Rect Center: " << m_item->boundingRect().center() << m_item->childrenBoundingRect().center();
    qDebug() << "Item Pos: " << m_item->pos();
    qDebug() << "Scene Pos: " << m_item->scenePos();
    qDebug() << "View Pos: " << m_view->itemAt(0, 0) << m_view->itemAt(data.pos);
    break;

  case Settings::Type::Line_Type:

    //pen.setWidth(thickness);
    //pen = QPen(Qt::black)
    pen.setWidth(data.thicc);

    //QGraphicsLineItem *QGraphicsScene::addLine(qreal x1, qreal y1, qreal x2, qreal y2, const QPen &pen = QPen())
    m_item = m_scene->addLine(QLineF(data.p1, data.p2), pen);
    //m_item = m_scene->addLine(QLineF(QPoint(0, 0), QPoint(50, 50)), pen);
    m_view->centerOn(0, 0);
    //m_item->setPos(data.pos);

    qDebug() << "Line Data: ";
    qDebug() << "Rect Center: " << m_item->boundingRect().center() << m_item->childrenBoundingRect().center();
    qDebug() << "Item Pos: " << m_item->pos();
    qDebug() << "Scene Pos: " << m_item->scenePos();
    qDebug() << "View Pos: " << m_view->itemAt(0, 0) << m_view->itemAt(data.pos);
    break;

  case Settings::Type::List_Type:

    qDebug() << "List Data: ";

    // Recursively display each entry using the rules above without any surrounding parenthesis.
    for(auto item : data.list){
      qDebug() << item.itemType << item.pos << item.text << item.size << item.thicc << item.p1 << item.p2 << "\n";
      drawItem(item);
    }
    break;

  default:

    qDebug() << "Stopped Recursion";
    break;
  }
}


