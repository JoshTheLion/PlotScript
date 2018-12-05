#include "interpreter.hpp"

// system includes
#include <stdexcept>
#include <iostream>

// module includes
#include "token.hpp"
#include "parse.hpp"
#include "expression.hpp"
#include "environment.hpp"
#include "semantic_error.hpp"
#include "startup_config.hpp"

Interpreter::Interpreter()
{
	inputQ = nullptr;
	outputQ = nullptr;
}

Interpreter::Interpreter(MessageQueue<Message> * inQ, MessageQueue<Message> * outQ)
{
	inputQ = inQ;
	outputQ = outQ;
	startup();
}

// Convert string->Expression for output messages
Expression errorExp(const std::string & err_str)
{
	Expression expResult(Atom("Error"));
	expResult.append(Expression(Atom(err_str)));
	//expResult.setProperty("Error", flag);
	return expResult;
}

void Interpreter::startup()
{
  std::ifstream ifs(STARTUP_FILE);
	std::ostringstream outStream;
	std::string strResult;
	Expression expResult;

  if(!ifs){
    expResult = errorExp("Could not open startup file for reading.");
  }
  
  if(!parseStream(ifs)){
    ifs.close();
		expResult = errorExp("Invalid Startup Program. Could not parse.");
  }
  else{
    try{
			expResult = evaluate();
			//outQ->push(expResult);
    }
    catch(const SemanticError & ex){
      ifs.close();
			outStream << "Invalid Startup Program: " << ex.what();
			strResult = outStream.str();
			expResult = errorExp(strResult);
    }
  }
	ifs.close();
  //outQ->push(expResult);
}

// Process Input Queue Messages
void Interpreter::threadEvalLoop()
{
	// Call start function
	while(true){

		// take a unit of work from the input queue
		Message line;
		inputQ->wait_and_pop(line);

		// Call stop function
		if (line.getString() == "%stop") break;

		std::istringstream inStream(line.getString());
		std::ostringstream outStream;
		std::string strResult;
		Expression expResult;

		if(!parseStream(inStream)){
			expResult = errorExp("Invalid Expression. Could not parse.");
		}
		else{
			try{
				expResult = evaluate();
			}
			catch(const SemanticError & ex){
				outStream << ex.what();
				strResult = outStream.str();
				expResult = errorExp(strResult);
			}
		}

		// put the result back into the output queue
		outputQ->push(Message(expResult));
	}
	// End of Program
}


bool Interpreter::parseStream(std::istream & expression) noexcept{

  TokenSequenceType tokens = tokenize(expression);

  ast = parse(tokens);

  return (ast != Expression());
};


Expression Interpreter::evaluate(){

  return ast.eval(env);
}
