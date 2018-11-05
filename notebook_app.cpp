#include "notebook_app.hpp"
#include "semantic_error.hpp"

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>

#include <QString>
#include <QLayout>
#include <QDebug>

NotebookApp::NotebookApp(QWidget * parent) : QWidget(parent){
	
	m_input = new InputWidget();
	m_input->setParent(parent);
    
	m_output = new OutputWidget();
	m_output->setParent(parent);
    
    /*--- Run Startup Script ---*/
    startup(m_interp);

    /*--- Read User Input ---*/
    QObject::connect(m_input, SIGNAL(textUpdated(QString)),
      this, SLOT(getUserText(QString)));
    
    /*--- Evaluate with Plotscript ---*/
    
    /*--- Display Result Graphics ---*/
    QObject::connect(this, SIGNAL(sendResult(Settings)),
      m_output, SLOT(getResult(Settings)));
    
    auto layout = new QGridLayout();
	
	layout->addWidget(m_input, 0, 0);
	layout->addWidget(m_output, 1, 0);
	
	setLayout(layout);
    resize(500, 500);
}

int NotebookApp::startup(Interpreter & interp){
  
  std::ifstream ifs(STARTUP_FILE);
  std::ostringstream err;

  if(!ifs){
    emit sendResult(errFormat("Error: Could not open startup file for reading."));
    return EXIT_FAILURE;
  }
  
  if(!interp.parseStream(ifs)){
    ifs.close();
    emit sendResult(errFormat("Error: Invalid startup program. Could not parse."));
    return EXIT_FAILURE;
  }
  else{
    try{
      Expression exp = interp.evaluate();
    }
    catch(const SemanticError & ex){
      ifs.close();
      err << ex.what();
      emit sendResult(errFormat(err.str()));
      return EXIT_FAILURE;
    }	
  }
  
  ifs.close();
  return EXIT_SUCCESS;
}

void NotebookApp::getUserText(QString inExp){
  
  qDebug() << "Slot: " << inExp << "\t";
  evalExpression(inExp.toStdString());
}

bool NotebookApp::evalExpression(std::string inExp){
  
  std::istringstream inStream(inExp);
  std::ostringstream outStream; // Need this to convert Expression->string
  std::string strResult = "default";
  Expression expResult;

  qDebug() << "Expression In: " << QString::fromStdString(inExp);

  if(!m_interp.parseStream(inStream)){
    emit sendResult(errFormat("Error: Invalid Program. Could not parse."));
    return EXIT_FAILURE;
  }
  else{
    try{
      expResult = m_interp.evaluate();
      outStream << expResult;
      strResult = outStream.str();
      qDebug() << "Valid Expression: " << QString::fromStdString(strResult);
    }
    catch(const SemanticError & ex){
      outStream << ex.what();
      emit sendResult(errFormat(outStream.str()));
      return EXIT_FAILURE;
    }	
  }
  
  qDebug() << "Expression Out: " << QString::fromStdString(strResult);
  setGraphicsType(expResult);
  return EXIT_SUCCESS;
}

void NotebookApp::setGraphicsType(Expression outExp){

  // The graphic object data to send to outputWidget
  Settings data;

  // Convert Expression->string
  std::ostringstream stream;
  stream << outExp;
  
  // Pull out necessary parts of result Expression
  std::string expName = outExp.getProperty("\"object-name\"").asString();
  std::string expValue = stream.str();
  
  qDebug() << "Data: " << QString::fromStdString(expName) << QString::fromStdString(expValue);
  
  // Assign graphic type and parameter data based on result
  if(outExp.isHeadLambda()){
    // Display nothing for procedures
    data = Settings();
  }
  else if(expName == "text"){
    
    QString text = QString::fromStdString(outExp.asString());
    
    // Position coordinates, default to origin
    double x = 0;
    double y = 0;

    // If "position" is in prop list, must be type "point" or error
    if(outExp.getProperty("\"position\"") != Expression()){
      
      Expression expProp = outExp.getProperty("\"position\"");
      std::string type = expProp.getProperty("\"object-name\"").asString();

      if((type == "point") && (expProp.isHeadList())){
        Expression::List points = expProp.asList();
        x = points[0].head().asNumber();
        y = points[1].head().asNumber();
      }
      else{
        // Error message
        emit sendResult(errFormat("Error: Position not a point"));
        return;
      }
    }
    
    // Package result values for output
    data = Settings(Settings::Type::Text_Type, QPoint(x,y), text);
  }
  else if((expName == "point") || (expName == "\"point\"")){
    
    if(!outExp.isHeadList()){
      // Error message
      emit sendResult(errFormat("Error: Position not a point"));
      return;
    }

    // Centered at the Point's coordinates with a diameter equal to the size property
    Expression::List points = outExp.asList();
    double x = points[0].head().asNumber();
    double y = points[1].head().asNumber();
    
    // Default (should be 0 for submission)
    double size = 0;
    
    // If "size" is present in the property list, it is an error if this property is not a positive Number.
    if(outExp.getProperty("\"size\"") != Expression()){
      
      Expression expProp = outExp.getProperty("\"size\"");

      if( expProp.head().isNumber() && (expProp.head().asNumber() > 0 ) ){
        size = outExp.getProperty("\"size\"").head().asNumber();
      }
      else{
        // Error message
        emit sendResult(errFormat("Error: Size is not a positive number"));
        return;
      }
    }
    
    qDebug() << "Point Data: " << x << y << size;
    
    // Package result values for output
    data = Settings(Settings::Type::Point_Type, QPoint(x, y), size);
  }
  /*
  else if(expName == "\"line\""){
    // starting at the position indicated by it's position property.
    // If present in the property list, it is an error if this property is not a Point.
    
  }
  else if(outExp.isHeadList()){
    // Recursively display each entry using the rules above without any surrounding parenthesis.
    //for(auto e : exp.asList());
      // Display e
    
  }
  */
  else{ // None, Number, Complex, Symbol, String, and error messages
    
    // Package result values for output
    data = Settings(Settings::Type::Text_Type, QPoint(0,0), QString::fromStdString(expValue));//item = new QGraphicsTextItem(expValue);
  }
  
  qDebug() << "Signal: " << data.itemType << "\n";
  
  // Send graphic item parameters to output
  emit sendResult(data);
}


Settings NotebookApp::errFormat(const std::string & message){
  
  QString error = QString::fromStdString(message);
  
  return Settings(Settings::Type::Text_Type, QPoint(0,0), error);
}