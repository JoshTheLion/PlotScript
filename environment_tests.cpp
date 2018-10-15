#include "catch.hpp"

#include "environment.hpp"
#include "semantic_error.hpp"

#include <cmath>

TEST_CASE( "Test default constructor", "[environment]" )
{
  Environment env;

  REQUIRE(env.is_known(Atom("pi")));
  REQUIRE(env.is_exp(Atom("pi")));
  REQUIRE(env.is_known(Atom("e")));
  REQUIRE(env.is_exp(Atom("e")));
  REQUIRE(env.is_known(Atom("I")));
  REQUIRE(env.is_exp(Atom("I")));

  REQUIRE(!env.is_known(Atom("hi")));
  REQUIRE(!env.is_exp(Atom("hi")));

  REQUIRE(env.is_proc(Atom("+")));
  REQUIRE(env.is_proc(Atom("-")));
  REQUIRE(env.is_proc(Atom("*")));
  REQUIRE(env.is_proc(Atom("/")));
  REQUIRE(env.is_proc(Atom("sqrt")));
  REQUIRE(env.is_proc(Atom("^")));
  REQUIRE(env.is_proc(Atom("ln")));

  REQUIRE(env.is_proc(Atom("sin")));
  REQUIRE(env.is_proc(Atom("cos")));
  REQUIRE(env.is_proc(Atom("tan")));

  REQUIRE(env.is_proc(Atom("real")));
  REQUIRE(env.is_proc(Atom("imag")));
  REQUIRE(env.is_proc(Atom("mag")));
  REQUIRE(env.is_proc(Atom("arg")));
  REQUIRE(env.is_proc(Atom("conj")));

  REQUIRE(env.is_proc(Atom("list")));
  REQUIRE(env.is_proc(Atom("first")));
  REQUIRE(env.is_proc(Atom("rest")));
  REQUIRE(env.is_proc(Atom("length")));
  REQUIRE(env.is_proc(Atom("append")));
  REQUIRE(env.is_proc(Atom("join")));
  REQUIRE(env.is_proc(Atom("range")));
  REQUIRE(!env.is_proc(Atom("op")));
}

TEST_CASE( "Test Lambda copy constructor", "[environment]" )
{
  Environment env;
  Environment clone1(env);
  
  INFO("Copied environment must contain every single (key,value) of original");
  REQUIRE(clone1 == env);

  INFO("Adding to original default environment");
  Expression a(Atom(1.0));
  env.add_exp(Atom("one"), a);
  
  REQUIRE(env.is_known(Atom("one")));
  REQUIRE(env.is_exp(Atom("one")));
  REQUIRE(env.get_exp(Atom("one")) == a);
  REQUIRE(clone1 != env);

  INFO("An environment copied from a non-default environment must reflect changes");
  Environment clone2(env);

  REQUIRE(clone2 == env);
  REQUIRE(clone2 != clone1);
}

TEST_CASE( "Test get expression", "[environment]" ) {
  Environment env;

  REQUIRE(env.get_exp(Atom("pi")) == Expression(std::atan2(0, -1)));
  REQUIRE(env.get_exp(Atom("hi")) == Expression());
}

TEST_CASE( "Test add expression", "[environment]" )
{
  Environment env;

  Expression a(Atom(1.0));
  env.add_exp(Atom("one"), a);
  REQUIRE(env.is_known(Atom("one")));
  REQUIRE(env.is_exp(Atom("one")));
  REQUIRE(env.get_exp(Atom("one")) == a);

  Expression b(Atom("hello"));
  env.add_exp(Atom("hi"), b);
  REQUIRE(env.is_known(Atom("hi")));
  REQUIRE(env.is_exp(Atom("hi")));
  REQUIRE(env.get_exp(Atom("hi")) == b);

  REQUIRE_THROWS_AS(env.add_exp(Atom(1.0), b), SemanticError);
}

TEST_CASE( "Test get built-in procedure", "[environment]" )
{
  Environment env;

  INFO("default procedure")
  Procedure p1 = env.get_proc(Atom("doesnotexist"));
  Procedure p2 = env.get_proc(Atom("alsodoesnotexist"));
  REQUIRE(p1 == p2);
  std::vector<Expression> args;
  REQUIRE(p1(args) == Expression());
  REQUIRE(p2(args) == Expression());

  INFO("trying add procedure")
  Procedure padd = env.get_proc(Atom("+"));
  args.emplace_back(1.0);
  args.emplace_back(2.0);
  REQUIRE(padd(args) == Expression(3.0));
}

TEST_CASE( "Test reset", "[environment]" )
{
  Environment env;

  Expression a(Atom(1.0));
  env.add_exp(Atom("one"), a);
  Expression b(Atom("hello"));
  env.add_exp(Atom("hi"), b);

  env.reset();
  REQUIRE(!env.is_known(Atom("one")));
  REQUIRE(!env.is_exp(Atom("one")));
  REQUIRE(env.get_exp(Atom("one")) == Expression());
  REQUIRE(!env.is_known(Atom("hi")));
  REQUIRE(!env.is_exp(Atom("hi")));
  REQUIRE(env.get_exp(Atom("hi")) == Expression());
}

TEST_CASE( "Test semantic errors", "[environment]" )
{
  Environment env;

  {
    Expression exp(Atom("begin"));
    
    REQUIRE_THROWS_AS(exp.eval(env), SemanticError);
  }
}

