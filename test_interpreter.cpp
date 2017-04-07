#include "catch.hpp"

#include <string>
#include <sstream>
#include <fstream>
#include <iostream>

#include "interpreter_semantic_error.hpp"
#include "interpreter.hpp"
#include "expression.hpp"
#include "test_config.hpp"

Expression run(const std::string & program){
  
  std::istringstream iss(program);
    
  Interpreter interp;
    
  bool ok = interp.parse(iss);
  if(!ok){
    std::cerr << "Failed to parse: " << program << std::endl; 
  }
  REQUIRE(ok);

  Expression result;
  REQUIRE_NOTHROW(result = interp.eval());

  return result;
}

Expression runfile(const std::string & fname){

  std::ifstream ifs(fname);
  REQUIRE(ifs.good() == true);
  
  Interpreter interp;
    
  bool ok = interp.parse(ifs);
  if(!ok){
    std::cerr << "Failed to parse file: " << fname << std::endl; 
  }
  REQUIRE(ok);

  Expression result;
  REQUIRE_NOTHROW(result = interp.eval());

  return result;
}

TEST_CASE( "Test Interpreter parser with expected input", "[interpreter]" ) {

  std::string program = "(begin (define r 10) (* pi (* r r)))";

  std::istringstream iss(program);
 
  Interpreter interp;

  bool ok = interp.parse(iss);

  REQUIRE(ok);
}

TEST_CASE( "Test Interpreter parser with numerical literals", "[interpreter]" ) {

  std::vector<std::string> programs = {"(1)", "(+1)", "(+1e+0)", "(1e-0)"};
  
  for(auto program : programs){
    std::istringstream iss(program);
 
    Interpreter interp;

    bool ok = interp.parse(iss);

    REQUIRE(ok);
  }
}

TEST_CASE( "Test Interpreter parser with truncated input", "[interpreter]" ) {

  {
    std::string program = "(f";
    std::istringstream iss(program);
  
    Interpreter interp;
    bool ok = interp.parse(iss);
    REQUIRE(!ok);
  }
  
  {
    std::string program = "(begin (define r 10) (* pi (* r r";
    std::istringstream iss(program);

    Interpreter interp;
    bool ok = interp.parse(iss);
    REQUIRE(!ok);
  }
}

TEST_CASE( "Test Interpreter parser with extra input", "[interpreter]" ) {

  std::string program = "(begin (define r 10) (* pi (* r r))) )";
  std::istringstream iss(program);

  Interpreter interp;

  bool ok = interp.parse(iss);

  REQUIRE(!ok);
}

TEST_CASE( "Test Interpreter parser with single non-keyword", "[interpreter]" ) {

  std::string program = "hello";
  std::istringstream iss(program);
  
  Interpreter interp;

  bool ok = interp.parse(iss);

  REQUIRE(!ok);
}

TEST_CASE( "Test Interpreter parser with empty input", "[interpreter]" ) {

  std::string program;
  std::istringstream iss(program);
  
  Interpreter interp;

  bool ok = interp.parse(iss);

  REQUIRE(!ok);
}

TEST_CASE( "Test Interpreter parser with empty expression", "[interpreter]" ) {

  std::string program = "( )";
  std::istringstream iss(program);
  
  Interpreter interp;

  bool ok = interp.parse(iss);

  REQUIRE(!ok);
}

TEST_CASE( "Test Interpreter parser with bad number string", "[interpreter]" ) {

  std::string program = "(1abc)";
  std::istringstream iss(program);
  
  Interpreter interp;

  bool ok = interp.parse(iss);

  REQUIRE(!ok);
}

TEST_CASE( "Test Interpreter parser with incorrect input. Regression Test", "[interpreter]" ) {

  std::string program = "(+ 1 2) (+ 3 4)";
  std::istringstream iss(program);
  
  Interpreter interp;

  bool ok = interp.parse(iss);

  REQUIRE(!ok);
}

TEST_CASE( "Test Interpreter result with literal expressions", "[interpreter]" ) {

  { // Boolean True
    std::string program = "(True)";
    Expression result = run(program);
    REQUIRE(result == Expression(true));
  }

  { // Boolean False
    std::string program = "(False)";
    Expression result = run(program);
    REQUIRE(result == Expression(false));
  }
  
  { // Number
    std::string program = "(4)";
    Expression result = run(program);
    REQUIRE(result == Expression(4.));
  }

  { // Symbol
    std::string program = "(pi)";
    Expression result = run(program);
    REQUIRE(result == Expression(atan2(0, -1)));
  }

}

TEST_CASE( "Test Interpreter result with simple procedures (add)", "[interpreter]" ) {

  { // add, binary case
    std::string program = "(+ 1 2)";
    Expression result = run(program);
    REQUIRE(result == Expression(3.));
  }
  
  { // add, 3-ary case
    std::string program = "(+ 1 2 3)";
    Expression result = run(program);
    REQUIRE(result == Expression(6.));
  }

  { // add, 6-ary case
    std::string program = "(+ 1 2 3 4 5 6)";
    Expression result = run(program);
    REQUIRE(result == Expression(21.));
  }
}
  
TEST_CASE( "Test Interpreter special form: if", "[interpreter]" ) {

  {
    std::string program = "(if True (4) (-4))";
    Expression result = run(program);
    REQUIRE(result == Expression(4.));
  }
  
  {
    std::string program = "(if False (4) (-4))";
    Expression result = run(program);
    REQUIRE(result == Expression(-4.));
  }
}

TEST_CASE( "Test Interpreter special forms: begin and define", "[interpreter]" ) {

  {
    std::string program = "(define answer 42)";
    Expression result = run(program);
    REQUIRE(result == Expression(42.));
  }

  {
    std::string program = "(begin (define answer 42)\n(answer))";
    Expression result = run(program);
    REQUIRE(result == Expression(42.));
  }
  
  {
    std::string program = "(begin (define answer (+ 9 11)) (answer))";
    Expression result = run(program);
    REQUIRE(result == Expression(20.));
  }

  {
    std::string program = "(begin (define a 1) (define b 1) (+ a b))";
    Expression result = run(program);
    REQUIRE(result == Expression(2.));
  }
}

TEST_CASE( "Test a complex expression", "[interpreter]" ) {

  {
    std::string program = "(+ (+ 10 1) (+ 30 (+ 1 1)))";
    Expression result = run(program);
    REQUIRE(result == Expression(43.));
  }
}

TEST_CASE( "Test relational procedures", "[interpreter]" ) {

  {
    std::vector<std::string> programs = {"(< 1 2)",
					 "(<= 1 2)",
					 "(<= 1 1)",
					 "(> 2 1)",
					 "(>= 2 1)",
					 "(>= 2 2)",
					 "(= 4 4)"};
    for(auto s : programs){
      Expression result = run(s);
      REQUIRE(result == Expression(true));
    }
  }

  {
    std::vector<std::string> programs = {"(< 2 1)",
					 "(<= 2 1)",
					 "(<= 1 0)",
					 "(> 1 2)",
					 "(>= 1 2)",
					 "(>= 2 3)",
					 "(= 0 4)"};
    for(auto s : programs){
      Expression result = run(s);
      REQUIRE(result == Expression(false));
    }
  }
}

TEST_CASE( "Test arithmetic procedures", "[interpreter]" ) {

  {
    std::vector<std::string> programs = {"(+ 1 -2)",
					 "(+ -3 1 1)",
					 "(- 1)",
					 "(- 1 2)",
					 "(* 1 -1)",
					 "(* 1 1 -1)",
					 "(/ -1 1)",
					 "(/ 1 -1)"};

    for(auto s : programs){
      Expression result = run(s);
      REQUIRE(result == Expression(-1.));
    }
  }
}

TEST_CASE( "Test logical procedures", "[interpreter]" ) {

  REQUIRE(run("(not True)") == Expression(false));
  REQUIRE(run("(not False)") == Expression(true));

  REQUIRE(run("(and True True)") == Expression(true));
  REQUIRE(run("(and True False)") == Expression(false));
  REQUIRE(run("(and False True)") == Expression(false));
  REQUIRE(run("(and False False)") == Expression(false));
  REQUIRE(run("(and True True False)") == Expression(false));

  REQUIRE(run("(or True True)") == Expression(true));
  REQUIRE(run("(or True False)") == Expression(true));
  REQUIRE(run("(or False True)") == Expression(true));
  REQUIRE(run("(or False False)") == Expression(false));
  REQUIRE(run("(or True True False)") == Expression(true));
}

TEST_CASE( "Test trig procedures", "[interpreter]" ) {

  REQUIRE(run("(sin pi)") == Expression(0.));
  REQUIRE(run("(cos pi)") == Expression(-1.));
  REQUIRE(run("(arctan 1 0)") == Expression(atan2(1,0)));

  std::vector<std::string> programs = {"(sin 0 1)",
				       "(cos 0 1)", 
				       "(arctan 0)",
				       "(sin True)",
				       "(cos True)",
				       "(arctan 1 False)"};
  for(auto s : programs){
    Interpreter interp;
   
    std::istringstream iss(s);
    
    REQUIRE(interp.parse(iss));
    
    REQUIRE_THROWS_AS(interp.eval(), InterpreterSemanticError);
  }
}

TEST_CASE( "Test some semantically invalid expresions", "[interpreter]" ) {
  
  std::vector<std::string> programs = {"(@ none)", // so such procedure
				       "(- 1 1 2)", // too many arguments
				       "(define if 1)", // redefine special form
				       "(define pi 3.14)"}; // redefine builtin symbol
    for(auto s : programs){
      Interpreter interp;

      std::istringstream iss(s);
      
      bool ok = interp.parse(iss);
      REQUIRE(ok);

      REQUIRE_THROWS_AS(interp.eval(), InterpreterSemanticError);
    }
}

TEST_CASE("Test graphic types", "[interpreter]") {

  {
    std::string program = "(point 0 0)";
    std::istringstream iss(program);
    Interpreter interp;

    REQUIRE(interp.parse(iss));
    REQUIRE(interp.eval() == Expression(std::make_tuple(0., 0.)));
  }
  {
    std::string program = "(line (point 0 0) (point 10 0))";
    std::istringstream iss(program);
    Interpreter interp;

    REQUIRE(interp.parse(iss));
    REQUIRE(interp.eval() ==
            Expression(std::make_tuple(0., 0.), std::make_tuple(10., 0.)));
  }
  {
    std::string program = "(arc (point 0 0) (point 10 0) pi)";
    std::istringstream iss(program);
    Interpreter interp;

    REQUIRE(interp.parse(iss));
    REQUIRE(interp.eval() == Expression(std::make_tuple(0., 0.),
                                        std::make_tuple(10., 0.),
                                        atan2(0, -1)));
  }
}

TEST_CASE( "Test file tests/test0.vts", "[interpreter]" ) {

  std::string fname = TEST_FILE_DIR + "/test0.vts";

  std::ifstream ifs(fname);
  REQUIRE(ifs.good() == true);
  
  Interpreter interp;
    
  bool ok = interp.parse(ifs);
  REQUIRE(!ok);
}

TEST_CASE( "Test syntactically correct file using CRLF (Windows) line endings.", "[interpreter]" ) {

  std::string input = TEST_FILE_DIR + "/test_crlf.vts";

  std::string expect = input + ".expected";
  Expression result = runfile(input);
  Expression expected_result = runfile(expect);
  
  REQUIRE(result == expected_result);
  REQUIRE(result == Expression(-1.));
}

TEST_CASE( "Test syntactically INCORRECT files", "[interpreter]" ) {

  std::string fname = TEST_FILE_DIR + "/test1.vts";

  std::ifstream ifs(fname);
  REQUIRE(ifs.good() == true);
  
  Interpreter interp;
    
  bool ok = interp.parse(ifs);
  REQUIRE(!ok);
}

TEST_CASE( "Test all syntactically and semantically CORRECT files.", "[interpreter]" ) {

  const int START_TEST = 2;
  const int LAST_TEST = 5;
  std::string base = TEST_FILE_DIR + "/test";
  
  for(int i = START_TEST; i <= LAST_TEST; ++i){
    
    std::string input = base + std::to_string(i) + ".vts";
    std::string expect = input + ".expected";
    Expression result = runfile(input);
    Expression expected_result = runfile(expect);
  
    REQUIRE(result == expected_result);
  }
}
