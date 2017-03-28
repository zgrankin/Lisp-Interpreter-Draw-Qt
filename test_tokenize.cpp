#include "catch.hpp"

#include <string>
#include <sstream>
#include <fstream>
#include <iostream>

#include "interpreter_semantic_error.hpp"
#include "interpreter.hpp"
#include "expression.hpp"
#include "test_config.hpp"

TEST_CASE("Test Tokenize for correct creation of tokens and building of tree", "[interpreter]") {

	std::string program;
	Tokenize a;
	std::vector<std::string> tokenOut;

	program = ")";
	tokenOut = a.tokenize(program);
	REQUIRE(tokenOut.size() == 1);
	REQUIRE(!a.buildAST(tokenOut));

	program = "(hello 123 this is a test file)";
	tokenOut = a.tokenize(program);
	REQUIRE(tokenOut.size() == 9);
	REQUIRE(a.buildAST(tokenOut));	

	program = "(.a54 1 2)";
	tokenOut = a.tokenize(program);
	REQUIRE(tokenOut.size() == 5);
	REQUIRE(a.buildAST(tokenOut));

	program = "(+ -4.5ee4 2 3)";
	tokenOut = a.tokenize(program);
	REQUIRE(tokenOut.size() == 6);
	REQUIRE(!a.buildAST(tokenOut));

	program = "(* -5.5e10 2 3 4 -5.5e-10)";
	tokenOut = a.tokenize(program);
	REQUIRE(tokenOut.size() == 8);
	REQUIRE(a.buildAST(tokenOut));

	program = "(-5.4-e4)";
	tokenOut = a.tokenize(program);
	REQUIRE(tokenOut.size() == 3);
	REQUIRE(!a.buildAST(tokenOut));

	program = "(.5)";
	tokenOut = a.tokenize(program);
	REQUIRE(tokenOut.size() == 3);
	REQUIRE(a.buildAST(tokenOut));

	program = "(or True False False)";
	tokenOut = a.tokenize(program);
	REQUIRE(tokenOut.size() == 6);
	REQUIRE(a.buildAST(tokenOut));

}

TEST_CASE("Test Special Parse Error Case", "[interpreter]") {

	std::string program = "(+ 1 2) 3";
	Tokenize a;
	std::vector<std::string> tokenOut = a.tokenize(program);

	REQUIRE(a.buildAST(tokenOut));
}