#ifndef NOTEBOOK_APP_H
#define NOTEBOOK_APP_H

#include <QWidget> 
#include <QLayout>  
#include <QDebug>	

#include"input_widget.hpp" 
#include"output_widget.hpp"

class NotebookApp: public QWidget	// Extends QObject base class
{
Q_OBJECT // Necessary macro first line of class definition

public:
	NotebookApp(QWidget * parent = nullptr);

//public slots:

private slots:
	// Keypress in the input widget has occurred
	//void plotScriptUpdated(std::string input);
	
private:
	InputWidget * input;
	OutputWidget * output;
	
//signals: // Neither public nor private
	
};
#endif // NOTEBOOK_APP_H
