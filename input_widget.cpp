#include "input_widget.hpp"

#include <QWidget>
#include <QDebug>
#include <QLineEdit>

// public:
InputWidget::InputWidget(QWidget * parent): QPlainTextEdit(parent){
	
	//inputExp = new QLineEdit();
    //inputExp = new QLineEdit();
    //inputExp->setParent(parent);
	//QObject::connect();
	
	// Evaluate
}
/*	
QString InputWidget::getUserText(){
	return inputExp;
}

Expression InputWidget::getEvalResult(){
	return result;
}

//private slots:
void InputWidget::textChanged(std::string s){
	qDebug() << s;
	inputExp = s;
}
*/
void InputWidget::keyPressEvent(QKeyEvent * event){
	// Stub
}


