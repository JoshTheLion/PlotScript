#include "catch.hpp"

#include "expression.hpp"

TEST_CASE( "Test default expression", "[expression]" ) {

  Expression exp;

  REQUIRE(!exp.isHeadNumber());
  REQUIRE(!exp.isHeadSymbol());
  REQUIRE(!exp.isHeadComplex());
}

TEST_CASE( "Test double expression", "[expression]" ) {

  Expression exp(6.023);

  REQUIRE(exp.isHeadNumber());
  REQUIRE(!exp.isHeadSymbol());
  REQUIRE(!exp.isHeadComplex());
}


TEST_CASE( "Test symbol expression", "[expression]" ) {

  Expression exp(Atom("asymbol"));

  REQUIRE(!exp.isHeadNumber());
  REQUIRE(exp.isHeadSymbol());
  REQUIRE(!exp.isHeadComplex());
}

TEST_CASE( "Test complex expression", "[expression]" ) {

  Expression exp(Atom(std::complex<double>(0.0, 1.0)));

  REQUIRE(!exp.isHeadNumber());
  REQUIRE(!exp.isHeadSymbol());
  REQUIRE(exp.isHeadComplex());
}

