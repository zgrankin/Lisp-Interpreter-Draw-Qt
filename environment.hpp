#ifndef _ENVIRONMENT_HPP_
#define _ENVIRONMENT_HPP_

#include <string>
#include <vector>
#include <tuple>
#include <map>
#include "expression.hpp"

using namespace std;


class Environment {
public:
	Environment();

	bool setVariable(std::string aString, Atom envAtom);
	Atom findVar(std::string aString);

	std::map<std::string, Atom> varMap;

	vector< Atom > graphics;

private:
	
};

#endif