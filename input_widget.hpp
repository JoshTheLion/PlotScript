#ifndef INPUT_WIDGET_H
#define INPUT_WIDGET_H

#include <QPlainTextEdit>
#include <QWidget>
#include <QDebug>
#include <QKeyEvent>

#include <QString>
#include <string>

class InputWidget: public QPlainTextEdit
{
Q_OBJECT

public:
	InputWidget(QWidget * parent = nullptr);

signals:
    void textUpdated(QString text);
	
private:
    // Override inherited to catch Shift+Enter
    void keyPressEvent(QKeyEvent *event); 
    QString inputExp;
};
#endif // INPUT_WIDGET_H
