#ifndef _MESSAGE_HPP_
#define _MESSAGE_HPP_

#include "expression.hpp"
#include "semantic_error.hpp"

#include <stdexcept>
#include <exception>
#include <iostream>
#include <string>

/* This class is to be used by the Interpreter to encapsulate the
 * input Expression string sent through the input MessageQueue by the
 * main thread (TUI or GUI). It is also used to encapsulate the Expression
 * or Error result of evaluating each input Message, and pass it back
 * through the output MessageQueue to the main thread to display */
class Message
{
	// Convenience typedef labels
	typedef std::exception_ptr Error;
	typedef std::string String;

public:
	
	// constructors for use in container push, assignment, and output
	Message() : type(NoneType){};
	Message(const String & s) : type(StringType), stringValue(s){};
	Message(const Expression & e) : type(ExpressionType), expValue(e){};
	Message(const Error & e) : type(ErrorType), errValue(e){};
	
	// assignment needed for wait_and_pop
	Message & operator=(const Message & x){
		// prevent self-assignment
		if(!(this == &x)){
			if(x.type == NoneType){
				type = NoneType;
			}
			else if(x.type == StringType){
				setString(x.stringValue);
			}
			else if (x.type == ExpressionType){
				setExp(x.expValue);
			}
			else if (x.type == ErrorType){
				setErr(x.errValue);
			}
		}
		return *this;
	}
	
	void setString(const String & value){
		type = StringType;
		stringValue = value;
	}
	
	void setExp(const Expression & value){
		type = ExpressionType;
		expValue = value;
	}

	void setErr(const Error & value){
		type = ErrorType;
		errValue = value;
	}

	// opens the message and get value, similar to calling future.get()
	Expression getExp(){
		Expression result;
		if(type == ExpressionType) { result = expValue; }
		else if(type == ErrorType) { std::rethrow_exception(errValue); }
		else if (type == StringType) { throw SemanticError(stringValue); }
		return result;
	};

	String getString(){
		String result;
		if (type == StringType) { result = stringValue; }
		return result;
	};

	bool operator==(const Message & right) const noexcept{

		if (type != right.type) return false;

		switch (type){
		case NoneType:
			if (right.type != NoneType) return false;
			break;
		case StringType:
			{
				if (right.type != StringType) return false;
				return stringValue == stringValue;
			}
			break;
		case ExpressionType:
			{
				if (right.type != ExpressionType) return false;
				return expValue == right.expValue;
			}
			break;
		case ErrorType:
			{
				if (right.type != ErrorType) return false;
				return errValue == right.errValue;
			}
			break;
		default:
			return false;
		}

		return true;
	}

private:
	
	// internal enum of known types
	enum InterpResultType { NoneType, StringType, ExpressionType, ErrorType };

	// track the type
	InterpResultType type;
	
	// values for the known types
	String stringValue;
	Expression expValue;
	Error errValue;
	//SemanticError err;
};

//bool operator!=(const Message & left, const Message & right) noexcept{
//
//	return !(left == right);
//}

#endif // _MESSAGE_HPP_
