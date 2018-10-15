#include "catch.hpp"

#include "expression.hpp"

TEST_CASE("Test Expression constructors", "[expression]")
{
  SECTION("Default type constructor")
  {
    Expression exp;
    
    REQUIRE(!exp.isHeadNumber());
    REQUIRE(!exp.isHeadSymbol());
    REQUIRE(!exp.isHeadComplex());
    REQUIRE(!exp.isHeadList());
    REQUIRE(!exp.isHeadLambda());
    REQUIRE( exp.isTailEmpty());
  }
  SECTION("Number type constructor", "[Number]")
  {
    Expression exp(1.0);

    REQUIRE( exp.isHeadNumber());
    REQUIRE(!exp.isHeadSymbol());
    REQUIRE(!exp.isHeadComplex());
    REQUIRE(!exp.isHeadList());
    REQUIRE(!exp.isHeadLambda());
    REQUIRE( exp.isTailEmpty());
  }
  SECTION("Symbol type constructor", "[Symbol]")
  {
    Expression exp(Atom("foo"));

    REQUIRE(!exp.isHeadNumber());
    REQUIRE( exp.isHeadSymbol());
    REQUIRE(!exp.isHeadComplex());
    REQUIRE(!exp.isHeadList());
    REQUIRE(!exp.isHeadLambda());
    REQUIRE( exp.isTailEmpty());
  }
  SECTION("Complex type constructor", "[Complex]")
  {
    Expression exp(Atom(std::complex<double>(0.0, 1.0)));

    REQUIRE(!exp.isHeadNumber());
    REQUIRE(!exp.isHeadSymbol());
    REQUIRE( exp.isHeadComplex());
    REQUIRE(!exp.isHeadList());
    REQUIRE(!exp.isHeadLambda());
    REQUIRE( exp.isTailEmpty());
  }
  SECTION("List type constructor", "[List]")
  {
    INFO("For empty list arg");

    Expression::List emptyList = { };
    Expression exp0(emptyList);

    REQUIRE(!exp0.isHeadNumber());
    REQUIRE( exp0.isHeadSymbol());
    REQUIRE(!exp0.isHeadComplex());
    REQUIRE( exp0.isHeadList());
    REQUIRE( exp0.isTailEmpty());

    INFO("For non-empty list arg");

    Expression::List dataList = { Expression(Atom("data")) };
    Expression exp1(dataList);

    REQUIRE(!exp1.isHeadNumber());
    REQUIRE( exp1.isHeadSymbol());
    REQUIRE(!exp1.isHeadComplex());
    REQUIRE( exp1.isHeadList());
    REQUIRE(!exp1.isTailEmpty());
  }
  SECTION("Lambda type constructor", "[Lambda]")
  {
    INFO("For default exp and empty list args");

    Expression defaultExp;
    Expression::List emptyList = { };
    Expression exp0(emptyList, defaultExp);

    REQUIRE(!exp0.isHeadNumber());
    REQUIRE( exp0.isHeadSymbol());
    REQUIRE(!exp0.isHeadComplex());
    REQUIRE(!exp0.isHeadList());
    REQUIRE( exp0.isHeadLambda());
    REQUIRE(!exp0.isTailEmpty());

    INFO("For Number exp and non-empty list args");

    Expression procExp(Atom(1));
    Expression::List paramList = { Expression(Atom("data")) };
    Expression exp1(paramList, procExp);

    REQUIRE(!exp1.isHeadNumber());
    REQUIRE( exp1.isHeadSymbol());
    REQUIRE(!exp1.isHeadComplex());
    REQUIRE(!exp1.isHeadList());
    REQUIRE( exp1.isHeadLambda());
    REQUIRE(!exp1.isTailEmpty());
  }
  SECTION("Copy constructor")
  {
    Expression exp1a(Atom(1.0));
    Expression exp2a(Atom("foo"));
    Expression exp3a(Atom(std::complex<double>(0.0, 1.0)));
    
    Expression::List list = { exp1a, exp2a, exp3a };
    Expression exp4a(list);

    Expression exp5a(list, exp3a);
    
    SECTION("Number type")
    {
      Expression exp1b = exp1a;

      REQUIRE( exp1b.isTailEmpty());
      REQUIRE( exp1b.isHeadNumber());
      REQUIRE(!exp1b.isHeadSymbol());
      REQUIRE(!exp1b.isHeadComplex());
      REQUIRE(!exp1b.isHeadList());
      REQUIRE(!exp1b.isHeadLambda());
    }
    SECTION("Symbol type")
    {
      Expression exp2b = exp2a;

      REQUIRE( exp2b.isTailEmpty());
      REQUIRE(!exp2b.isHeadNumber());
      REQUIRE( exp2b.isHeadSymbol());
      REQUIRE(!exp2b.isHeadComplex());
      REQUIRE(!exp2b.isHeadList());
      REQUIRE(!exp2b.isHeadLambda());
    }
    SECTION("Complex type")
    {
      Expression exp3b = exp3a;

      REQUIRE( exp3b.isTailEmpty());
      REQUIRE(!exp3b.isHeadNumber());
      REQUIRE(!exp3b.isHeadSymbol());
      REQUIRE( exp3b.isHeadComplex());
      REQUIRE(!exp3b.isHeadList());
      REQUIRE(!exp3b.isHeadLambda());
    }
    SECTION("List type")
    {
      Expression exp4b = exp4a;

      REQUIRE(!exp4b.isTailEmpty());
      REQUIRE(!exp4b.isHeadNumber());
      REQUIRE( exp4b.isHeadSymbol());
      REQUIRE(!exp4b.isHeadComplex());
      REQUIRE( exp4b.isHeadList());
      REQUIRE(!exp4b.isHeadLambda());
    }
    SECTION("Lambda type")
    {
      Expression exp5b = exp5a;

      REQUIRE(!exp5b.isTailEmpty());
      REQUIRE(!exp5b.isHeadNumber());
      REQUIRE( exp5b.isHeadSymbol());
      REQUIRE(!exp5b.isHeadComplex());
      REQUIRE(!exp5b.isHeadList());
      REQUIRE( exp5b.isHeadLambda());
    }
  }
}

TEST_CASE("Test Expression assignment", "[expression]") {

  Expression exp1a(Atom(1.0));
  Expression exp2a(Atom("foo"));
  Expression exp3a(Atom(std::complex<double>(0.0, 1.0)));

  Expression::List list1 = { exp1a, exp2a, exp3a };
  Expression exp4a(list1);

  Expression exp5a(list1, exp3a);

  SECTION("Assign Number type")
  {
    INFO("number to number");
    Expression exp1b(Atom(2.0));
    exp1b = exp1a;
    REQUIRE(exp1b.isHeadNumber());
    REQUIRE(exp1b.head().asNumber() == 1.0);
    
    INFO("number to symbol");
    Expression exp2b(Atom("foo"));
    exp2b = exp1a;
    REQUIRE(exp2b.isHeadNumber());
    REQUIRE(exp2b.head().asNumber() == 1.0);

    INFO("number to complex");
    Expression exp3b(Atom(std::complex<double>(1.0, 2.0)));
    exp3b = exp1a;
    REQUIRE(exp3b.isHeadNumber());
    REQUIRE(exp3b.head().asNumber() == 1.0);

    INFO("number to list");
    Expression::List list2 = { exp1b, exp2b, exp3b };
    Expression exp4b(list2);
    exp4b = exp1a;
    REQUIRE(exp4b.isHeadNumber());
    REQUIRE(exp4b.head().asNumber() == 1.0);

    INFO("number to lambda");
    Expression exp5b(list2, exp3b);
    exp5b = exp1a;
    REQUIRE(exp5b.isHeadNumber());
    REQUIRE(exp5b.head().asNumber() == 1.0);
  }
  SECTION("Assign Symbol type")
  {
    INFO("symbol to number");
    Expression exp1b(Atom(2.0));
    exp1b = exp2a;
    REQUIRE(exp1b.isHeadSymbol());
    REQUIRE(exp1b.head().asSymbol() == "foo");

    INFO("symbol to symbol");
    Expression exp2b(Atom("oof"));
    exp2b = exp2a;
    REQUIRE(exp2b.isHeadSymbol());
    REQUIRE(exp2b.head().asSymbol() == "foo");

    INFO("symbol to complex");
    Expression exp3b(Atom(std::complex<double>(1.0, 2.0)));
    exp3b = exp2a;
    REQUIRE(exp3b.isHeadSymbol());
    REQUIRE(exp3b.head().asSymbol() == "foo");

    INFO("symbol to list");
    Expression::List list2 = { exp1b, exp2b, exp3b };
    Expression exp4b(list2);
    exp4b = exp2a;
    REQUIRE(exp4b.isHeadSymbol());
    REQUIRE(exp4b.head().asSymbol() == "foo");

    INFO("symbol to lambda");
    Expression exp5b(list2, exp3b);
    exp5b = exp2a;
    REQUIRE(exp5b.isHeadSymbol());
    REQUIRE(exp5b.head().asSymbol() == "foo");
  }
  SECTION("Assign Complex type")
  {
    INFO("complex to number");
    Expression exp1b(Atom(2.0));
    exp1b = exp3a;
    REQUIRE(exp1b.isHeadComplex());
    REQUIRE(exp1b.head().asComplex() == std::complex<double>(0.0, 1.0));

    INFO("complex to symbol");
    Expression exp2b(Atom("oof"));
    exp2b = exp3a;
    REQUIRE(exp2b.isHeadComplex());
    REQUIRE(exp2b.head().asComplex() == std::complex<double>(0.0, 1.0));

    INFO("complex to complex");
    Expression exp3b(Atom(std::complex<double>(1.0, 2.0)));
    exp3b = exp3a;
    REQUIRE(exp3b.isHeadComplex());
    REQUIRE(exp3b.head().asComplex() == std::complex<double>(0.0, 1.0));

    INFO("complex to list");
    Expression::List list2 = { exp1b, exp2b, exp3b };
    Expression exp4b(list2);
    exp4b = exp3a;
    REQUIRE(exp4b.isHeadComplex());
    REQUIRE(exp4b.head().asComplex() == std::complex<double>(0.0, 1.0));

    INFO("complex to lambda");
    Expression exp5b(list2, exp3b);
    exp5b = exp3a;
    REQUIRE(exp5b.isHeadComplex());
    REQUIRE(exp5b.head().asComplex() == std::complex<double>(0.0, 1.0));
  }
  SECTION("Assign List type")
  {
    INFO("list to number");
    Expression exp1b(Atom(2.0));
    exp1b = exp4a;
    REQUIRE(exp1b.isHeadList());
    REQUIRE(exp1b.asList() == list1);

    INFO("list to symbol");
    Expression exp2b(Atom("oof"));
    exp2b = exp4a;
    REQUIRE(exp2b.isHeadList());
    REQUIRE(exp2b.asList() == list1);

    INFO("list to complex");
    Expression exp3b(Atom(std::complex<double>(1.0, 2.0)));
    exp3b = exp4a;
    REQUIRE(exp3b.isHeadList());
    REQUIRE(exp3b.asList() == list1);

    INFO("list to list");
    Expression::List list2 = { exp1b, exp2b, exp3b };
    Expression exp4b(list2);
    exp4b = exp4a;
    REQUIRE(exp4b.isHeadList());
    REQUIRE(exp4b.asList() == list1);

    INFO("list to lambda");
    Expression exp5b(list2, exp3b);
    exp5b = exp4a;
    REQUIRE(exp5b.isHeadList());
    REQUIRE(exp5b.asList() == list1);
  }
  SECTION("Assign Lambda type")
  {
    INFO("lambda to number");
    Expression exp1b(Atom(2.0));
    exp1b = exp5a;
    REQUIRE(exp1b.isHeadLambda());
    REQUIRE(exp1b.asLambda() == Expression::Lambda(list1, exp3a));

    INFO("lambda to symbol");
    Expression exp2b(Atom("oof"));
    exp2b = exp5a;
    REQUIRE(exp2b.isHeadLambda());
    REQUIRE(exp2b.asLambda() == Expression::Lambda(list1, exp3a));

    INFO("lambda to complex");
    Expression exp3b(Atom(std::complex<double>(1.0, 2.0)));
    exp3b = exp5a;
    REQUIRE(exp3b.isHeadLambda());
    REQUIRE(exp3b.asLambda() == Expression::Lambda(list1, exp3a));

    INFO("lambda to list");
    Expression::List list2 = { exp1b, exp2b, exp3b };
    Expression exp4b(list2);
    exp4b = exp5a;
    REQUIRE(exp4b.isHeadLambda());
    REQUIRE(exp4b.asLambda() == Expression::Lambda(list1, exp3a));

    INFO("lambda to lambda");
    Expression exp5b(list2, exp3b);
    exp5b = exp5a;
    REQUIRE(exp5b.isHeadLambda());
    REQUIRE(exp5b.asLambda() == Expression::Lambda(list1, exp3a));
  }
}

// All other tests of eval, apply, and private helper methods
// will be done as integration tests in interpreter_tests because
// the Expression methods require an associated Environment


