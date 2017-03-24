#ifndef _INTERPRETER_HPP_
#define _INTERPRETER_HPP_

#include "tokenize.hpp"
#include "expression.hpp"
#include "environment.hpp"
#include <sstream>

class Interpreter {
public:

	// Default construct an Interpreter with the default environment and an empty AST
	Interpreter();

	// Given a vtscript program as a std::istream, attempt to parse into an internal AST
	// return true on success, false on failure
	bool parse(std::istream & expression) noexcept;

	// Evaluate the current AST and return the resulting Expression
	// throws InterpreterSemanticError if a semantic error is encountered
	// the exception message string should document the nature of the semantic error 
	Expression eval();	

	string expressionToString(Expression result);

private:
	Tokenize tree;
	Expression expression;
	Environment theEnvironment;

	string streamToString(std::istream &expression);

	string output;

};

#endif