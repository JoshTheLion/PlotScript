#include "token.hpp"

// system includes
#include <cctype>
#include <iostream>

// define constants for special characters
const char OPENCHAR = '(';
const char CLOSECHAR = ')';
const char COMMENTCHAR = ';';
const char STRINGCHAR = '\"';

Token::Token(TokenType t): m_type(t){}

Token::Token(const std::string & str): m_type(STRING), value(str) {}

Token::TokenType Token::type() const{
  return m_type;
}

std::string Token::asString() const{
  switch(m_type){
  case OPEN:
    return "(";
  case CLOSE:
    return ")";
  case STRING:
    return value;
  }
  return "";
}


// add token to sequence unless it is empty, clears token
void store_ifnot_empty(std::string & token, TokenSequenceType & seq){
  if(!token.empty()){
    seq.emplace_back(token);
    token.clear();
  }
}

TokenSequenceType tokenize(std::istream & seq){
  TokenSequenceType tokens;
  std::string token;
  
  while(true){
    char c = seq.get();
    if(seq.eof()) break;
    
    if(c == COMMENTCHAR){
      // chomp until the end of the line
      while((!seq.eof()) && (c != '\n')){
	    c = seq.get();
      }
      if(seq.eof()) break;
    }
    else if(c == STRINGCHAR){
      // End and store previous token
      store_ifnot_empty(token, tokens);
      
      // Start new String literal Token
      token.push_back(c);
      
      // Begin reading in other String characters (If any)
      c = seq.get();
      while((!seq.eof()) && (c != '\"')){
        token.push_back(c);
        c = seq.get();
      }
      // Check that Token had both " characters before storing
      if(c == STRINGCHAR){
        token.push_back(c);
        store_ifnot_empty(token, tokens);
      }
      else{ // Error: Invalid String declaration, unable to parse
        token.clear();
        tokens.clear();
        break;
      }
    }
    else if(c == OPENCHAR){
      store_ifnot_empty(token, tokens);
      tokens.push_back(Token::TokenType::OPEN);
    }
    else if(c == CLOSECHAR){
      store_ifnot_empty(token, tokens);
      tokens.push_back(Token::TokenType::CLOSE);
    }
    else if(isspace(c)){ // Store Token and start next Token
      store_ifnot_empty(token, tokens);
    }
    else{
      token.push_back(c);
    }
  }
  store_ifnot_empty(token, tokens);

  return tokens;
}
