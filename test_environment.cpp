#include "catch.hpp"

#include <string>
#include <sstream>
#include <fstream>
#include <iostream>

#include "interpreter_semantic_error.hpp"
#include "interpreter.hpp"
#include "expression.hpp"
#include "test_config.hpp"

TEST_CASE("Test environment", "[interpreter]") {

	Environment testEnv;
	Atom testAtom;
	
	REQUIRE(testEnv.setVariable("hello", testAtom));

}

TEST_CASE("Test environment already defined", "[interpreter]") {

	Environment testEnv;
	Atom testAtom;

	REQUIRE_THROWS_AS(!testEnv.setVariable("begin", testAtom), InterpreterSemanticError);
}