#include "environment.hpp"
#include <cmath>

Environment::Environment() {
	Atom atom;
	setVariable("begin", atom);
	setVariable("define", atom);
	setVariable("if", atom);
	setVariable("not", atom);
	setVariable("and", atom);
	setVariable("or", atom);
	setVariable("<", atom);
	setVariable("<=", atom);
	setVariable(">", atom);
	setVariable(">=", atom);
	setVariable("=", atom);
	setVariable("+", atom);
	setVariable("-", atom);
	setVariable("*", atom);
	setVariable("/", atom);

	Atom pi;
	pi.atomType = NumberType;
	pi.number = atan2(0, -1);
	setVariable("pi", pi);
	
}

bool Environment::setVariable(std::string aString, Atom envAtom)
{
	if (varMap.count(aString) > 0)
		throw InterpreterSemanticError("Error: Variable already defined.");
	else {
		varMap.insert(std::pair<std::string, Atom>(aString, envAtom));
			return true;
	}
}

Atom Environment::findVar(std::string aString)
{
	return varMap[aString];
}
