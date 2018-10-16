/*! \file atom.hpp
Defines the Atom type and associated functions.
 */
#ifndef ATOM_HPP
#define ATOM_HPP

#include "token.hpp"

#include <complex>
#include <string>

/*! \class Atom
\brief A variant type that may be a Number or Symbol or Complex or the default type None.

This class provides value semantics.
*/
class Atom {
public:

  /// Construct a default Atom of type None
  Atom();

  /// Construct an Atom of type Number with value
  Atom(double value);

  /// Construct an Atom of type Symbol or String with value
  Atom(const std::string & value);

  /// Construct an Atom of type Complex with value
  Atom(std::complex<double> value);

  /// Construct an Atom directly from a Token
  Atom(const Token & token);

  /// Copy-construct an Atom
  Atom(const Atom & x);

  /// Assign an Atom
  Atom & operator=(const Atom & x);

  /// Atom destructor
  ~Atom();

  /// predicate to determine if an Atom is of type None
  bool isNone() const noexcept;

  /// predicate to determine if an Atom is of type Number
  bool isNumber() const noexcept;

  /// predicate to determine if an Atom is of type Symbol
  bool isSymbol() const noexcept;

  /// predicate to determine if an Atom is of type Complex
  bool isComplex() const noexcept;

  /// predicate to determine if an Atom is of type String
  bool isString() const noexcept;

  /// value of Atom as a Number literal, return 0 if not a Number
  double asNumber() const noexcept;

  /// value of Atom as a Symbol, returns empty-string if not a Symbol
  std::string asSymbol() const noexcept;

  /// value of Atom as a Complex number, return 0 if not a Complex
  std::complex<double> asComplex() const noexcept;

  /// value of Atom as a String literal, returns empty-string if not a String
  std::string asString() const noexcept;

  /// equality comparison based on type and value
  bool operator==(const Atom & right) const noexcept;

private:

  // internal enum of known types
  enum Type {NoneKind, NumberKind, SymbolKind, ComplexKind, StringKind};

  // track the type
  Type m_type;

  // values for the known types. Note the use of a union requires care
  // when setting non POD values (see setSymbol)
  union { // A union is a special class type that can hold only one of its non-static data members at a time.
    double numberValue;
    std::string symbolValue;
	std::complex<double> complexValue;
    std::string stringValue;
  };

  // helper to set type and value of Number
  void setNumber(double value);

  // helper to set type and value of Symbol
  void setSymbol(const std::string & value);
  
  // helper to set type and value of Complex
  void setComplex(std::complex<double> value);

  // helper to set type and value of String
  void setString(const std::string & value);
};

/// inequality comparison for Atom
bool operator!=(const Atom &left, const Atom & right) noexcept;

/// output stream rendering
std::ostream & operator<<(std::ostream & out, const Atom & a);

#endif
