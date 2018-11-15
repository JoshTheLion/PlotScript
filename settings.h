#ifndef SETTINGS_H
#define SETTINGS_H

#include <QWidget>
#include <QVector>
#include <QString>
#include <QPoint>

/* This structure is to be used by NotebookApp::setGraphicsType(Expression exp)
 * to encapsulate the type and data parameters of each evaluated expression, and
 * to send it to OutputWidget to be converted into a QGraphicsItem and drawn. */
struct Settings {
  
  // internal enum of known types
  enum Type { None_Type, TUI_Type, Text_Type, Point_Type, Line_Type, List_Type };
  
  // track the type, default none
  Type itemType = None_Type;
  
  QPoint pos;
  QVector<Settings> list;
  QString text; // used when type is Text_Type or TUI_Type
  double scale; // used when type is Text_Type
  double rotate;// used when type is Text_Type
  double size;  // used when type is Point_Type
  QPoint p1;		// used when type is Line_Type
  QPoint p2;		// used when type is Line_Type
	double thicc; // used when type is Line_Type

  // constructors for use in assignment, signal, and output
  Settings() {};
  Settings(Type t, QString str) : itemType(t), text(str) {};
  Settings(Type t, QPoint p, QString str, double s, double r) : itemType(t), pos(p), text(str), scale(s), rotate(r) {};
  Settings(Type p, QPoint c, double s) : itemType(p), pos(c), size(s) {};
  Settings(Type l, QPoint p1, QPoint p2, double t) : itemType(l), p1(p1), p2(p2), thicc(t) {};
  Settings(Type v, QVector<Settings> l) : itemType(v), list(l) {};
};

#endif
