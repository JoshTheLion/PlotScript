/*! \file interpreter.hpp

The interpreter can parse from a stream into an internal AST and evaluate it.
It maintains an environment during evaluation.
 */

#ifndef INTERPRETER_HPP
#define INTERPRETER_HPP

// system includes
#include <iostream>
#include <fstream>
#include <istream>
#include <sstream>
#include <string>
#include <thread>

// module includes
#include "environment.hpp"
#include "expression.hpp"
#include "semantic_error.hpp"
//#include "startup_config.hpp"
#include "message_queue.hpp"
#include "message.hpp"

/*! \class Interpreter
\brief Class to parse and evaluate an expression (program)

Interpreter has an Environment, which starts at a default.
The parse method builds an internal AST.
The eval method updates Environment and returns last result.
*/
class Interpreter{
public:
	
	Interpreter();

	Interpreter(MessageQueue<Message> * inputQ, MessageQueue<Message> * outputQ);
	
	// Overloaded function call operator to start threads in
	//void Interpreter::operator()() const;

	/// Open the start-up file and evaluate the program
	void startup();

	/// Main thread function that polls the input MessageQueue until interrupt message is received
	void threadEvalLoop();
	
	/// Process the input message and return result message
	Message evalStream(std::istream & stream);

  /*! Parse into an internal Expression from a stream
    \param expression the raw text stream repreenting the candidate expression
    \return true on successful parsing 
   */
  bool parseStream(std::istream &expression) noexcept;

  /*! Evaluate the Expression by walking the tree, returning the result.
    \return the Expression resulting from the evaluation in the current environment
    \throws SemanticError when a semantic error is encountered
   */
  Expression evaluate();

private:
  
	Environment env;
	
	Expression ast;

	/// The thread-safe message queue channels for kernel I/O
	MessageQueue<Message> * inputQ;
	MessageQueue<Message> * outputQ;
  
};

#endif
