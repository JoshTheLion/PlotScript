#include "expression.hpp"
#include "environment.hpp"
#include "semantic_error.hpp"

#include <sstream>
#include <iomanip>
#include <list>
#include <vector>
#include <algorithm>
#include <string>

Expression::Expression(){}

Expression::Expression(const Atom & a){

  m_head = a;
}

// recursive copy
Expression::Expression(const Expression & a){

  m_head = a.m_head;
  m_props = a.m_props;
  for(auto e : a.m_tail){
    m_tail.push_back(e);
  }
}

// List Type constructor
Expression::Expression(const List & list){

  m_head = Atom("list");
  m_tail = list;
}

// Lambda Type constructor
Expression::Expression(const List & parameters, const Expression & function){

  m_head = Atom("lambda");

  // Combine both arguments into new Lambda Type Expression
  m_tail = { Expression(parameters), function };
}

Expression & Expression::operator=(const Expression & a){

  // prevent self-assignment
  if(this != &a){
    m_head = a.m_head;
    m_props = a.m_props;
    m_tail.clear();
    for(auto e : a.m_tail){
      m_tail.push_back(e);
    }
  }
  
  return *this;
}


Atom & Expression::head(){
  return m_head;
}

const Atom & Expression::head() const{
  return m_head;
}
void Expression::append(const Atom & a){
  m_tail.emplace_back(a);
}

Expression * Expression::tail(){
  Expression * ptr = nullptr;
  
  if(m_tail.size() > 0){
    ptr = &m_tail.back();
  }

  return ptr;
}

Expression::ConstIteratorType Expression::tailConstBegin() const noexcept{
  return m_tail.cbegin();
}

Expression::ConstIteratorType Expression::tailConstEnd() const noexcept{
  return m_tail.cend();
}


bool Expression::isTailEmpty() const noexcept{
  return m_tail.empty();
}

bool Expression::isHeadNumber() const noexcept{
  return m_head.isNumber();
}

bool Expression::isHeadSymbol() const noexcept{
  return m_head.isSymbol();
}

bool Expression::isHeadComplex() const noexcept{
  return m_head.isComplex();
}

bool Expression::isHeadString() const noexcept{
  return m_head.isString();
}

bool Expression::isHeadList() const noexcept{
  return ((m_head.isSymbol()) && (m_head.asSymbol() == "list"));
}

bool Expression::isHeadLambda() const noexcept{
  return ((m_head.isSymbol()) && (m_head.asSymbol() == "lambda"));
}


Expression::List Expression::asList() const noexcept{
  
  List result;
  
  if (isHeadList()) { result = m_tail; }

  return result;
}

Expression::Lambda Expression::asLambda() const noexcept{
  
  Lambda result;
  
  if (isHeadLambda()) { result = std::make_pair(m_tail[0].m_tail, m_tail[1]); }

  return result;
}

Expression::String Expression::asString() const noexcept{
  
  String result = "default";
  
  // Cut off extra quotation marks for easier comparison
  if(isHeadString()){
    
    String str = m_head.asString();
    size_t min = 1;
    
    if(str.size() > min){
      result = str.substr(1, str.size() - 2);
    }
  }
  
  return result;
}

/***********************************************************************
Property List and Graphic Primitive Methods
**********************************************************************/
void Expression::setProperty(const String key, Expression value)
{
  // Add/reset (key, value) to this Expression's property list
  if(this->m_props.find(key) != this->m_props.end()){
    std::swap(this->m_props.at(key), value);
  }
  else{
		this->m_props.emplace(key, value);
  }
}

Expression Expression::getProperty(const String key) const noexcept{

  // Search this Expression's property list for key
  auto result = this->m_props.find(key);
  if(result != this->m_props.end()){
    return result->second;
  }
  
  // Default return NONE
  return Expression();
}

bool Expression::isPointG() const noexcept{
  
  if(getProperty("\"object-name\"").m_head.asString() == "\"point\""){
    if(isHeadList() && (m_tail.size() == 2)){
      if (m_tail[0].isHeadNumber() && m_tail[1].isHeadNumber()){
        return true;
      }
    }
  }
  return false;
}

bool Expression::isLineG() const noexcept{
  
  if(getProperty("\"object-name\"").m_head.asString() == "\"line\""){
    if(isHeadList() && (m_tail.size() == 2)){
      //if( m_tail[0].isPointG() && m_tail[1].isPointG() ){
        return true;
      //}
    }
  }
  return false;
}

bool Expression::isTextG() const noexcept{
  
  return ( isHeadString() && (getProperty("\"object-name\"").m_head.asString() == "\"text\"") );
}


/*
std::vector<double> Expression::getDataExtrema(const Expression::List & dataList){
	
	// Must be at least 2 points to process
	if(dataList.size() >= 2){
		throw SemanticError("Error: invalid number of Data points");
	}
	
	// Store data in tail, props in prop list
	//std::vector<double> data;
	//Expression result;
	
	// Initialize extrema point data
	double xMax = 0; double yMax = 0;
	double xMin = 0; double yMin = 0;
	
	// Check first two points first
	Expression p1 = dataList[0];
	Expression p2 = dataList[1];

	// Each Data entry must a List of 2 Numbers or error
	if ( p1.isHeadList() && (p1.asList().size() == 2)
		&& p2.isHeadList() && (p2.asList().size() == 2) )
	{
		List temp1 = p1.asList(); List temp2 = p2.asList();
		if ( temp1[0].isHeadNumber() && temp1[1].isHeadNumber()
			&& temp1[0].isHeadNumber() && temp1[1].isHeadNumber() )
		{
			double x1 = temp1[0].head().asNumber();
			double y1 = temp1[1].head().asNumber();
			
			double x2 = temp2[0].head().asNumber();
			double y2 = temp2[1].head().asNumber();

			// Assign initial data bounding box values for comparison
			xMax = std::max(x1, x2);	yMax = std::max(y1, y2);
			xMin = std::min(x1, x2);	yMin = std::min(y1, y2);
		}
		else { throw SemanticError("Error: invalid Data points"); }
	}
	else { throw SemanticError("Error: invalid Data points"); }
	

	// Can now safely check any and all remaining points in data list
	for (auto exp : dataList) {
		// Each Data entry must a List of 2 Numbers or error
		if ( exp.isHeadList() && (exp.asList().size() == 2)
			&& exp.asList()[0].isHeadNumber() && exp.asList()[1].isHeadNumber() )
		{
				//Keep track of the max and min x and y values
				double thisX = exp.asList()[0].head().asNumber();
				double thisY = exp.asList()[1].head().asNumber();
				
				xMax = std::max(xMax, thisX);		yMax = std::max(yMax, thisY);
				xMin = std::max(xMin, thisX);		yMin = std::max(yMin, thisY);
		}
		else { throw SemanticError("Error: found invalid Data point"); }
	}
	
	std::vector<double> data = { xMax, yMax, xMin, yMin };
	//result = Expression(data);
	
	// Assign properties

	//return result;
	return data;
}
*/

std::vector<Expression::Point> parseData(const Expression::List & dataList, LayoutParams & params){
	
	std::vector<Expression::Point> results;
	
	// Initialize extrema point data
	double xMax = 0; double yMax = 0;
	double xMin = 0; double yMin = 0;
	
	// Check first two points first
	Expression p1 = dataList[0];
	Expression p2 = dataList[1];

	// Each Data entry must a List of 2 Numbers or error
	if ( p1.isHeadList() && (p1.asList().size() == 2)
		&& p2.isHeadList() && (p2.asList().size() == 2) )
	{
		Expression::List temp1 = p1.asList();
		Expression::List temp2 = p2.asList();

		if ( temp1[0].isHeadNumber() && temp1[1].isHeadNumber()
			&& temp2[0].isHeadNumber() && temp2[1].isHeadNumber() )
		{
			double x1 = temp1[0].head().asNumber();
			double y1 = temp1[1].head().asNumber();
			
			double x2 = temp2[0].head().asNumber();
			double y2 = temp2[1].head().asNumber();

			// Assign initial data bounding box values for comparison
			xMax = std::max(x1, x2);	yMax = std::max(y1, y2);
			xMin = std::min(x1, x2);	yMin = std::min(y1, y2);
		}
		else { throw SemanticError("Error: invalid Data points"); }
	}
	else { throw SemanticError("Error: invalid Data points"); }


	// Can now safely check any and all remaining points in data list
	for (auto & exp : dataList) {
		// Each Data entry must a List of 2 Numbers or error
		if (exp.isHeadList() && (exp.asList().size() == 2)) {
			if (exp.asList()[0].isHeadNumber() && exp.asList()[1].isHeadNumber()) {
				/*--- Keep track of the max and min x and y values ---*/
				double thisX = exp.asList()[0].head().asNumber();
				double thisY = exp.asList()[1].head().asNumber();
				
				xMax = std::max(xMax, thisX);		yMax = std::max(yMax, thisY);
				xMin = std::min(xMin, thisX);		yMin = std::min(yMin, thisY);
				
				Expression::Point point = std::make_pair(thisX, thisY);
				
				results.push_back(point);
			}
			else { throw SemanticError("Error: found invalid Data point"); }
		}
		else { throw SemanticError("Error: found invalid Data point"); }
	}
	
	// Assign properties
	params.xMax = xMax;		params.yMax = yMax;
	params.xMin = xMin;		params.yMin = yMin;

	return results;
};

Expression makePoint(double x, double y, double size){
	
	// Create Number Expression coordinates
	Expression xVal = Expression(Atom(x));
	Expression yVal = Expression(Atom(y));
	Expression::List values = { xVal, yVal };

	// Create a Point graphic item
	Expression pointItem = Expression(values);
	
	// Set properties
	Expression name = Expression(Atom("\"point\""));
	pointItem.setProperty("\"object-name\"", name);

	Expression s = Expression(Atom(size));
	pointItem.setProperty("\"size\"", s);

	return pointItem;
};

Expression makeLine(double x1, double y1, double x2, double y2, double thicc){
	
	// Create Point Expression items
	Expression p1 = makePoint(x1, y1, 1);
	Expression p2 = makePoint(x2, y2, 1);
	Expression::List values = { p1, p2 };

	// Create a Line graphic item
	Expression lineItem = Expression(values);

	// Set properties
	Expression name = Expression(Atom("\"line\""));
	lineItem.setProperty("\"object-name\"", name);

	Expression t = Expression(Atom(thicc));
	lineItem.setProperty("\"thickness\"", t);

	return lineItem;
};

Expression::List makeBoundBox(LayoutParams & params){
	
	// Pull struct data into local variables
	double xMax = params.xMax;	double yMax = params.yMax;
	double xMin = params.xMin;	double yMin = params.yMin;
	//double xMid = params.xMid;	double yMid = params.yMid;
	
	// Top border Line: ( (xMin, yMax) (xMax, yMax) )
	Expression topLine = makeLine(xMin, yMax, xMax, yMax, 0);

	// Bottom border Line: ( (xMin, yMin) (xMax, yMin) )
	Expression bottomLine = makeLine(xMin, yMin, xMax, yMin, 0);
	
	// Left border Line: ( (xMin, yMin) (xMin, yMax) )
	Expression leftLine = makeLine(xMin, yMin, xMin, yMax, 0);
	
	// Right border Line: ( (xMax, yMin) (xMax, yMax) )
	Expression rightLine = makeLine(xMax, yMin, xMax, yMax, 0);
	
	Expression::List results = { topLine, bottomLine, leftLine, rightLine };

	// Draw X Axis?
	if ( (yMin < 0) && (0 < yMax) ){
		Expression xAxisLine = makeLine(xMin, 0, xMax, 0, 0);
		results.push_back(xAxisLine);
		params.xAxis = true;
	}

	// Draw Y Axis?
	if ( (xMin < 0) && (0 < xMax) ){
		Expression yAxisLine = makeLine(0, yMin, 0, yMax, 0);
		results.push_back(yAxisLine);
		params.yAxis = true;
	}

	return results;
};

double getTextScale(const Expression::List & options){
	
	double txtScale = 1;	// Scaling factor for all Text

	for (auto & option : options) {
		// Each Options entry must be a List of 2 Expressions
		if ( option.isHeadList() && (option.asList().size() == 2)
			&& (option.asList()[0].isHeadString()) )
		{
				std::string name = option.asList()[0].asString();
				Expression value = option.asList()[1];

				if (name == "text-scale") {
					// Must be a positive Number, defaults to 1
					if (value.isHeadNumber() && (value.head().asNumber() > 0)) {
						txtScale = value.head().asNumber();
					}
					else {
						throw SemanticError("Error: invalid Option, text-scale must be positive");
					}
				}
		}
		else {
			throw SemanticError("Error: found invalid Option");
		}
	}
	return txtScale;
};

Expression makeText(const Expression::String & text, double x, double y, double s, double rotate){

	// Need to add '\"' to make text a String literal
	std::istringstream textStream(text);
	std::ostringstream outStream;
	outStream << "\"" << text << "\"";
	std::string strHack = outStream.str();

	Expression result = Expression(Atom(strHack));

	// Convert input to radians
	double deg = rotate;
	double rad = deg * (std::atan2(0,-1)/180);
	
	Expression rotation = Expression(Atom(rad));
	Expression name = Expression(Atom("\"text\""));
	Expression scale = Expression(Atom(s));

	result.setProperty("\"text-rotation\"", rotation);
	result.setProperty("\"object-name\"", name);
	result.setProperty("\"text-scale\"", scale);

	// Make Text item's center-point
	Expression xVal = Expression(Atom(x));
	Expression yVal = Expression(Atom(y));

	Expression::List data = { xVal, yVal };
	Expression pointItem = Expression(data);

	pointItem.setProperty("\"object-name\"", Expression(Atom("\"point\"")));

	result.setProperty("\"position\"", pointItem);

	return result;
};

Expression::List processOptions(const Expression::List & options, const LayoutParams & params){
	
	Expression::List results;

	for (auto & option : options) {
		// Each Options tag must be a List of 2 Expressions, the first being a String
		if ( option.isHeadList() && (option.asList().size() == 2)
			&& (option.asList()[0].isHeadString()) )
		{
			// Separate tag arguments into easily testable values
			std::string tagName = option.asList()[0].asString();
			Expression tagValue = option.asList()[1];

			// Check which(if any) plotting option to assign
			if ( (tagName == "title") && (tagValue.isHeadString()) ) {
				// Make a new Text graphic item horizontally centered at the top
				double x = params.xMid;
				double y = params.yMax + params.A;
				Expression item = makeText(tagValue.asString(), x, y, params.txtScale, 0);
				results.push_back(item);
			}
			else if ( (tagName == "abscissa-label") && (tagValue.isHeadString()) ) {
				// Make a new Text graphic item horizontally centered at the bottom
				double x = params.xMid;
				double y = params.yMin - params.A;
				Expression item = makeText(tagValue.asString(), x, y, params.txtScale, 0);
				results.push_back(item);
			}
			else if ( (tagName == "ordinate-label") && (tagValue.isHeadString()) ) {
				// Make a new Text graphic item vertically centered on the left
				double x = params.xMin - params.B;
				double y = params.yMid;
				Expression item = makeText(tagValue.asString(), x, y, params.txtScale, -90);
				results.push_back(item);
			}
			// end if
		}
		else {
			throw SemanticError("Error: invalid Option arguments");
		}
	} 
	// end for
	return results;
};

Expression::List makeTickLabels(const LayoutParams & oldParams, const LayoutParams & newParams){

	// Pull struct data into local variables
	double xMax = newParams.xMax;	double yMax = newParams.yMax;
	double xMin = newParams.xMin;	double yMin = newParams.yMin;
	double xOff = newParams.D;		double yOff = newParams.C;
	
	double scale = newParams.txtScale;
	
	// Label values should be unscaled input values, but use scaled values for placing
	std::ostringstream outStream1;
	outStream1 << std::setprecision(2) << oldParams.xMax;
	std::string xTop = outStream1.str();
	
	std::ostringstream outStream2;
	outStream2 << std::setprecision(2) << oldParams.xMin;
	std::string xBot = outStream2.str();
	
	std::ostringstream outStream3;
	outStream3 << std::setprecision(2) << oldParams.yMax;
	std::string yRight = outStream3.str();
	
	std::ostringstream outStream4;
	outStream4 << std::setprecision(2) << oldParams.yMin;
	std::string yLeft = outStream4.str();

	Expression xTopLabel = makeText(xTop, xMin - xOff, yMax, scale, 0.0);
	Expression xBottomLabel = makeText(xBot, xMin - xOff, yMin, scale, 0.0);
	
	Expression yRightLabel = makeText(yRight, xMax, yMin - yOff, scale, 0.0);
	Expression yLeftLabel = makeText(yLeft, xMin, yMin - yOff, scale, 0.0);

	Expression::List results = { xTopLabel, xBottomLabel, yLeftLabel, yRightLabel };

	return results;
};

/*
struct LayoutParams {
  
	double N = 20;		// Scale for the N x N bounding rect
	double A = 3;			// Vertical offset distance for title and axes labels
	double B = 3;			// Horizontal offset distance for title and axes labels
	double C = 2;			// Vertical offset distance for tick labels
	double D = 2;			// Horizontal offset distance for tick labels
	double P = 0.5;		// Size of points
	
	double xMax; double yMax;
	double xMin; double yMin;
	double xMid; double yMid;
	
	double txtScale;
};
*/
Expression::List Expression::makeDiscretePlot(const List & data, const List & options){

	List results;
	LayoutParams params;

	/*--- Read and Process each Data List Entry ---*/
	std::vector<Point> points = parseData(data, params);
	
	LayoutParams outParams;
	
	/*--- Calculate Scaled Values ---*/
	double width = std::abs(params.xMax - params.xMin);
	double height = std::abs(params.yMax - params.yMin);
	
	if( (width == 0) || (height == 0) ){
		throw SemanticError("Error: invalid Data");
	}
	
	/*--- Apply Scaling Factor ---*/
	double scaleX = (params.N / width);
	double scaleY = (params.N / height); // Fixes the inverted y-axis of Q?
	
	outParams.xMin = scaleX * params.xMin;
	outParams.xMax = scaleX * params.xMax;
	outParams.yMin = scaleY * params.yMin;
	outParams.yMax = scaleY * params.yMax;
	outParams.xMid = (params.xMax + params.xMin) / 2;
	outParams.yMid = (params.yMax + params.yMin) / 2;

	/*--- Analyze and Setup Graphical Layout Data ---*/
	List box = makeBoundBox(outParams);
	for (auto & item : box) {
		results.push_back(item);
	}

	/*--- Create Stem Plot Points ---*/
	for (auto & point : points) {
		
		// Scale each old point to make new point
		double thisX = point.first  * scaleX;
		double thisY = point.second * scaleY;
		
		// Create and add a Point graphic item to result, along with extension line
		Expression pointItem = makePoint(thisX, thisY, outParams.P);
		
		// Check which direction to draw extension lines
		Expression stemItem;
		
		if(outParams.xAxis){
			// Draw line from point to axis
			stemItem = makeLine(thisX, thisY, thisX, 0.0, 0.0);
		}
		else if(outParams.yMax > 0){
			// Draw line from point to top box edge
			stemItem = makeLine(thisX, thisY, thisX, outParams.yMax, 0.0);
		}
		else if(outParams.yMin < 0){
			// Draw line from point to bottom box edge
			stemItem = makeLine(thisX, thisY, thisX, outParams.yMin, 0.0);
		}
		
		results.push_back(pointItem);
		results.push_back(stemItem);
	}

	/*--- Get Text Scaling Factor ---*/
	outParams.txtScale = getTextScale(options); // Scaling factor for all Text

	/*--- Use Options List to Make Text Labels ---*/
	List optionsResult = processOptions(options, outParams);
	for (auto & item : optionsResult) {
		results.push_back(item);
	}

	/*--- Make the Tick Mark Text Labels ---*/
	List labels = makeTickLabels(params, outParams);
	for (auto & item : labels) {
		results.push_back(item);
	}
	
	return results;
}

/***********************************************************************
Private Methods
**********************************************************************/

Expression Expression::apply(const Atom & op, const List & args, const Environment & env){

  // head must be a symbol
  if(!op.isSymbol()){
    throw SemanticError("Error during evaluation: procedure name not symbol");
  }
  
  // Must map to a built-in or user-defined proc
  if(env.is_proc(op)){
    // map from symbol to proc
    Procedure proc = env.get_proc(op);

    // call proc with args
    return proc(args);
  }
  else if(env.is_anon_proc(op)){
    // Get the function the symbol maps to
	Expression mappedExp = env.get_exp(op);

	// Evaluate function with args
	return call_lambda(mappedExp, args, env);
  }
  else{
    throw SemanticError("Error during evaluation: symbol does not name a procedure");
  }
  return Expression();
}

Expression Expression::handle_lookup(const Atom & head, const Environment & env){
  
  // if symbol is in env return value
  if (head.isSymbol()) { 
    if (env.is_exp(head)) {
      return env.get_exp(head);
    }
    else {
      throw SemanticError("Error during evaluation: unknown symbol");
    }
  }
  // if literal is in env return value
  else if (head.isNumber() || head.isComplex() || head.isString()) {
    return Expression(head);
  }
  else {
    throw SemanticError("Error during evaluation: Invalid type in terminal expression");
  }
}

/* (begin <expression> <expression> ...) evaluates each expression in order,
 * evaluating to the last.
 */
Expression Expression::handle_begin(Environment & env){
  
  if(m_tail.size() == 0){
    throw SemanticError("Error during evaluation: zero arguments to begin");
  }

  // evaluate each arg from tail, return the last
  Expression result;
  for(Expression::IteratorType it = m_tail.begin(); it != m_tail.end(); ++it){
    result = it->eval(env);
  }
  
  return result;
}

/*
 * (define <symbol> <expression>) adds a mapping from the symbol to the
 * result of the expression in the environment. It is an error to redefine
 * a symbol. This evaluates to the expression the symbol is defined as (maps
 * to in the environment).
 */
Expression Expression::handle_define(Environment & env){

  // tail must have two arguments or error
  if(m_tail.size() != 2){
    throw SemanticError("Error during evaluation: invalid number of arguments to define");
  }
  
  // tail[0] must be symbol
  if(!m_tail[0].isHeadSymbol()){
    throw SemanticError("Error during evaluation: first argument to define not symbol");
  }

  // but tail[0] must not be a special-form or procedure
  std::string s = m_tail[0].head().asSymbol();
  if((s == "define") || (s == "begin") || (s == "lambda")){
    throw SemanticError("Error during evaluation: attempt to redefine a special-form");
  }
  
  if((env.is_proc(m_head)) || (s == "apply") || (s == "map")
      /*|| (s == "set-property") || (s == "get-property")*/)
  {
    throw SemanticError("Error during evaluation: attempt to redefine a built-in procedure");
  }

  // eval tail[1]
  Expression result = m_tail[1].eval(env);

  // Only user-defined functions can be overriden
  if( (env.is_exp(m_head)) && (!env.is_anon_proc(m_head)) ){
    throw SemanticError("Error during evaluation: attempt to redefine a previously defined symbol");
  }

  // and add to env
  env.add_exp(m_tail[0].head(), result);
  
  return result;
}

/* (lambda <list> <expression>)
 * The lambda special-form has two arguments. The first argument is a
 * parenthetical list of symbols that are the lambda function arguments
 * (parameters or inputs). The second argument is an expression.
 * Note this expression is not evaluated and may use arbitrary symbols
 * including the arguments.
 * 
 * The lambda special-form should return an Expression of type Procedure.
 * Once defined such a procedure can be called the same way as built-in
 * ones.
*/
Expression Expression::handle_lambda() {
  
  // tail must have 2 arguments or error
  if(m_tail.size() != 2){
    throw SemanticError("Error during evaluation: invalid number of arguments to lambda");
  }
  
  // tail[0] must be list of symbols
  if(!m_tail[0].head().isSymbol()){
    throw SemanticError("Error during evaluation: first argument to lambda not a symbol");
  }

  // Must convert tail[0] into a List of Symbols to store
  List params = { m_tail[0].head() };

  for(auto exp : m_tail[0].m_tail) {
    // Check each parameter is a symbol
	if(exp.head().isSymbol()){
	  params.push_back(exp);
	}
	else {
	  throw SemanticError("Error during evaluation: an argument to lambda is invalid");
	}
  }
  
  // Store tail[1] Expression for procedure
  Expression function(m_tail[1]);

  // Combine into one output Expression
  return Expression(params, function);
}

/*
 * (apply <procedure> <list>)
 * The built-in binary procedure apply has two arguments. The first argument
 * is a procedure, the second a list. It treats the elements of the list
 * as the arguments to the procedure, returning the result after evaluation.
 */
Expression Expression::handle_apply(Environment & env){
  
  // tail must have 2 arguments or error
  if(m_tail.size() != 2){
    throw SemanticError("Error during evaluation: invalid number of arguments in call to apply");
  }
  
  // tail[0] must be a symbol
  if( !(m_tail[0].isHeadSymbol() && m_tail[0].isTailEmpty()) ){
    throw SemanticError("Error during evaluation: first argument in call to apply is not a Symbol");
  }
  
  // Extract first piece of apply function
  Atom proc = m_tail[0].head();
  std::string s = proc.asSymbol();

  // tail[0] must be a built-in or user-defined procedure
  if( !(env.is_proc(proc) || env.is_anon_proc(proc) || (s == "apply") || (s == "map")
      || (s == "set-property") || (s == "get-property")) )
  {
    throw SemanticError("Error during evaluation: first argument in call to apply is not a Procedure");
  }

  // tail[1] must be a List of arguments
  if(!m_tail[1].isHeadList()){
    throw SemanticError("Error during evaluation: second argument in call to apply is not a List");
  }
  
  // Extract second piece of apply function
  List args = m_tail[1].asList();
  
  // Set up restructured AST in form: (<procedure> <arg list>)
  Expression result = Expression(proc);
  result.m_tail = args;
  
  // Evaluate result of applied procedure
  return result.eval(env);
}

/*
 * (map <procedure> <list>)
 * The built-in binary procedure map is similar to apply, but treats each
 * entry of the list as a separate argument to the procedure, returning a
 * list of the same size of results.
 */
Expression Expression::handle_map(Environment & env){
  
  // tail must have 2 arguments or error
  if(m_tail.size() != 2){
    throw SemanticError("Error during evaluation: invalid number of arguments in call to map");
  }
  
  // tail[0] must be a symbol
  if( !(m_tail[0].isHeadSymbol() && m_tail[0].isTailEmpty()) ){
    throw SemanticError("Error during evaluation: first argument in call to map is not a Symbol");
  }
  
  // Extract first piece of map function
  Atom proc = m_tail[0].head();
  std::string s = proc.asSymbol();

  // tail[0] must be a built-in or user-defined procedure
  if( !(env.is_proc(proc) || env.is_anon_proc(proc) || (s == "apply") || (s == "map")
      || (s == "set-property") || (s == "get-property")) )
  {
    throw SemanticError("Error during evaluation: first argument to map is not a Procedure");
  }

  // tail[1] must be a List of arguments
  if(!m_tail[1].isHeadList()){
    throw SemanticError("Error during evaluation: second argument to map is not a List");
  }
  
  // Extract second piece of map function
  List args = m_tail[1].asList();

  // Set up restructured AST in form: (list <expression> <expression> ...)
  Expression result(Atom("list"));

  // Apply the procedure to each argument
  for(auto & exp : args){
    // Create a new Expression in form: (<procedure> <argument>)
    Expression argApply(proc);
    argApply.m_tail = {exp};
    
    // Add it to the AST
    result.m_tail.push_back(argApply);
  }
  
  // Evaluate modified AST and return result
  return result.eval(env);
}

/*
 * (set-property <String> <Expression> <Expression>)
 * set-property is a tertiary procedure taking a String expression as it's first
 * argument (the key), an arbitrary expression as it's second argument (the value),
 * and an Expression as the third argument. The procedure should add or reset an
 * entry to the third argument's property list, after evaluating it, with a key
 * equal to the first argument and value that results from evaluating the second
 * argument. The resulting expression, with the modified property list, is returned.
 * The property-list has no effect on how the expression is printed.
 * 
 * Note the value argument to set-property is evaluated before adding it to the
 * property list, but there are no side effects to the global environment
 * (similar to lambdas).
 */
Expression Expression::set_property(Environment & env)
{
  // tail must have 3 arguments or error
  if(m_tail.size() != 3){
    throw SemanticError("Error: invalid number of arguments in call to set-property");
  }
  
  // tail[0] must be a String literal
  if(!m_tail[0].isHeadString()){
    throw SemanticError("Error: first argument in call to set-property not a String");
  }
  String key = m_tail[0].head().asString();
  
  // Copy construct a new temporary Environment for evaluation
  Environment tempEnv(env);
  
  // Evaluate value Expression and copy result
  Expression value = m_tail[1].eval(tempEnv);

  // Evaluate main Expression and copy result (including m_props)
  Expression result = m_tail[2].eval(env);

	// Add to property List
	result.setProperty(key, value);

  // Return copied Expression with modified property list
  return result;
}

/*
 * (get-property <String> <Expression>)
 * get-property is a binary procedure taking a String expression as its
 * first argument (the key) and an arbitrary expression as the second
 * argument. The procedure returns the expression associated with the first
 * argument of the expression in the second argument, or returns an Expression
 * of type None if they key does not exist in the property list.
 */
Expression Expression::get_property(Environment & env)
{
  // tail must have 2 arguments or error
  if(m_tail.size() != 2){
    throw SemanticError("Error: invalid number of arguments in call to get-property");
  }
  
  // tail[0] must be a String literal
  if(!m_tail[0].isHeadString()){
    throw SemanticError("Error: first argument in call to get-property not a String");
  }
  String key = m_tail[0].head().asString();
  
  // tail[1] can be any valid Expression
  Expression exp = m_tail[1].eval(env);

  return exp.getProperty(key);
}

// this is a simple recursive version. the iterative version is more
// difficult with the ast data structure used (no parent pointer).
// this limits the practical depth of our AST
Expression Expression::eval(Environment & env){
  
  if( (m_tail.empty()) && (!isHeadList()) ){ // Base Case
    return handle_lookup(m_head, env);
  }
  // handle begin special-form
  else if(m_head.isSymbol() && m_head.asSymbol() == "begin"){
    return handle_begin(env);
  }
  // handle define special-form
  else if(m_head.isSymbol() && m_head.asSymbol() == "define"){
    return handle_define(env);
  }
  // handle lambda special-form
  else if(m_head.isSymbol() && m_head.asSymbol() == "lambda"){
    return handle_lambda();
  }
  // handle apply special-form/procedure
  else if(m_head.isSymbol() && m_head.asSymbol() == "apply"){
    return handle_apply(env);
  }
  // handle map special-form/procedure
  else if(m_head.isSymbol() && m_head.asSymbol() == "map"){
    return handle_map(env);
  }
  // handle set-property special-form/procedure
  else if(m_head.isSymbol() && m_head.asSymbol() == "set-property"){
    return set_property(env);
  }
  // handle get-property special-form/procedure
  else if(m_head.isSymbol() && m_head.asSymbol() == "get-property"){
    return get_property(env);
  }
  // else attempt to treat as procedure
  else{ 
    // First: Evaluate/simplify all subtrees
	std::vector<Expression> results;
    for(Expression::IteratorType it = m_tail.begin(); it != m_tail.end(); ++it){
      results.push_back(it->eval(env));
    }
    // Last: Apply sub-tree result to function pointer
    return apply(m_head, results, env);
  }
}

// Use values passed into Lambda Parameters by the anonymous function call to
// evaluate user-defined procedure, calculate resulting value
Expression Expression::call_lambda(Expression & lambda, const List & args, const Environment & env){
	
	// Make it easier to access arguments (I'll change later)
	List argsIn = Expression(args).asList();

	// Extract lambda pieces
	List params = lambda.m_tail[0].m_tail;
	Expression function = lambda.m_tail[1];

	// Function call must match number of defined arguments or error
    if(params.size() != argsIn.size()) {
		throw SemanticError("Error during evaluation: invalid number of arguments to call lambda function");
    }

	// Copy construct a new temporary Environment for Lambda evaluation
	Environment shadowEnv(env);

	// Set up restructured AST in form: (begin <expression> <expression> ...)
	Expression shadowAST(Atom("begin"));

	// Assign a value to each parameter
	for(size_t i = 0; i < params.size(); i++) {
		// Create a new special-form Expression: (define <symbol> <expression>)
		Expression argDef(Atom("define"));
		argDef.m_tail = {params[i], argsIn[i]};

		// Add it to the AST
		shadowAST.m_tail.push_back(argDef);
	}

	// Lastly, add the stored function definition
	shadowAST.m_tail.push_back(function);
	
	// Evaluate modified AST in Shadow and return result to the Main Environment
	return shadowAST.eval(shadowEnv);
}

std::ostream & operator<<(std::ostream & out, const Expression & exp){

  if(exp.head().isNone()){
    out << exp.head();
    return out;
  }

  out << "(";
  
  if( (!exp.isHeadList()) && (!exp.isHeadLambda()) ){
    out << exp.head();
	if(!exp.isTailEmpty()){
	  out << " ";
	}
  }
  
  // Print each tail Expression preceded by a space, except for the first entry
  for(auto e = exp.tailConstBegin(); e != exp.tailConstEnd(); ++e){
	if(e != exp.tailConstBegin()){
	  out << " ";
	}
	out << *e;
  }

  out << ")";

  return out;
}

bool Expression::operator==(const Expression & exp) const noexcept{

  bool result = (m_head == exp.m_head);

  result = result && (m_tail.size() == exp.m_tail.size());

  if(result){ // Recursively compare each of the tail expressions
    for(auto leftExp = m_tail.begin(), rightExp = exp.m_tail.begin();
	    (leftExp != m_tail.end()) && (rightExp != exp.m_tail.end());
	    ++leftExp, ++rightExp)
    { // Might have to beef this up for new Exp Types
      result = result && (*leftExp == *rightExp);
    }
  }

  return result;
}

bool operator!=(const Expression & left, const Expression & right) noexcept{

  return !(left == right);
}
