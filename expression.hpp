#ifndef _EXPRESSION_HPP_
#define _EXPRESSION_HPP_

#include <string>
#include <vector>
#include <tuple>
#include <functional>
#include "interpreter_semantic_error.hpp"

using std::vector;
using std::string;
using std::tuple;
using std::make_tuple;

enum VariantType { NoneType, BoolType, NumberType, SymbolType, PointType, LineType, ArcType };

class Environment;

struct Atom {
	VariantType atomType = NoneType;
	double number;
	bool truthValue;
	string var;

	tuple<double, double> point;
	tuple<double, double> point2;
};

class Expression
{
public:
	Environment* environment;

	vector<Expression*> children;
	Expression* parent;
	Atom atom;

	// Default construct an Expression of type None
	Expression();

	~Expression();

	// Construct an Expression with a single Boolean atom with value
	Expression(bool value);

	// Construct an Expression with a single Number atom with value
	Expression(double value);

	// Construct an Expression with a single Symbol atom with value
	Expression(const std::string & value);

	// Construct an Expression with a single Point atom with value
	Expression(std::tuple<double, double> value);

	// Construct an Expression with a single Line atom with starting
	// point start and ending point end
	Expression(std::tuple<double, double> start, std::tuple<double, double> end);

	// Construct an Expression with a single Arc atom with center
	// point center, starting point start, and spanning angle angle in radians 
	Expression(std::tuple<double, double> center, std::tuple<double, double> start, double angle);

	// Equality operator for two Expressions, two expressions are equal if the have the same 
	// type, atom value, and number of arguments
	bool operator==(const Expression & exp) const noexcept;

	//bool almost_equal(double x, double y);

	void defineMethod();
	
	Expression evaluateTree();

	void outputFinalAnswer();

private:
	void euthanizeChildren();
	
};


#endif