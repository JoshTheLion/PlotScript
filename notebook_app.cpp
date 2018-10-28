#include "notebook_app.hpp"
#include "semantic_error.hpp"

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>

#include <QString>
#include <QLayout>
#include <QDebug>

NotebookApp::NotebookApp(QWidget * parent): QWidget(parent){
	
	input = new InputWidget();
	input->setParent(parent);
    
	output = new OutputWidget();
	output->setParent(parent);
    
    /*--- Startup Script ---*/
    // TO-DO

    /*--- Read User Input ---*/
    QObject::connect(input, SIGNAL(textUpdated(QString)),
      this, SLOT(getUserText(QString)));
    
    /*--- Evaluate with Plotscript ---*/
    
    /*--- Display Result Graphics ---*/
    QObject::connect(this, SIGNAL(resultEvaluated(QString)),
      output, SLOT(getResult(QString)));
    
    auto layout = new QGridLayout();
	
	layout->addWidget(input, 0, 0);
	layout->addWidget(output, 1, 0);
	
	setLayout(layout);
}

void NotebookApp::getUserText(QString inExp){
  
  qDebug() << "Slot: " << inExp << "\t";
  QString result = evalExpression(inExp.toStdString());
  
  qDebug() << "Signal: " << result << "\n";
  emit resultEvaluated(result);
}

QString NotebookApp::evalExpression(std::string inExp){
  
  std::istringstream inStream(inExp);
  std::ostringstream outStream; // Need this to convert Expression->string
  std::string result = "default";
  
  qDebug() << "Expression In: " << QString::fromStdString(inExp) << "\t";

  if(!m_interp.parseStream(inStream)){
    result = "Invalid Program. Could not parse.";
    qDebug() << "Invalid Expression: " << QString::fromStdString(inExp) << "\t";
  }
  else{
    try{
      Expression exp = m_interp.evaluate();
      outStream << exp;
      result = outStream.str();
      qDebug() << "Valid Expression: " << QString::fromStdString(result) << "\t";
    }
    catch(const SemanticError & ex){
      outStream << ex.what();
      result = outStream.str();
    }	
  }
  
  qDebug() << "Expression Out: " << QString::fromStdString(result) << "\n";
  return QString::fromStdString(result);
}


