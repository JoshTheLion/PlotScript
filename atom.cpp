#include "atom.hpp"

#include <sstream>
#include <cctype>
#include <cmath>
#include <limits>

Atom::Atom(): m_type(NoneKind) {}

Atom::Atom(double value){

  setNumber(value);
}

Atom::Atom(const std::string & value) : Atom(){

  // Check if String literal
  if((value.front() == '\"') && ((value.back() == '\"'))){
    setString(value);
  }
  else{
    setSymbol(value);
  }
}

Atom::Atom(std::complex<double> value) : Atom(){

  setComplex(value);
}

Atom::Atom(const Token & token): Atom(){
  
  // is token a number?
  double temp;
  std::istringstream iss(token.asString());
  
  if(iss >> temp){
    // check for trailing characters if >> succeeds
    if(iss.rdbuf()->in_avail() == 0){
      setNumber(temp);
    }
  }
  // make sure does not start with number
  else if(!std::isdigit(token.asString()[0])){
	// is token symbol, complex, or string literal?
	std::string tempS = token.asString();
	std::complex<double> IMAG = std::complex<double>(0.0, 1.0);

	if((tempS.length() == 1) && (tempS[0] == 'I')){
	  setComplex(IMAG);
	}
    else if((tempS[0] == '\"')){
      setString(tempS);
    }
	else{
      setSymbol(tempS);
	}
  }
}



Atom::Atom(const Atom & x): Atom(){
  if(x.isNumber()){
    setNumber(x.numberValue);
  }
  else if(x.isSymbol()){
    setSymbol(x.symbolValue);
  }
  else if(x.isComplex()){
	setComplex(x.complexValue);
  }
  else if (x.isString()) {
    setString(x.stringValue);
  }
}

Atom & Atom::operator=(const Atom & x){

  if(this != &x){
    if(x.m_type == NoneKind){
      m_type = NoneKind;
    }
    else if(x.m_type == NumberKind){
      setNumber(x.numberValue);
    }
    else if(x.m_type == SymbolKind){
      setSymbol(x.symbolValue);
    }
	else if (x.m_type == ComplexKind) {
	  setComplex(x.complexValue);
	}
    else if (x.m_type == StringKind) {
      setString(x.stringValue);
    }
  }
  return *this;
}
  
Atom::~Atom(){

  // we need to ensure the destructors are called
  if(m_type == SymbolKind){
    symbolValue.~basic_string();
  }
  if(m_type == ComplexKind){
    complexValue.~complex();
  }
  if (m_type == StringKind) {
    stringValue.~basic_string();
  }
}

bool Atom::isNone() const noexcept{
  return m_type == NoneKind;
}

bool Atom::isNumber() const noexcept{
  return m_type == NumberKind;
}

bool Atom::isSymbol() const noexcept{
  return m_type == SymbolKind;
}  

bool Atom::isComplex() const noexcept {
  return m_type == ComplexKind;
}

bool Atom::isString() const noexcept {
  return m_type == StringKind;
}

void Atom::setNumber(double value){

  m_type = NumberKind;
  numberValue = value;
}

void Atom::setSymbol(const std::string & value){

  // we need to ensure the destructor of the current symbol string is called
  if(m_type == SymbolKind){
    symbolValue.~basic_string();
  }
    
  m_type = SymbolKind;

  // copy construct in place
  new (&symbolValue) std::string(value);
}

void Atom::setComplex(std::complex<double> value) {

  // we need to ensure the destructor of the complex number is called
  if(m_type == ComplexKind){
    complexValue.~complex();
  }
    
  m_type = ComplexKind;

  // copy construct in place
  new (&complexValue) std::complex<double>(value);
}

void Atom::setString(const std::string & value){

  // we need to ensure the destructor of the current string literal is called
  if(m_type == StringKind){
    stringValue.~basic_string();
  }
    
  m_type = StringKind;

  // copy construct in place
  new (&stringValue) std::string(value);
}

double Atom::asNumber() const noexcept{

  return (m_type == NumberKind) ? numberValue : 0.0;  
}

std::string Atom::asSymbol() const noexcept{

  std::string result;

  if(m_type == SymbolKind){
    result = symbolValue;
  }

  return result;
}

std::complex<double> Atom::asComplex() const noexcept {

	std::complex<double> result = (0.0);

	if(m_type == ComplexKind){
		result = complexValue;
	}
	else if(m_type == NumberKind){ // Shortcut to convert calculation results
		result = std::complex<double>(numberValue, 0.0);
	}

	return result;
}

std::string Atom::asString() const noexcept{

  std::string result;

  if(m_type == StringKind){
    result = stringValue;
  }

  return result;
}

bool Atom::operator==(const Atom & right) const noexcept{
  
  if(m_type != right.m_type) return false;

  switch(m_type){
  case NoneKind:
    if(right.m_type != NoneKind) return false;
    break;
  case NumberKind:
    {
      if(right.m_type != NumberKind) return false;
      double dleft = numberValue;
      double dright = right.numberValue;
      double diff = fabs(dleft - dright);
      if(std::isnan(diff) ||
	 (diff > std::numeric_limits<double>::epsilon())) return false;
    }
    break;
  case SymbolKind:
    {
      if(right.m_type != SymbolKind) return false;

      return symbolValue == right.symbolValue;
    }
    break;
  case ComplexKind:
    {
	  if(right.m_type != ComplexKind) return false;

	  return complexValue == right.complexValue;
    }
    break;
  case StringKind:
    {
      if(right.m_type != StringKind) return false;

      return stringValue == right.stringValue;
    }
    break;
  default:
    return false;
  }

  return true;
}

bool operator!=(const Atom & left, const Atom & right) noexcept{
  
  return !(left == right);
}


std::ostream & operator<<(std::ostream & out, const Atom & a){

  if(a.isNumber()){
    out << a.asNumber();
  }
  if(a.isSymbol()){
    out << a.asSymbol();
  }
  if (a.isComplex()){
	out << a.asComplex().real() << "," << a.asComplex().imag();
  }
  if(a.isString()){
    out << a.asString();
  }
  return out;
}
