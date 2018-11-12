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
  Settings result = setGraphicsType(expResult);
  emit sendResult(result);
  return EXIT_SUCCESS;
}

Settings NotebookApp::setGraphicsType(Expression outExp){

  // The graphic object data to send to outputWidget
  Settings data;
  
  // Pull out necessary parts of result Expression
  Expression propExp = outExp.getProperty("\"object-name\"");
  std::ostringstream nameStream;
  nameStream << propExp;
  std::string expName = nameStream.str();

  // Convert Expression->string
  std::ostringstream expStream;
  expStream << outExp;
  std::string expValue = expStream.str();
  
  qDebug() << "Data: " << QString::fromStdString(expName) << QString::fromStdString(expValue);
  
  // Assign graphic type and parameter data based on result
  if(outExp.isHeadLambda()){
    
    // Display nothing for procedures
    data = Settings();
  }
  else if(outExp.isTextG()){
    
    QString text = QString::fromStdString(outExp.asString());
    
    // Default values
    double x = 0;
    double y = 0;
    double scale = 1;
    double rotate = 0;

    // If "position" is in prop list, must be type "point" or error
    if(outExp.getProperty("\"position\"") != Expression()){

      Expression expProp = outExp.getProperty("\"position\"");

      if(expProp.isPointG()){
        Expression::List points = expProp.asList();
        x = points[0].head().asNumber();
        y = points[1].head().asNumber();
      }
      else{
        return errFormat("Error: Position not a point");
      }
    }
    
    // If "text-scale" is in prop list, it should be a positive Number
    if(outExp.getProperty("\"text-scale\"") != Expression()){

      Expression expProp = outExp.getProperty("\"text-scale\"");

      if( expProp.isHeadNumber() && (expProp.head().asNumber() > 0) ){
        scale = expProp.head().asNumber();
      }
    }
    
    // If "text-rotation" is in prop list, it should be a Number in radians
    if (outExp.getProperty("\"text-rotation\"") != Expression()) {

      Expression expProp = outExp.getProperty("\"text-rotation\"");

      if (expProp.isHeadNumber()) {
        // Convert input to degrees for rotate function to work
        double rad = expProp.head().asNumber();
        double deg = rad * (180/std::atan2(0, -1));
        rotate = deg;
      }
    }

    // Package result values for output
    data = Settings(Settings::Type::Text_Type, QPoint(x,y), text, scale, rotate);
  }
  else if(outExp.isPointG()){

    // Center at the Point's coordinates with a diameter equal to the size property
    Expression::List points = outExp.asList();
    
    double x = points[0].head().asNumber();
    double y = points[1].head().asNumber();

    double size = 0;
    
    // If "size" is present in the property list, it is an error if this property is not a positive Number.
    if(outExp.getProperty("\"size\"") != Expression()){
      
      Expression expProp = outExp.getProperty("\"size\"");

      if( expProp.head().isNumber() && (expProp.head().asNumber() > 0 ) ){
        size = outExp.getProperty("\"size\"").head().asNumber();
      }
      else{
        return errFormat("Error: Size is not a positive number");
      }
    }
    
    // Package result values for output
    data = Settings(Settings::Type::Point_Type, QPoint(x, y), size);
  }
  else if(outExp.isLineG()){

    // Use the Line's coordinates with a thickness equal to the thickness property.
    Expression::List points = outExp.asList();
    
    Expression::List point1 = points[0].asList();
    Expression::List point2 = points[1].asList();

    double x1 = point1[0].head().asNumber();
    double y1 = point1[1].head().asNumber();
    
    double x2 = point2[0].head().asNumber();
    double y2 = point2[1].head().asNumber();

    QPoint p1(x1, y1);
    QPoint p2(x2, y2);
    
    double thicc = 1;
    
    // If "thickness" is present in the property list, it is an error if this property is not a positive Number.
    if(outExp.getProperty("\"thickness\"") != Expression()){
      
      Expression expProp = outExp.getProperty("\"thickness\"");

      if( expProp.head().isNumber() && (expProp.head().asNumber() > 0 ) ){
        thicc = outExp.getProperty("\"thickness\"").head().asNumber();
      }
      else{
        return errFormat("Error: Thickness is not a positive number");
      }
    }
    
    // Package result values for output
    data = Settings(Settings::Type::Line_Type, p1, p2, thicc);
  }
  else if(outExp.isHeadList() && (!outExp.isTailEmpty())){

    // Recursively display each entry using the rules above without any surrounding parenthesis.
    QVector<Settings> list;
    for(Expression exp : outExp.asList()){
      list.push_back(setGraphicsType(exp));
    }
    
    // Package result values for output
    data = Settings(Settings::Type::List_Type, list);
  }
  else{ // None, Number, Complex, Symbol, String
    
    // Package result values for output
    data = Settings(Settings::Type::TUI_Type, QString::fromStdString(expValue));
  }
  
  qDebug() << "Result: " << data.itemType;
  
  // Send graphic item parameters to output widget to display
  return data;
}


Settings NotebookApp::errFormat(const std::string & message){
  
  QString error = QString::fromStdString(message);
  
  return Settings(Settings::Type::TUI_Type, error);
}