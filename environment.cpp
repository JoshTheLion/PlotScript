#include "environment.hpp"

#include <cassert>
#include <cmath>
#include <complex>

#include "environment.hpp"
#include "semantic_error.hpp"

/*********************************************************************** 
Helper Function(s)
**********************************************************************/

// predicate, the number of args is nargs
bool nargs_equal(const std::vector<Expression> & args, unsigned nargs){
  return args.size() == nargs;
}


/*********************************************************************** 
Each of the functions below have the signature that corresponds to the
typedef'd Procedure function pointer.
**********************************************************************/

// the default procedure always returns an expresison of type None
Expression default_proc(const std::vector<Expression> & args){
  args.size(); // make compiler happy we used this parameter
  return Expression();
};

Expression add(const std::vector<Expression> & args)
{
  // If any argument is complex, the result should be complex
  double realSum = 0.0;
  double imagSum = 0.0;
  bool has_complex = false; // Flag to determine result type

  for( auto & a :args){ // Loops over a range of values, whose types are automatically deduced (Like Java)
    if(a.isHeadNumber()){
      realSum += a.head().asNumber();
    }
	else if(a.isHeadComplex()){
      realSum += a.head().asComplex().real();
	  imagSum += a.head().asComplex().imag();
	  has_complex = true;
	}
    else{
      throw SemanticError("Error in call to add, argument not a number");
    }
  }
  
  if(has_complex){
	  return Expression(std::complex<double> (realSum, imagSum));
  }
  else{
	  return Expression(realSum);
  }
};

Expression mul(const std::vector<Expression> & args)
{
  // If any argument is complex, the result should be complex
  std::complex<double> result = (1.0);
  bool has_complex = false; // Flag to determine result type
  
  for( auto & a :args){ // Loops over a range of values, whose types are automatically deduced (Like Java)
    if(a.isHeadNumber()){
      result *= a.head().asNumber();
    }
	else if(a.isHeadComplex()){
      result *= a.head().asComplex();
	  has_complex = true;
	}
    else{
      throw SemanticError("Error in call to mul, argument not a number");
    }
  }

  if(has_complex){
	  return Expression(result);
  }
  else{
	  return Expression(result.real());
  }
};

Expression subneg(const std::vector<Expression> & args)
{
  // If any argument is complex, the result should be complex
  //std::complex<double> result = 0.0;
  double realResult = 0.0;
  double imagResult = 0.0;
  bool has_complex = false; // Flag to determine result type

  // preconditions
  if(nargs_equal(args,1)){
    if(args[0].isHeadNumber()){
      realResult = -args[0].head().asNumber();
    }
    else if(args[0].isHeadComplex()){
      realResult = -args[0].head().asComplex().real();
      imagResult = -args[0].head().asComplex().imag();
	  has_complex = true;
    }
    else{
      throw SemanticError("Error in call to negate: invalid argument.");
    }
  }
  else if(nargs_equal(args,2)){
    if( (args[0].isHeadNumber()) && (args[1].isHeadNumber()) ){
      realResult = args[0].head().asNumber() - args[1].head().asNumber();
    }
	else if( (args[0].isHeadComplex()) || (args[1].isHeadComplex()) ){
      realResult = args[0].head().asComplex().real() - args[1].head().asComplex().real();
      imagResult = args[0].head().asComplex().imag() - args[1].head().asComplex().imag();
	  has_complex = true;
    }
    else{      
      throw SemanticError("Error in call to subtraction: invalid argument.");
    }
  }
  else{
    throw SemanticError("Error in call to subtraction or negation: invalid number of arguments.");
  }

  if(has_complex){
	  return Expression(std::complex<double> (realResult, imagResult));
  }
  else{
	  return Expression(realResult);
  }
};

Expression div(const std::vector<Expression> & args)
{
  // If any argument is complex, the result should be complex
  std::complex<double> result = (1.0);
  bool has_complex = false; // Flag to determine result type

  if(nargs_equal(args,2)){
    if( (args[0].isHeadNumber()) && (args[1].isHeadNumber()) ){
      result = args[0].head().asNumber() / args[1].head().asNumber();
    }
	else if( (args[0].isHeadComplex()) || (args[1].isHeadComplex()) ){
      result = args[0].head().asComplex() / args[1].head().asComplex();
	  has_complex = true;
	}
    else{      
      throw SemanticError("Error in call to division: invalid argument.");
    }
  }
  else{
    throw SemanticError("Error in call to division: invalid number of arguments.");
  }
  
  if(has_complex){
	  return Expression(result);
  }
  else{
	  return Expression(result.real());
  }
};

Expression sqrt(const std::vector<Expression> & args)
{
	// The square root of a Complex argument or a negative Number argument
	std::complex<double> result = (0.0); 
	bool is_complex = false; // Flag to determine result type

	if (nargs_equal(args, 1)) {
		if ((args[0].isHeadNumber()) && (args[0].head().asNumber() >= 0)) {
			result = std::sqrt(args[0].head().asNumber());
		}
		else if ((args[0].isHeadComplex()) || (args[0].head().asNumber() < 0)) {
			result = std::sqrt(args[0].head().asComplex());
			is_complex = true;
		}
		else {
			throw SemanticError("Error in call to square root: invalid argument.");
		}
	}
	else {
		throw SemanticError("Error in call to square root: invalid number of arguments.");
	}
	
	if (is_complex) {
		return Expression(result);
	}
	else {
		return Expression(result.real());
	}
};

Expression a_pow_b(const std::vector<Expression> & args)
{
	// The result should be of type Number only when both arguments are of type Number
	std::complex<double> result = (0.0);
	bool is_complex = false; // Flag to determine result type
	
	if (nargs_equal(args, 2)) {
		if (args[0].isHeadNumber() && args[1].isHeadNumber()) {
			// Store values in local variable for readability
			double a = args[0].head().asNumber();
			double b = args[1].head().asNumber();
			
			// Check for known cmath error sources before calling function
			if ((a >= 0) && (b > 0)) {
				result = std::pow(a, b);
			}
			else {
				throw SemanticError("Error in call to a_pow_b: invalid argument.");
			}
		}
		else if ((args[0].isHeadComplex()) || (args[1].isHeadComplex())) {
			// Store values in local variable for readability
			std::complex<double> a = args[0].head().asComplex();
			std::complex<double> b = args[1].head().asComplex();
			result = std::pow(a, b);
			is_complex = true;
		}
		else {
			throw SemanticError("Error in call to a_pow_b: invalid argument.");
		}
	}
	else {
		throw SemanticError("Error in call to a_pow_b: invalid number of arguments.");
	}
	
	if (is_complex) {
		return Expression(result);
	}
	else {
		return Expression(result.real());
	}
};

Expression nat_log(const std::vector<Expression> & args) {

	double result = 0;

	if (nargs_equal(args, 1)) {
		// Check for known cmath error sources before calling function
		if (args[0].isHeadNumber() && (args[0].head().asNumber() >= 0)) {
			result = std::log(args[0].head().asNumber());
		}
		else {
			throw SemanticError("Error in call to natural log: invalid argument.");
		}
	}
	else {
		throw SemanticError("Error in call to natural log: invalid number of arguments.");
	}
	return Expression(result);
};

Expression sine(const std::vector<Expression> & args) {

	double result = 0;

	if (nargs_equal(args, 1)) {
		if (args[0].isHeadNumber()) { 
			result = std::sin(args[0].head().asNumber());
		}
		else { // Might have to limit domain with fmod(args, 2*pi)
			throw SemanticError("Error in call to sine: invalid argument.");
		}
	}
	else {
		throw SemanticError("Error in call to sine: invalid number of arguments.");
	}
	return Expression(result);
};

Expression cosine(const std::vector<Expression> & args) {

	double result = 0;

	if (nargs_equal(args, 1)) {
		if (args[0].isHeadNumber()) {
			result = std::cos(args[0].head().asNumber());
		}
		else { // Might have to limit domain with fmod(args, 2*pi)
			throw SemanticError("Error in call to cosine: invalid argument.");
		}
	}
	else {
		throw SemanticError("Error in call to cosine: invalid number of arguments.");
	}
	return Expression(result);
};

Expression tangent(const std::vector<Expression> & args) {

	double result = 0;

	if (nargs_equal(args, 1)) {
		if (args[0].isHeadNumber()) {
			result = std::tan(args[0].head().asNumber());
		}
		else { // Might have to limit domain with fmod(args, 2*pi)
			throw SemanticError("Error in call to tangent: invalid argument.");
		}
	}
	else {
		throw SemanticError("Error in call to tangent: invalid number of arguments.");
	}
	return Expression(result);
};


Expression get_real_num(const std::vector<Expression> & args)
{
  double result = 0.0;

  if(nargs_equal(args,1)){
    if(args[0].isHeadComplex()){
      result = args[0].head().asComplex().real();
    }
    else{      
      throw SemanticError("Error in call to get real part: invalid argument.");
    }
  }
  else{
    throw SemanticError("Error in call to get real part: invalid number of arguments.");
  }
  
  return Expression(result);
};

Expression get_imag_num(const std::vector<Expression> & args)
{
  double result = 0.0;

  if(nargs_equal(args,1)){
    if(args[0].isHeadComplex()){
      result = args[0].head().asComplex().imag();
    }
    else{      
      throw SemanticError("Error in call to get imaginary part: invalid argument.");
    }
  }
  else{
    throw SemanticError("Error in call to get imaginary part: invalid number of arguments.");
  }
  
  return Expression(result);
};

Expression get_mag(const std::vector<Expression> & args)
{
  // The magnitude (absolute value) of a Complex as a Number
  double result = 0.0;
  
  if(nargs_equal(args,1)){
    if(args[0].isHeadComplex()){
      result = std::abs(args[0].head().asComplex()); // abs is a non-member function of complex
    }
    else{      
      throw SemanticError("Error in call to get magnitude: invalid argument.");
    }
  }
  else{
    throw SemanticError("Error in call to get magnitude: invalid number of arguments.");
  }
  
  return Expression(result);
};

Expression get_arg(const std::vector<Expression> & args)
{
  // The argument (angle or phase) of a Complex as a Number in radians
  double result = 0.0;
  
  if(nargs_equal(args,1)){
    if(args[0].isHeadComplex()){
      result = std::arg(args[0].head().asComplex()); // arg is a non-member function of complex
    }
    else{      
      throw SemanticError("Error in call to get argument: invalid argument.");
    }
  }
  else{
    throw SemanticError("Error in call to get argument: invalid number of arguments.");
  }
  
  return Expression(result);
};

Expression get_conj(const std::vector<Expression> & args)
{
  // The conjugate of a Complex argument
	std::complex<double> result = (0.0);
  
  if(nargs_equal(args,1)){
    if(args[0].isHeadComplex()){
      result = std::conj(args[0].head().asComplex()); // conj is a non-member function of complex
    }
    else{      
      throw SemanticError("Error in call to get argument: invalid argument.");
    }
  }
  else{
    throw SemanticError("Error in call to get argument: invalid number of arguments.");
  }
  
  return Expression(result);
};

/*
In our version lists are created using a built-in procedure list, producing an
expression of type List, which may hold an arbitrary-sized, ordered, list of
expressions of any type, including List itself (i.e. it is recursive).
*/
Expression make_list(const std::vector<Expression> & args)
{
  // Deep copy new List Type Expression from args
  Expression results;
  results = Expression(args);

  return Expression(results);
};

//Add a built-in unary procedure first returning the first expression of the List
//argument. It is a semantic error if the expression is not a List or is empty.
Expression first(const std::vector<Expression> & args) { return Expression(); }

//Add a built-in unary procedure rest returning a list staring at the second element
//of the List argument up to and including the last element. It is a semantic error
//if the expression is not a List or is empty.
Expression rest(const std::vector<Expression> & args) { return Expression(); }

//Add a built-in unary procedure length returning the number of items in a List
//argument as a Number Expression. It is a semantic error if the expression is not
//a List.
Expression length(const std::vector<Expression> & args) { return Expression(); }

//Add a built-in binary procedure append that appends the expression of the second
//argument to the first List argument. It is a semantic error if the first argument
//is not a List.
Expression append(const std::vector<Expression> & args) { return Expression(); }

//Add a built-in binary procedure join that joins each of the List arguments into
//one list. It is a semantic error if any argument is not a List.
Expression join(const std::vector<Expression> & args) { return Expression(); }

//Add a built-in procedure range that produces a list of Numbers from a lower-bound
//(the first argument) to an upper-bound (the second argument) in positive increments
//specified by a third argument. It is a semantic error if any argument is not a
//Number, the first argument is not less than the second, or the increment is not
//strictly positive.
Expression range(const std::vector<Expression> & args) { return Expression(); }


/***********************************************************************
Built-In Symbols
**********************************************************************/

const double PI = std::atan2(0, -1);
const double EXP = std::exp(1);
const std::complex<double> IMAG = std::complex<double>(0.0, 1.0);

/***********************************************************************
Public Methods
**********************************************************************/

Environment::Environment(){

  reset();
}

bool Environment::is_known(const Atom & sym) const{
  if(!sym.isSymbol()) return false;
  
  return envmap.find(sym.asSymbol()) != envmap.end();
}

bool Environment::is_exp(const Atom & sym) const{
  if(!sym.isSymbol()) return false;
  
  auto result = envmap.find(sym.asSymbol());
  return (result != envmap.end()) && (result->second.type == ExpressionType);
}

Expression Environment::get_exp(const Atom & sym) const{

  Expression exp;
  
  if(sym.isSymbol()){
    auto result = envmap.find(sym.asSymbol());
    if((result != envmap.end()) && (result->second.type == ExpressionType)){
      exp = result->second.exp;
    }
  }

  return exp;
}

void Environment::add_exp(const Atom & sym, const Expression & exp){

  if(!sym.isSymbol()){
    throw SemanticError("Attempt to add non-symbol to environment");
  }
    
  // error if overwriting symbol map
  if(envmap.find(sym.asSymbol()) != envmap.end()){
    throw SemanticError("Attempt to overwrite symbol in environemnt");
  }

  envmap.emplace(sym.asSymbol(), EnvResult(ExpressionType, exp)); 
}

bool Environment::is_proc(const Atom & sym) const{
  if(!sym.isSymbol()) return false;
  
  auto result = envmap.find(sym.asSymbol());
  return (result != envmap.end()) && (result->second.type == ProcedureType);
}

Procedure Environment::get_proc(const Atom & sym) const{

  //Procedure proc = default_proc;

  if(sym.isSymbol()){
    auto result = envmap.find(sym.asSymbol());
    if((result != envmap.end()) && (result->second.type == ProcedureType)){
      return result->second.proc;
    }
  }

  return default_proc;
}

/*
Reset the environment to the default state. First remove all entries and
then re-add the default ones.
 */
void Environment::reset(){

  envmap.clear();
  
  // Built-In value of pi
  envmap.emplace("pi", EnvResult(ExpressionType, Expression(PI)));

  // Built-In value of Euler's Number
  envmap.emplace("e", EnvResult(ExpressionType, Expression(EXP)));

  // Built-In value of Complex symbol I
  envmap.emplace("I", EnvResult(ExpressionType, Expression(IMAG)));

  // Procedure: add;
  envmap.emplace("+", EnvResult(ProcedureType, add)); 

  // Procedure: subneg;
  envmap.emplace("-", EnvResult(ProcedureType, subneg)); 

  // Procedure: mul;
  envmap.emplace("*", EnvResult(ProcedureType, mul)); 

  // Procedure: div;
  envmap.emplace("/", EnvResult(ProcedureType, div)); 

  // Procedure: sqrt;
  envmap.emplace("sqrt", EnvResult(ProcedureType, sqrt));

  // Procedure: (^ a b);
  envmap.emplace("^", EnvResult(ProcedureType, a_pow_b));

  // Procedure: ln;
  envmap.emplace("ln", EnvResult(ProcedureType, nat_log));

  // Procedure: sin;
  envmap.emplace("sin", EnvResult(ProcedureType, sine));

  // Procedure: cos;
  envmap.emplace("cos", EnvResult(ProcedureType, cosine));

  // Procedure: tan;
  envmap.emplace("tan", EnvResult(ProcedureType, tangent));

  // Procedure: real;
  envmap.emplace("real", EnvResult(ProcedureType, get_real_num));

  // Procedure: imag;
  envmap.emplace("imag", EnvResult(ProcedureType, get_imag_num));

  // Procedure: mag;
  envmap.emplace("mag", EnvResult(ProcedureType, get_mag));

  // Procedure: arg;
  envmap.emplace("arg", EnvResult(ProcedureType, get_arg));

  // Procedure: conj;
  envmap.emplace("conj", EnvResult(ProcedureType, get_conj));

  // Procedure: list;
  envmap.emplace("list", EnvResult(ProcedureType, make_list));
}
