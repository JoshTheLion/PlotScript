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

// A REPL is a repeated read-eval-print loop
void repl(){
  
	MessageQueue<Message> inputQueue;
	MessageQueue<Message> outputQueue;

	Interpreter interp(&inputQueue, &outputQueue);
	std::thread kernelThread(&Interpreter::threadEvalLoop, &interp);
	
	//std::thread * kernelThread;
	//kernelThread = new std::thread(&Interpreter::parseStream, &interp);

  while(!std::cin.eof()){
    
    prompt();
		std::string line = readline();
    
    if(line.empty()) continue;
		
		// Check for special kernel control commands?

		// Push message to input queue
		inputQueue.push(Message(line));

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
	
	// Tell the Interpreter kernel to stop
	inputQueue.push(Message("%stop"));
	kernelThread.join();

	// Double-check current # of threads is 1

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
