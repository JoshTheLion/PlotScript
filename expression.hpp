/*! \file expression.hpp
Defines the Expression type and assiciated functions.
 */
#ifndef EXPRESSION_HPP
#define EXPRESSION_HPP

#include "token.hpp"
#include "atom.hpp"

#include <vector>
#include <string>
#include <utility>
#include <map>

// forward declare Environment
class Environment;

/*! \class Expression
\brief An expression is a tree of Atoms.

An expression is an atom called the head followed by a (possibly empty) 
list of expressions called the tail.
 */
class Expression {
public:

  // Labels for the formatted data containers stored in formatted Expressions
  typedef std::string String;
  typedef std::vector<Expression> List;
  typedef std::pair<List, Expression> Lambda;
  
  // convenience typedef
  typedef std::vector<Expression>::const_iterator ConstIteratorType;

  /// Default construct and Expression, whose type in NoneType
  Expression();

  /*! Construct an Expression with given Atom as head an empty tail
    \param atom the atom to make the head
  */
  Expression(const Atom & a);

  /// deep-copy construct an expression (recursive)
  Expression(const Expression & a);

  // List Type constructor
  Expression(const List & list);
  
  // Lambda Type constructor
  Expression(const List & parameters, const Expression & function);

  /// deep-copy assign an expression  (recursive)
  Expression & operator=(const Expression & a);

  /// return a reference to the head Atom
  Atom & head();

  /// return a const-reference to the head Atom
  const Atom & head() const;

  /// append Atom to tail of the expression
  void append(const Atom & a);

  /// return a pointer to the last expression in the tail, or nullptr
  Expression * tail();

  /// return a const-iterator to the beginning of tail
  ConstIteratorType tailConstBegin() const noexcept;

  /// return a const-iterator to the tail end
  ConstIteratorType tailConstEnd() const noexcept;

  /// convienience member to determine if tail is empty
  bool isTailEmpty() const noexcept;

  /// convienience member to determine if head atom is a number
  bool isHeadNumber() const noexcept;

  /// convienience member to determine if head atom is a symbol
  bool isHeadSymbol() const noexcept;

  /// convienience member to determine if head atom is a symbol
  bool isHeadComplex() const noexcept;

  /// convienience member to determine if head atom is a string literal
  bool isHeadString() const noexcept;

  /// convienience member to determine if head atom is a list
  bool isHeadList() const noexcept;

  /// convienience member to determine if head atom is a lambda
  bool isHeadLambda() const noexcept;

  /// value of Expression as a List vector, return empty List vector if not a List
  List asList() const noexcept;

  /// value of Expression as a Lambda pair (params, proc), return empty pair if not a Lambda
  Lambda asLambda() const noexcept;

  /// Evaluate expression using a post-order traversal (recursive)
  Expression eval(Environment & env);
  
  // Apply operation to evaluated expression
  Expression apply(const Atom & op, const List & args, const Environment & env);

  /// equality comparison for two expressions (recursive)
  bool operator==(const Expression & exp) const noexcept;
  
private:

  // the head of the expression
  Atom m_head;

  // the tail list is expressed as a vector for access efficiency
  // and cache coherence, at the cost of wasted memory.
  std::vector<Expression> m_tail;

  // Property list
  std::map<String, Expression> m_props;

  // convenience typedef
  typedef std::vector<Expression>::iterator IteratorType;
  
  // internal helper methods
  Expression handle_lookup(const Atom & head, const Environment & env);
  Expression handle_define(Environment & env);
  Expression handle_begin(Environment & env);
  Expression handle_lambda();
  
  // Built-In Functions
  Expression call_lambda(Expression & lambda, const List & args, const Environment & env);
  Expression handle_apply(Environment & env);
  Expression handle_map(Environment & env);
  Expression set_property(Environment & env);
  Expression get_property(Environment & env);
};

/// Render expression to output stream
std::ostream & operator<<(std::ostream & out, const Expression & exp);

/// inequality comparison for two expressions (recursive)
bool operator!=(const Expression & left, const Expression & right) noexcept;
  
#endif
