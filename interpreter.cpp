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

void Interpreter::startup()
{
  std::ifstream ifs(STARTUP_FILE);
	std::string strResult;
	Message result;

  if( (!ifs) && (outputQ != nullptr) ){
    strResult = "Error: Could not open startup file for reading.";
		//outputQ->push(Message(Message::Type::ErrorType, strResult));
  }

	// Only send message for evaluation errors
	result = evalStream(ifs);
	if( result.isError() && (outputQ != nullptr) ){
		//outputQ->push(result);
	}

	ifs.close();
}

//void Interpreter::operator()() const
//{
//	while(true){
//
//		// Take a unit of work from the input queue
//		Message line;
//		inputQ->wait_and_pop(line);
//
//		// Check for special kernel control commands
//		if(line.getString() == "%stop") break;
//		if(line.getString() == "%exit") break;
//		if(line.getString() == "%reset"){
//			// Clear and reset the Environment
//			env.reset();
//			startup();
//			break;
//		}
//		
//		// Process input and add result to output MessageQueue
//		std::istringstream inStream(line.getString());
//		Message result = evalStream(inStream);/*** Not thread-safe ***/
//		outputQ->push(result);
//	}
//}


void Interpreter::threadEvalLoop()
{
	while(true){

		// Take a unit of work from the input queue
		Message line;
		inputQ->wait_and_pop(line);

		// Check for special kernel control commands
		if(line.getString() == "%stop") break;
		if(line.getString() == "%exit") break;
		if(line.getString() == "%reset"){
			// Clear and reset the Environment
			env.reset();
			startup();
			break;
		}
		
		// Process input and add result to output MessageQueue
		std::istringstream inStream(line.getString());
		//Message result = evalStream(inStream);/*** Not thread-safe ***/
		outputQ->push(Message(evalStream(inStream)));
	}
	// End of Program
}

Message Interpreter::evalStream(std::istream & stream){
	
	std::ostringstream outStream;
	std::string errResult;
	Expression expResult;
	Message result;

	if(!parseStream(stream)){
		errResult = "Error: Invalid Expression. Could not parse.";
		result = Message(Message::Type::ErrorType, errResult);
	}
	else{
		try{
			expResult = evaluate();
			result = Message(Message::Type::ExpressionType, expResult);
		}
		catch(const SemanticError & ex){
			outStream << ex.what();
			errResult = outStream.str();
			result = Message(Message::Type::ErrorType, errResult);
		}
	}
	return result;
}


/***********************************************************************
	Backwards-Compatibility Base Code Methods (Don't Touch)
**********************************************************************/
bool Interpreter::parseStream(std::istream & expression) noexcept{

  TokenSequenceType tokens = tokenize(expression);

  ast = parse(tokens);

  return (ast != Expression());
};


Expression Interpreter::evaluate(){

  return ast.eval(env);
}
