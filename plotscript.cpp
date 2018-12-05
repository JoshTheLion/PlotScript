#include <string>
#include <sstream>
#include <iostream>
#include <fstream>
#include <thread>

#include "interpreter.hpp"
#include "semantic_error.hpp"
#include "startup_config.hpp"
#include "message_queue.hpp"
#include "message.hpp"

//typedef std::string InputMessage;
//typedef Expression OutputMessage;

void prompt(){
  std::cout << "\nplotscript> ";
}

std::string readline(){
  std::string line;
  std::getline(std::cin, line);

  return line;
}

void error(const std::string & err_str){
  std::cerr << "Error: " << err_str << std::endl;
}

void info(const std::string & err_str){
  std::cout << "Info: " << err_str << std::endl;
}

int startup(Interpreter & interp){
  
  std::ifstream ifs(STARTUP_FILE);
  
  if(!ifs){
    error("Could not open file for reading.");
    return EXIT_FAILURE;
  }
  
  if(!interp.parseStream(ifs)){
    ifs.close();
    error("Invalid Program. Could not parse.");
    return EXIT_FAILURE;
  }
  else{
    try{
      Expression exp = interp.evaluate();
    }
    catch(const SemanticError & ex){
      ifs.close();
      std::cerr << ex.what() << std::endl;
      return EXIT_FAILURE;
    }	
  }
  
  ifs.close();
  return EXIT_SUCCESS;
}

int eval_from_stream(std::istream & stream){

  Interpreter interp;
  
  /*** Evaluate startup.pls ***/
  if(startup(interp) != 0){
    error("Invalid Startup Program.");
    return EXIT_FAILURE;
  }

  if(!interp.parseStream(stream)){
    error("Invalid Program. Could not parse.");
    return EXIT_FAILURE;
  }
  else{
    try{
      Expression exp = interp.evaluate();
      std::cout << exp << std::endl;
    }
    catch(const SemanticError & ex){
      std::cerr << ex.what() << std::endl;
      return EXIT_FAILURE;
    }	
  }

  return EXIT_SUCCESS;
}

int eval_from_file(std::string filename){
      
  std::ifstream ifs(filename);
  
  if(!ifs){
    error("Could not open file for reading.");
    return EXIT_FAILURE;
  }
  
  return eval_from_stream(ifs);
}

int eval_from_command(std::string argexp){

  std::istringstream expression(argexp);

  return eval_from_stream(expression);
}

bool isRunning(const std::thread * kernel) noexcept{
	
	//return kernel.joinable();
	if(kernel != nullptr){
		return kernel->joinable();
	}
	return false;
}

// Start an interpreter kernel in a separate thread.
// It should have no effect if a thread is already running.
// The number of threads used should be two after this command.
void start(std::thread * kernel, Interpreter * interp){

	if(!isRunning(kernel)){
		*kernel = std::thread(&Interpreter::threadEvalLoop, std::ref(*interp));
	}
}

// Stop a running interpreter kernel. It should have no effect if a
// thread is already stopped.
// The number of threads used should be one after this command.
void stop(std::thread * kernel, MessageQueue<Message> * inQ){
	
	if(isRunning(kernel)){
		inQ->push(Message(Message::Type::StringType, "%stop"));
		kernel->join();		// Wait for thread execution to catch up
		//delete kernel;		// De-allocate thread object heap memory
		kernel = nullptr;	// Remove old memory address stored
	}
}

// Stop and reset a running interpreter kernel to the default state,
// clearing environment. It should then start a new running kernel.
// The number of threads used should be two after this command.
void reset(std::thread * kernel, Interpreter * interp, MessageQueue<Message> * inQ){
	
	if(isRunning(kernel)){
		inQ->push(Message(Message::Type::StringType, "%reset"));
		kernel->join();		// Wait for thread execution to catch up
		//kernel->~thread();		// De-allocate thread object heap memory
		kernel = nullptr;	// Remove old memory address stored
	}
	
	if(!isRunning(kernel)){
		*kernel = std::thread(&Interpreter::threadEvalLoop, std::ref(*interp));
	}
}

// If a user enters a plotscript expression when the interpreter is not
// running, display the error message: "Error: interpreter kernel not running".

// A REPL is a repeated read-eval-print loop
void repl(){
  
	MessageQueue<Message> inputQueue;
	MessageQueue<Message> outputQueue;

	// Initialize Interpreter object and check results of startup
	Interpreter interp(&inputQueue, &outputQueue);
	
	//if(!outputQueue.empty()){
	//	Message result;
	//	if(outputQueue.try_pop(result)){
	//		try{
	//			Expression exp = result.getExp();
	//			std::cout << exp << std::endl;
	//		}
	//		catch (const SemanticError & ex){
	//			std::cerr << ex.what() << std::endl;
	//		}
	//	}
	//}

	std::thread * kernelThread = nullptr;
	start(kernelThread, &interp);

  while(!std::cin.eof()){
    
    prompt();
		std::string line = readline();
    
    if(line.empty()) continue;
		
		// Check for special kernel control commands?
		if(line == "%exit"){
			stop(kernelThread, &inputQueue);
			break;
		}
		else if(line == "%start"){
			start(kernelThread, &interp);
		}
		else if(line == "%stop"){
			stop(kernelThread, &inputQueue);
		}
		else if(line == "%reset"){
			reset(kernelThread, &interp, &inputQueue);
		}
		else if(!isRunning(kernelThread)){
			error("interpreter kernel not running");
			continue;
		}

		// Push message to input queue
		Message message(Message::Type::StringType, line);
		inputQueue.push(message);

		// Asynchronously receive output results to display
		Message result;
		outputQueue.wait_and_pop(result);
		
		try{
      Expression exp = result.getExp();
			std::cout << exp << std::endl;
    }
    catch(const SemanticError & ex){
      std::cerr << ex.what() << std::endl;
    }
	}
	
	// Double-check the Interpreter kernel was told to stop
	stop(kernelThread, &inputQueue);

	// Double-check current # of threads is 1 (how can I do this?)

	// End of program
}

int main(int argc, char *argv[])
{  
  if(argc == 2){
    return eval_from_file(argv[1]);
  }
  else if(argc == 3){
    if(std::string(argv[1]) == "-e"){
      return eval_from_command(argv[2]);
    }
    else{
      error("Incorrect number of command line arguments.");
    }
  }
  else{
    repl();
  }
    
  return EXIT_SUCCESS;
}
