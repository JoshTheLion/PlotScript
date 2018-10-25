#ifndef INPUT_WIDGET_H
#define INPUT_WIDGET_H

#include <QPlainTextEdit>
#include <QLineEdit>
#include <QWidget>
#include <QDebug>
#include <QKeyEvent>

#include <QString>
#include <string>

class InputWidget: public QPlainTextEdit
{
Q_OBJECT // Necessary macro in class definition

public:
	InputWidget(QWidget * parent = nullptr);
	
	//std::string getUserText();
    //QString getUserText();
	//Expression getEvalResult();

//public slots:

private slots:
	//void textChanged(QString text);

signals:
  // When the shift+enter keys are pushed, the text is put into the stream
    void textChanged(QString text);
	
private:
	//QLineEdit * value;
	//std::string inputExp;
    QString inputExp;
    //QLineEdit * inputExp;
	void keyPressEvent(QKeyEvent *event);
//signals: // Neither public nor private
	//Expression * result; // Send result to OutputWidget
};
#endif // INPUT_WIDGET_H
