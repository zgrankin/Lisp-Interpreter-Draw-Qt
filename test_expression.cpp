#include "catch.hpp"

#include <tuple>
#include <string>
#include <sstream>
#include <fstream>
#include <iostream>

#include "interpreter_semantic_error.hpp"
#include "interpreter.hpp"
#include "expression.hpp"
#include "test_config.hpp"

using std::tuple;
using std::make_tuple;

Expression runScript(const std::string & program) {

	std::istringstream iss(program);

	Interpreter interp;

	bool ok = interp.parse(iss);
	if (!ok) {
		std::cerr << "Failed to parse: " << program << std::endl;
	}
	REQUIRE(ok);

	Expression result;
	result = interp.eval();

	return result;
}

TEST_CASE("Test Expression M-ary Operators Invalid Input", "[interpreter]") {

	std::string program = "(+ True False)";
	std::istringstream iss(program);
	REQUIRE_THROWS_AS(runScript(program), InterpreterSemanticError);
	program = "(+)";
	REQUIRE_THROWS_AS(runScript(program), InterpreterSemanticError);

	program = "(* True False)";
	REQUIRE_THROWS_AS(runScript(program), InterpreterSemanticError);
	program = "(*)";
	REQUIRE_THROWS_AS(runScript(program), InterpreterSemanticError);

	program = "(and 1 2)";
	REQUIRE_THROWS_AS(runScript(program), InterpreterSemanticError);
	program = "(and)";
	REQUIRE_THROWS_AS(runScript(program), InterpreterSemanticError);

	program = "(or 1 2)";
	REQUIRE_THROWS_AS(runScript(program), InterpreterSemanticError);
	program = "(or)";
	REQUIRE_THROWS_AS(runScript(program), InterpreterSemanticError);

	Interpreter interp;

	bool ok = interp.parse(iss);

	REQUIRE(ok);
}

TEST_CASE("Test Expression Binary Operators Invalid Input", "[interpreter]") {

	std::string program = "(- True False)";
	std::istringstream iss(program);
	REQUIRE_THROWS_AS(runScript(program), InterpreterSemanticError);
	program = "(- 1 2 3)";
	REQUIRE_THROWS_AS(runScript(program), InterpreterSemanticError);
	program = "(-)";
	REQUIRE_THROWS_AS(runScript(program), InterpreterSemanticError);

	program = "(/ True False)";
	REQUIRE_THROWS_AS(runScript(program), InterpreterSemanticError);
	program = "(/ 1 2 3)";
	REQUIRE_THROWS_AS(runScript(program), InterpreterSemanticError);
	program = "(/ 1)";
	REQUIRE_THROWS_AS(runScript(program), InterpreterSemanticError);
	program = "(/)";
	REQUIRE_THROWS_AS(runScript(program), InterpreterSemanticError);

	program = "(< 1 2 3)";
	REQUIRE_THROWS_AS(runScript(program), InterpreterSemanticError);
	program = "(< True False)";
	REQUIRE_THROWS_AS(runScript(program), InterpreterSemanticError);
	program = "(< 1)";
	REQUIRE_THROWS_AS(runScript(program), InterpreterSemanticError);
	program = "(<)";
	REQUIRE_THROWS_AS(runScript(program), InterpreterSemanticError);

	program = "(<= 1 2 3)";
	REQUIRE_THROWS_AS(runScript(program), InterpreterSemanticError);
	program = "(<= True False)";
	REQUIRE_THROWS_AS(runScript(program), InterpreterSemanticError);
	program = "(<= 1)";
	REQUIRE_THROWS_AS(runScript(program), InterpreterSemanticError);
	program = "(<=)";
	REQUIRE_THROWS_AS(runScript(program), InterpreterSemanticError);

	program = "(> 1 2 3)";
	REQUIRE_THROWS_AS(runScript(program), InterpreterSemanticError);
	program = "(> True False)";
	REQUIRE_THROWS_AS(runScript(program), InterpreterSemanticError);
	program = "(> 1)";
	REQUIRE_THROWS_AS(runScript(program), InterpreterSemanticError);
	program = "(>)";
	REQUIRE_THROWS_AS(runScript(program), InterpreterSemanticError);

	program = "(>= 1 2 3)";
	REQUIRE_THROWS_AS(runScript(program), InterpreterSemanticError);
	program = "(>= True False)";
	REQUIRE_THROWS_AS(runScript(program), InterpreterSemanticError);
	program = "(>= 1)";
	REQUIRE_THROWS_AS(runScript(program), InterpreterSemanticError);
	program = "(>=)";
	REQUIRE_THROWS_AS(runScript(program), InterpreterSemanticError);

	program = "(= 1 2 3)";
	REQUIRE_THROWS_AS(runScript(program), InterpreterSemanticError);
	program = "(= True False)";
	REQUIRE_THROWS_AS(runScript(program), InterpreterSemanticError);
	program = "(= 1)";
	REQUIRE_THROWS_AS(runScript(program), InterpreterSemanticError);
	program = "(=)";
	REQUIRE_THROWS_AS(runScript(program), InterpreterSemanticError);

	Interpreter interp;

	bool ok = interp.parse(iss);

	REQUIRE(ok);
}

TEST_CASE("Test Expression Special Forms Invalid Input", "[interpreter]") {

	std::string program = "(begin a b)";
	std::istringstream iss(program);
	REQUIRE_THROWS_AS(runScript(program), InterpreterSemanticError);
	program = "(begin)";
	REQUIRE_THROWS_AS(runScript(program), InterpreterSemanticError);

	program = "(if (+ 1 2) 4 5)";
	REQUIRE_THROWS_AS(runScript(program), InterpreterSemanticError);
	program = "(if (< 1 2) 4 5 6)";
	REQUIRE_THROWS_AS(runScript(program), InterpreterSemanticError);
	program = "(if)";
	REQUIRE_THROWS_AS(runScript(program), InterpreterSemanticError);

	program = "(define 1 a)";
	REQUIRE_THROWS_AS(runScript(program), InterpreterSemanticError);
	program = "(define a 1 (+ 1 2))";
	REQUIRE_THROWS_AS(runScript(program), InterpreterSemanticError);
	program = "(define)";
	REQUIRE_THROWS_AS(runScript(program), InterpreterSemanticError);

	Interpreter interp;

	bool ok = interp.parse(iss);

	REQUIRE(ok);
}

TEST_CASE("Test Special Case Number Input", "[interpreter]") {

	std::string program = "(.5)";
	REQUIRE(runScript(program).atom.atomType == NumberType);
	program = "(0.5)";
	REQUIRE(runScript(program).atom.atomType == NumberType);
	program = "(0.5e5)";
	REQUIRE(runScript(program).atom.atomType == NumberType);
	program = "(-4e-5)";
	REQUIRE(runScript(program).atom.atomType == NumberType);
	program = "(-4-e5)";

	std::istringstream iss(program);
	Interpreter interp;
	bool ok = interp.parse(iss);

	REQUIRE(!ok);
}

TEST_CASE("Test Symbol Constructor", "[interpreter]") {
	string greeting = "supdawg";
	string anotherGreeting = "supdawg";
	Atom hi;
	hi.var = "supdawg";
	REQUIRE(hi.var == Expression(greeting).atom.var);
	REQUIRE(Expression(anotherGreeting) == Expression(greeting).atom.var);
}

TEST_CASE("Test Things Not being Covered", "[interpreter]") {

	std::string program = "(not 1)";
	REQUIRE_THROWS_AS(runScript(program), InterpreterSemanticError);

    program = "(point 1 2 3)";
	REQUIRE_THROWS_AS(runScript(program), InterpreterSemanticError);

	program = "(line 1 2 3)";
	REQUIRE_THROWS_AS(runScript(program), InterpreterSemanticError);

	program = "(arc 1 2)";
	REQUIRE_THROWS_AS(runScript(program), InterpreterSemanticError);

	program = "(draw)";
	REQUIRE_THROWS_AS(runScript(program), InterpreterSemanticError);

	program = "(draw a)";
	REQUIRE_THROWS_AS(runScript(program), InterpreterSemanticError);

	program = "(draw True)";
	REQUIRE_THROWS_AS(runScript(program), InterpreterSemanticError);

	program = "(draw (point 1 2))";
	REQUIRE(runScript(program).atom.point == make_tuple(1,2));

	program = "(draw (point a 2))";
	REQUIRE_THROWS_AS(runScript(program), InterpreterSemanticError);

	program = "(begin (define a (point 1 2)) (draw a))";
	REQUIRE(runScript(program).atom.point == make_tuple(1, 2));

	program = "(begin (define a (line (point 1 2) (point 3 4))) (draw a))";
	REQUIRE(runScript(program).atom.point == make_tuple(1, 2));
	REQUIRE(runScript(program).atom.point2 == make_tuple(3, 4));

	program = "(begin (define a (arc (point 1 2) (point 3 4) 3.14)) (draw a))";
	REQUIRE(runScript(program).atom.point == make_tuple(1, 2));
	REQUIRE(runScript(program).atom.point2 == make_tuple(3, 4));
	REQUIRE(runScript(program).atom.number == 3.14);

	REQUIRE(!(Expression() == Expression()));
}

TEST_CASE("Test Function expressionToString", "[interpreter]") {
	Interpreter test;
	Expression testAtom;
	testAtom.atom.atomType = PointType;
	testAtom.atom.point = make_tuple(1, 2);
	testAtom.atom.point2 = make_tuple(3, 4);
	testAtom.atom.number = 3.14;
	testAtom.atom.truthValue = false;
	REQUIRE(test.expressionToString(testAtom) == "(1,2)");
	testAtom.atom.atomType = LineType;
	REQUIRE(test.expressionToString(testAtom) == "((1,2),(3,4))");
	testAtom.atom.atomType = ArcType;
	REQUIRE(test.expressionToString(testAtom) == "((1,2),(3,4) 3.14)");
	testAtom.atom.atomType = BoolType;
	REQUIRE(test.expressionToString(testAtom) == "(False)");
}