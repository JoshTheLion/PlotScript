#include "expression.hpp"

#include <sstream>
#include <list>
#include <vector>

#include "environment.hpp"
#include "semantic_error.hpp"

Expression::Expression(){}

Expression::Expression(const Atom & a){

  m_head = a;
}

// recursive copy
Expression::Expression(const Expression & a){

  m_head = a.m_head;
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
  m_tail.push_back(Expression(parameters)); 
  m_tail.push_back(function);
}

Expression & Expression::operator=(const Expression & a){

  // prevent self-assignment
  if(this != &a){
    m_head = a.m_head;
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

bool Expression::isHeadList() const noexcept{
  return ((m_head.isSymbol()) && (m_head.asSymbol() == "list"));
}

bool Expression::isHeadLambda() const noexcept{
  return ((m_head.isSymbol()) && (m_head.asSymbol() == "lambda"));
}

Expression::List Expression::asList() const noexcept{
  
  List result;
  
  if(isHeadList()){
	  result = m_tail;
  }

  return result;
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

Expression apply(const Atom & op, const std::vector<Expression> & args, const Environment & env){

  // head must be a symbol
  if(!op.isSymbol()){
    throw SemanticError("Error during evaluation: procedure name not symbol");
  }
  
  if(env.is_exp(op)){
    Expression mappedExp = env.get_exp(op);
	if(mappedExp.isHeadLambda()){
	  // Shadow Method
	  throw SemanticError("Error: Josh is a genious!");
	}
  }

  // must map to a proc
  if(!env.is_proc(op)){
    throw SemanticError("Error during evaluation: symbol does not name a procedure");
  }
  
  // map from symbol to proc
  Procedure proc = env.get_proc(op);
  
  // call proc with args
  return proc(args);
}

Expression Expression::handle_lookup(const Atom & head, const Environment & env){
    if(head.isSymbol()){ // if symbol is in env return value
      if(env.is_exp(head)){
	    return env.get_exp(head);
      }
      else{
	    throw SemanticError("Error during evaluation: unknown symbol");
      }
    }
    else if(head.isNumber() || head.isComplex()){ // if literal is in env return value
      return Expression(head);
    }
    else{
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

  // tail must have size 3 or error
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
  
  if(env.is_proc(m_head)){
    throw SemanticError("Error during evaluation: attempt to redefine a built-in procedure");
  }
	
  // eval tail[1]
  Expression result = m_tail[1].eval(env);

  if(env.is_exp(m_head)){
    throw SemanticError("Error during evaluation: attempt to redefine a previously defined symbol");
  }
    
  //and add to env
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
Expression Expression::handle_lambda(/*Environment & env*/) {
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
	  throw SemanticError("Error during evaluation: first argument to lambda is invalid");
	}
  }
  
  // Store tail[1] Expression for procedure
  Expression function(m_tail[1]);

  // Combine into one output Expression
  return Expression(params, function);
}

// this is a simple recursive version. the iterative version is more
// difficult with the ast data structure used (no parent pointer).
// this limits the practical depth of our AST
Expression Expression::eval(Environment & env){
  
  if( (m_tail.empty()) && (!isHeadList()) ){ //Base Case
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
    return handle_lambda(/*env*/);
  }
  // else attempt to treat as procedure
  else{ 
    // First: Evaluate/simplify all subtrees
	std::vector<Expression> results;
    for(Expression::IteratorType it = m_tail.begin(); it != m_tail.end(); ++it){
      results.push_back(it->eval(env));
    }// Last: Apply function pointer to sub-tree result
    return apply(m_head, results, env);
  }
}


std::ostream & operator<<(std::ostream & out, const Expression & exp){

  out << "(";
  
  if( (!exp.isHeadList()) && (!exp.isHeadLambda()) ){
    out << exp.head();
	if(!exp.isTailEmpty()){
	  out << " ";
	}
  }
  
  // Print each List entry preceded by a space, except for the first entry
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

  if(result){
    for(auto lefte = m_tail.begin(), righte = exp.m_tail.begin();
	(lefte != m_tail.end()) && (righte != exp.m_tail.end());
	++lefte, ++righte){
      result = result && (*lefte == *righte);
    }
  }

  return result;
}

bool operator!=(const Expression & left, const Expression & right) noexcept{

  return !(left == right);
}
