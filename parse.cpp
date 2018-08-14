#include "parse.hpp"

#include <stack>

bool setHead(Expression &exp, const Token &token) {

  Atom a(token);

  exp.head() = a;

  return !a.isNone();
}

bool append(Expression *exp, const Token &token) {

  Atom a(token);

  exp->append(a);

  return !a.isNone();
}

Expression parse(const TokenSequenceType &tokens) noexcept {

  Expression ast;

  // cannot parse empty
  if (tokens.empty())
    return Expression();

  bool athead = false;

  // stack tracks the last node created
  std::stack<Expression *> stack;

  std::size_t num_tokens_seen = 0;

  for (auto &t : tokens) {

    if (t.type() == Token::OPEN) {
      athead = true;
    } else if (t.type() == Token::CLOSE) {
      if (stack.empty()) {
        return Expression();
      }
      stack.pop();

      if (stack.empty()) {
        num_tokens_seen += 1;
        break;
      }
    } else {

      if (athead) {
        if (stack.empty()) {
          if (!setHead(ast, t)) {
            return Expression();
          }
          stack.push(&ast);
        } else {
          if (stack.empty()) {
            return Expression();
          }

          if (!append(stack.top(), t)) {
            return Expression();
          }
          stack.push(stack.top()->tail());
        }
        athead = false;
      } else {
        if (stack.empty()) {
          return Expression();
        }

        if (!append(stack.top(), t)) {
          return Expression();
        }
      }
    }
    num_tokens_seen += 1;
  }

  if (stack.empty() && (num_tokens_seen == tokens.size())) {
    return ast;
  }

  return Expression();
};
