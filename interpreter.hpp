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

// define thread-safe message queues
//typedef std::string InputMessage;
//typedef Expression OutputMessage;

//typedef MessageQueue<InputMessage> InputQueue;
//typedef MessageQueue<OutputMessage> OutputQueue;

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
	
	/// Open the start-up file and evaluate the program
	void startup();

	/// Main thread function that polls the shared Input Message Queue until exit message is read
	void threadEvalLoop();

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
