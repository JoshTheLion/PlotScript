#include "interpreter.hpp"

// system includes
#include <stdexcept>

// module includes
#include "token.hpp"
#include "parse.hpp"
#include "expression.hpp"
#include "environment.hpp"
#include "semantic_error.hpp"

bool Interpreter::parseStream(std::istream & expression) noexcept{

  TokenSequenceType tokens = tokenize(expression);

  ast = parse(tokens);

  return (ast != Expression());
};
				     

Expression Interpreter::evaluate(){

  Expression result;
    
  try{
    result = ast.eval(env);
  }
  catch(const SemanticError & ex){
    throw;
  }
  
  return result;
}
