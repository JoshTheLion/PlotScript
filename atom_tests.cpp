#include "catch.hpp"

#include "atom.hpp"

TEST_CASE( "Test Atom constructors", "[atom]" ) {

  SECTION("Default Constructor")
  {
    Atom a;

    REQUIRE(a.isNone());
    REQUIRE(!a.isNumber());
    REQUIRE(!a.isSymbol());
	REQUIRE(!a.isComplex());
  }
  
  SECTION("Number Constructor")
  {
    Atom a(1.0);

    REQUIRE(!a.isNone());
    REQUIRE(a.isNumber());
    REQUIRE(!a.isSymbol());
	REQUIRE(!a.isComplex());
  }
  
  SECTION("Symbol Constructor")
  {
    Atom a("hi");

    REQUIRE(!a.isNone());
    REQUIRE(!a.isNumber());
    REQUIRE(a.isSymbol());
	REQUIRE(!a.isComplex());
  }
  
  SECTION("Complex Constructor")
  {
    Atom a(std::complex<double>(0.0, 1.0));

    REQUIRE(!a.isNone());
    REQUIRE(!a.isNumber());
    REQUIRE(!a.isSymbol());
	REQUIRE(a.isComplex());
  }
  
  SECTION("Token Constructor")
  {
    Token t1("hi");
    Atom a1(t1);
    
    REQUIRE(!a1.isNone());
    REQUIRE(!a1.isNumber());
    REQUIRE(a1.isSymbol());
    REQUIRE(!a1.isComplex());
    
    Token t2("I");
    Atom a2(t2);
    
    REQUIRE(!a2.isNone());
    REQUIRE(!a2.isNumber());
    REQUIRE(!a2.isSymbol());
    REQUIRE(a2.isComplex());
  }
  
  SECTION("Copy Constructor")
  {
    Atom a1(1.0);
    Atom b1("hi");
    Atom c1(std::complex<double>(0.0, 1.0));
    
    Atom a2 = a1;
    REQUIRE(!a2.isNone());
    REQUIRE(a2.isNumber());
    REQUIRE(!a2.isSymbol());
    REQUIRE(!a2.isComplex());
    
    Atom b2 = b1;
    REQUIRE(!b2.isNone());
    REQUIRE(!b2.isNumber());
    REQUIRE(b2.isSymbol());
    REQUIRE(!b2.isComplex());
    
    Atom c2 = c1;
    REQUIRE(!c2.isNone());
    REQUIRE(!c2.isNumber());
    REQUIRE(!c2.isSymbol());
    REQUIRE(c2.isComplex());
  }
}

TEST_CASE( "Test Atom assignment", "[atom]" ) {

  SECTION("Assign default") {
    {
      INFO("default to default");
      Atom a;
      Atom b;
      b = a;
      REQUIRE(b.isNone());
      REQUIRE(!b.isNumber());
      REQUIRE(!b.isSymbol());
      REQUIRE(!b.isComplex());
    }

    {
      INFO("default to number");
      Atom a;
      Atom b(1.0);
      b = a;
      REQUIRE(b.isNone());
      REQUIRE(!b.isNumber());
      REQUIRE(!b.isSymbol());
      REQUIRE(!b.isComplex());
    }

    {
      INFO("default to symbol");
      Atom a;
      Atom b("hi");
      b = a;
      REQUIRE(b.isNone());
      REQUIRE(!b.isNumber());
      REQUIRE(!b.isSymbol());
      REQUIRE(!b.isComplex());
    }

    {
      INFO("default to complex");
      Atom a;
      Atom b(std::complex<double>(0.0, 1.0));
      b = a;
      REQUIRE(b.isNone());
      REQUIRE(!b.isNumber());
      REQUIRE(!b.isSymbol());
      REQUIRE(!b.isComplex());
    }
  }

  SECTION("Assign number") {
    {
      INFO("number to default");
      Atom a(1.0);
      Atom b;
      b = a;
      REQUIRE(b.isNumber());
      REQUIRE(b.asNumber() == 1.0);
    }

    {
      INFO("number to number");
      Atom a(1.0);
      Atom b(2.0);
      b = a;
      REQUIRE(b.isNumber());
      REQUIRE(b.asNumber() == 1.0);
    }

    {
      INFO("number to symbol");
      Atom a(1.0);
      Atom b("hi");
      b = a;
      REQUIRE(b.isNumber());
      REQUIRE(b.asNumber() == 1.0);
    }

    {
      INFO("number to complex");
      Atom a(1.0);
      Atom b(std::complex<double>(0.0, 1.0));
      b = a;
      REQUIRE(b.isNumber());
      REQUIRE(b.asNumber() == 1.0);
    }
  }

  SECTION("Assign symbol") {
    {
      INFO("symbol to default");
      Atom a("hi");
      Atom b;
      b = a;
      REQUIRE(b.isSymbol());
      REQUIRE(b.asSymbol() == "hi");
    }

    {
      INFO("symbol to number");
      Atom a("hi");
      Atom b(1.0);
      b = a;
      REQUIRE(b.isSymbol());
      REQUIRE(b.asSymbol() == "hi");
    }

    {
      INFO("symbol to symbol");
      Atom a("hi");
      Atom b("bye");
      b = a;
      REQUIRE(b.isSymbol());
      REQUIRE(b.asSymbol() == "hi");
    }

    {
      INFO("symbol to complex");
      Atom a("hi");
      Atom b(std::complex<double>(0.0, 1.0));
      b = a;
      REQUIRE(b.isSymbol());
      REQUIRE(b.asSymbol() == "hi");
    }
  }
  
  SECTION("Assign complex") {
    
    std::complex<double> cTest(0.0, 3.0);
    {
      INFO("complex to default");
      Atom a(cTest);
      Atom b;
      b = a;
      REQUIRE(b.isComplex());
      REQUIRE(b.asComplex() == cTest);
    }

    {
      INFO("complex to number");
      Atom a(cTest);
      Atom b(1.0);
      b = a;
      REQUIRE(b.isComplex());
      REQUIRE(b.asComplex() == cTest);
    }

    {
      INFO("complex to symbol");
      Atom a(cTest);
      Atom b("hi");
      b = a;
      REQUIRE(b.isComplex());
      REQUIRE(b.asComplex() == cTest);
    }

    {
      INFO("complex to complex");
      Atom a(cTest);
      Atom b(std::complex<double>(0.0, 2.0));
      b = a;
      REQUIRE(b.isComplex());
      REQUIRE(b.asComplex() == cTest);
    }
  }
}

TEST_CASE( "Test Atom comparison", "[atom]" ) {

  SECTION("Compare default") {
    {
      INFO("compare default to default");
      Atom a;
      Atom b;
      REQUIRE(a == b);
    }

    {
      INFO("compare default to number");
      Atom a;
      Atom b(1.0);
      REQUIRE(a != b);
    }

    {
      INFO("compare default to symbol");
      Atom a;
      Atom b("hi");
      REQUIRE(a != b);
    }

    {
      INFO("compare default to complex");
      Atom a;
      Atom b(std::complex<double>(0.0, 1.0));
      REQUIRE(a != b);
    }
  }

  SECTION("Compare number") {

    {
      INFO("compare number to default");
      Atom a(1.0);
      Atom b;
      REQUIRE(a != b);
    }

    {
      INFO("compare number to number");
      Atom a(1.0);
      Atom b(1.0);
      Atom c(2.0);
      REQUIRE(a == b);
      REQUIRE(a != c);
    }

    {
      INFO("compare number to symbol");
      Atom a(1.0);
      Atom b("hi");
      REQUIRE(a != b);
    }
	
    {
      INFO("compare number to complex");
      Atom a(1.0);
      Atom b(std::complex<double>(0.0, 1.0));
      REQUIRE(a != b);
    }
  }

  SECTION("Compare symbol") {
    {
      INFO("compare symbol to default");
      Atom a("hi");
      Atom b;
      REQUIRE(a != b);
    }

    {
      INFO("compare symbol to number");
      Atom a("hi");
      Atom b(1.0);
      REQUIRE(a != b);
    }

    {
      INFO("compare symbol to symbol");
      Atom a("hi");
      Atom b("hi");
      Atom c("bye");
      REQUIRE(a == b);
      REQUIRE(a != c);
    }

    {
      INFO("compare symbol to complex");
      Atom a("hi");
      Atom b(std::complex<double>(0.0, 1.0));
      REQUIRE(a != b);
    }
  }

  SECTION("Compare complex") {
    {
      INFO("compare complex to default");
      Atom a(std::complex<double>(0.0, 1.0));
      Atom b;
      REQUIRE(a != b);
    }

    {
      INFO("compare complex to number");
      Atom a(std::complex<double>(0.0, 1.0));
      Atom b(1.0);
      REQUIRE(a != b);
    }

    {
      INFO("compare complex to symbol");
      Atom a(std::complex<double>(0.0, 1.0));
      Atom b("hi");
      REQUIRE(a != b);
    }

    {
      INFO("compare complex to complex");
      Atom a(std::complex<double>(0.0, 1.0));
      Atom b(std::complex<double>(0.0, 1.0));
      Atom c(std::complex<double>(0.0, 2.0));
      REQUIRE(a == b);
      REQUIRE(a != c);
    }
  }
}





