#include "notebook_app.hpp"
#include "semantic_error.hpp"

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>

#include <QString>
#include <QLayout>
#include <QDebug>
#include <QGraphicsItem>
#include <QGraphicsEllipseItem>

NotebookApp::NotebookApp(QWidget * parent): QWidget(parent){
	
	input = new InputWidget();
	input->setParent(parent);
    
	output = new OutputWidget();
	output->setParent(parent);
    
    /*--- Run Startup Script ---*/
    startup(m_interp);

    /*--- Read User Input ---*/
    QObject::connect(input, SIGNAL(textUpdated(QString)),
      this, SLOT(getUserText(QString)));
    
    /*--- Evaluate with Plotscript ---*/
    
    /*--- Display Result Graphics ---*/
    QObject::connect(this, SIGNAL(graphicsResult(QGraphicsItem*)),
      output, SLOT(getItem(QGraphicsItem*)));
    
    auto layout = new QGridLayout();
	
	layout->addWidget(input, 0, 0);
	layout->addWidget(output, 1, 0);
	
	setLayout(layout);
}

int NotebookApp::startup(Interpreter & interp){
  
  std::ifstream ifs(STARTUP_FILE);
  std::ostringstream err;

  if(!ifs){
    emit graphicsResult(errFormat("Error: Could not open file for reading."));
    return EXIT_FAILURE;
  }
  
  if(!interp.parseStream(ifs)){
    ifs.close();
    emit graphicsResult(errFormat("Error: Invalid Program. Could not parse."));
    return EXIT_FAILURE;
  }
  else{
    try{
      Expression exp = interp.evaluate();
    }
    catch(const SemanticError & ex){
      ifs.close();
      err << ex.what() << std::endl;
      emit graphicsResult(errFormat(err.str()));
      return EXIT_FAILURE;
    }	
  }
  
  ifs.close();
  return EXIT_SUCCESS;
}

void NotebookApp::getUserText(QString inExp){
  
  qDebug() << "Slot: " << inExp << "\t";
  evalExpression(inExp.toStdString());
  
  //qDebug() << "Signal: " << result << "\n";
  //emit resultEvaluated(result);
}

void NotebookApp::evalExpression(std::string inExp){
  
  std::istringstream inStream(inExp);
  std::ostringstream outStream; // Need this to convert Expression->string
  std::string strResult = "default";
  Expression expResult;

  qDebug() << "Expression In: " << QString::fromStdString(inExp) << "\t";

  if(!m_interp.parseStream(inStream)){
    strResult = "Error: Invalid Program. Could not parse.";
    qDebug() << "Invalid Expression: " << QString::fromStdString(inExp) << "\t";
    expResult = Expression(Atom(strResult));
  }
  else{
    try{
      expResult = m_interp.evaluate();
      outStream << expResult;
      strResult = outStream.str();
      qDebug() << "Valid Expression: " << QString::fromStdString(strResult) << "\t";
    }
    catch(const SemanticError & ex){
      outStream << ex.what();
      strResult = outStream.str();
      expResult = Expression(Atom(strResult));
    }	
  }
  
  qDebug() << "Expression Out: " << QString::fromStdString(strResult) << "\n";
  setGraphicsType(expResult);
  //return QString::fromStdString(strResult);
}

void NotebookApp::setGraphicsType(Expression exp){

  // The graphic object to send to outputWidget
  QGraphicsItem * item = nullptr;
  
  // The data tag key for different object type names
  static const int ObjectType = 0;

  // Convert Expression->string
  std::ostringstream stream;
  stream << exp;
  
  // Pull out necessary parts of result Expression
  QString expName = QString::fromStdString(exp.getProperty("object-name").asString());
  QString expValue = QString::fromStdString(stream.str());
  
  // Assign graphic type
  if(exp.isHeadLambda()){
    // Display nothing for procedures
    item = nullptr;
  }
  if(exp.isHeadList()){
    // Recursively display each entry using the rules above without any surrounding parenthesis.
    //for(auto e : exp.asList());
      // Display e
  }
  else if(expName == "text"){
    
    // Position coordinates, default to origin
    double x = 0;
    double y = 0;

    // If "position" is in prop list, must be type "point" or error
    if(exp.getProperty("position") != Expression()){
      
      Expression expProp = exp.getProperty("position");
      std::string type = expProp.getProperty("object-name").asString();

      if((type == "point") && (expProp.isHeadList())){
        Expression::List points = expProp.asList();
        x = points[0].head().asNumber();
        y = points[1].head().asNumber();
      }
      else{
        // Error message
        emit graphicsResult(errFormat("Error: Position not a point"));
        return;
      }
    }
    item = new QGraphicsTextItem(expValue);
    item->setPos(x, y);
    
    // Tag object for ID
    item->setData(ObjectType, "Text Type");
  }
  else if(expName == "point"){
    
    if(!exp.isHeadList()){
      // Error message
      emit graphicsResult(errFormat("Error: Position not a point"));
      return;
    }

    // Centered at the Point's coordinates with a diameter equal to the size property.
    Expression::List points = exp.asList();
    double x = points[0].head().asNumber();;
    double y = points[1].head().asNumber();
    
    // Default
    double size = 0;
    
    // If "size" is present in the property list, it is an error if this property is not a positive Number.
    if(exp.getProperty("size") != Expression()){
      
      Expression expProp = exp.getProperty("size");

      if( expProp.head().isNumber() && (expProp.head().asNumber() > 0 ) ){
        size = exp.getProperty("size").head().asNumber();
      }
      else{
        // Error message
        emit graphicsResult(errFormat("Error: Size is not a positive number"));
        return;
      }
    }
    item = new QGraphicsEllipseItem(x, y, size, size);
    //item->setPos(x, y);
    
    // Tag object for ID
    item->setData(ObjectType, "Point Type");
  }
  else if(expName == "line"){
    // starting at the position indicated by it's position property.
    // If present in the property list, it is an error if this property is not a Point.
    //item = new QGraphicsLineItem();
    //scene->addItem(item);
    item = nullptr;
  }
  else{ // None, Number, Complex, Symbol, String, and error messages
    item = new QGraphicsTextItem(expValue);

    //scene->addItem(item);
    item->setPos(0, 0);
    
    // Tag the object for ID
    item->setData(ObjectType, "TUI Type");
  }
  
  // Send reference to graphic item to output
  qDebug() << "Signal: " << item << "\n";
  emit graphicsResult(item);
}


QGraphicsTextItem * NotebookApp::errFormat(std::string message){
  
  QString error = QString::fromStdString(message);
  
  QGraphicsTextItem * result = new QGraphicsTextItem(error);
  
  return result;
}