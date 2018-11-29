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
    
    m_view->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    m_view->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    
    m_layout = new QGridLayout;

    m_layout->addWidget(m_view, 0, 0);
    m_view->centerOn(0, 0);
    
    this->setLayout(m_layout);
    this->show();
}

void OutputWidget::getResult(Settings result){
  
  // Remove old graphic item first
  m_scene->clear();
  m_item = nullptr;
  
  // Draw cosmetic lines at scene origin for visual reference
  //drawCrosshairs();

  // Display new graphic item
  if(result.itemType != Settings::Type::None_Type){
    drawItem(result);
  }

	// Manually re-focus the view
	m_view->fitInView(m_scene->itemsBoundingRect(), Qt::KeepAspectRatio);
}

void OutputWidget::drawItem(Settings data){

  qDebug() << "Item Settings Data:";
  qDebug() << "Type: " << data.itemType;
  qDebug() << "Pos: " << data.pos;
  
  // Setup painter tools
  QPen pen;
  pen.setColor(Qt::black);
  pen.setBrush(Qt::SolidPattern);
	pen.setWidth(0); // How did adding this single line fix everything???
	
	// Setup specified font settings
	auto font = QFont("Monospace");
	font.setStyleHint(QFont::TypeWriter);
	font.setPointSize(1);

  // Compiler forced me to declare these up here for some reason
  QRectF bRect;
  double dx;
  double dy;

  Settings::Type name = data.itemType;

  switch (name) {
  case Settings::Type::TUI_Type:

    m_item = m_scene->addText(data.text, font);

    break;

  case Settings::Type::Text_Type:
    
    // Initialize Text item
    //m_item = m_scene->addText(data.text, QFont("Courier", 1, 1, false));
		m_item = m_scene->addText(data.text, font);
    m_item->setScale(data.scale);
    
    // Translate item center-point to scene origin
    dx = 0 - (m_item->boundingRect().center().x());
    dy = 0 - (m_item->boundingRect().center().y());
    m_item->moveBy(dx, dy);
    
    // Re-assign item origin used for transformations
    m_item->setTransformOriginPoint(m_item->boundingRect().center());
    
    qDebug() << "Transform Origin Point 1: " << m_item->transformOriginPoint() << m_item->mapToScene(m_item->transformOriginPoint());

    // Should now rotate around item's center instead of corner
    m_item->setRotation(data.rotate);

    // Translate item center-point to input coordinates
    m_item->moveBy(data.pos.x(), data.pos.y());

    qDebug() << "Transform Origin Point 2: " << m_item->transformOriginPoint() << m_item->mapToScene(m_item->transformOriginPoint());

    qDebug() << "Text Data: ";
    qDebug() << "Item Rect Corners: " << m_item->boundingRect().topLeft() << m_item->boundingRect().bottomRight();
    qDebug() << "Item Rect Center: " << m_item->boundingRect().center();
    qDebug() << "Item Pos: " << m_item->pos();
    qDebug() << "Scene Rect Corners: " << m_item->sceneBoundingRect().topLeft() << m_item->sceneBoundingRect().bottomRight();
    qDebug() << "Scene Rect Center: " << m_item->sceneBoundingRect().center();
    qDebug() << "Scene Pos: " << m_item->scenePos();
    qDebug() << "View Pos: " << m_view->itemAt(0, 0) << m_view->itemAt(data.pos);
    break;

  case Settings::Type::Point_Type:

    // Initialize and center a bounding rect at the origin of its local item coordinates
    bRect = QRectF(0.0, 0.0, data.size, data.size);
    bRect.moveCenter(QPointF(0.0, 0.0));

    // Create an Ellipse bounded by the QRectF and add to scene
    m_item = m_scene->addEllipse(bRect, pen, pen.brush());
    
    // Move item to specified location
    m_item->moveBy(data.pos.x(), data.pos.y());

    qDebug() << "Ellipse Data: ";
    qDebug() << "Rect Center: " << m_item->mapToScene(m_item->boundingRect().center()) << m_item->childrenBoundingRect().center();
    qDebug() << "Item Pos: " << m_item->pos(); // Position of item's center point in parent coordinates
    qDebug() << "Scene Pos: " << m_item->scenePos() << m_scene->itemAt(data.pos, QTransform());
    qDebug() << "View Pos: " << m_view->itemAt(0, 0) << m_view->itemAt(data.pos);
    break;

  case Settings::Type::Line_Type:

    pen.setWidth(data.thicc);
    m_item = m_scene->addLine(QLineF(data.p1, data.p2), pen);

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

    qDebug() << "Invalid Item Type";
    break;
  }
  
}

void OutputWidget::drawCrosshairs(){
  
  QPen p;
  p.setWidth(0);
	p.setColor(Qt::black);
	p.setBrush(Qt::SolidPattern);

	double scale = 1;
	double radius = 0.1;

	// Initialize and center a bounding rect at the origin of its local item coordinates
	QRectF bRect(0.0, 0.0, 2 * radius, 2 * radius);
	bRect.moveCenter(QPointF(0.0, 0.0));

	// Create an Ellipse bounded by the QRectF and add to scene
	m_item = m_scene->addEllipse(bRect, p, p.brush());

  m_scene->addLine(QLineF(QPoint(-1 * scale, 0), QPoint(1 * scale, 0)), p);
  m_scene->addLine(QLineF(QPoint( 0, -1 * scale), QPoint(0, 1 * scale)), p);
}

void OutputWidget::resizeEvent(QResizeEvent * event){
	
  // Automatically re-focus the view when widget is resized
  m_view->fitInView(m_scene->itemsBoundingRect(), Qt::KeepAspectRatio);
	event->accept();
}

