#include "expression.hpp"
#include "environment.hpp"
#include <cmath>
#include <limits>
#include <iostream>


bool almost_equal(double x, double y);

Expression::Expression(): parent(nullptr)
{

	atom.atomType = NoneType;
}

Expression::~Expression()
{
	euthanizeChildren();
}


Expression::Expression(bool value)
{
	atom.atomType = BoolType;
	atom.truthValue = value;
	if (value == true)
		atom.var = "True";
	else if (value == false)
		atom.var = "False";
}

Expression::Expression(double value)
{
	atom.atomType = NumberType;
	atom.number = value;
}

Expression::Expression(const std::string & value)
{
	atom.atomType = SymbolType;
	atom.var = value;
}

Expression::Expression(std::tuple<double, double> value)
{
	atom.atomType = PointType;
	atom.point = value;
	atom.truthValue = false;
}

Expression::Expression(std::tuple<double, double> start, std::tuple<double, double> end)
{
	atom.atomType = LineType;
	atom.point = start;
	atom.point2 = end;
	atom.truthValue = false;
}

Expression::Expression(std::tuple<double, double> center, std::tuple<double, double> start, double angle)
{
	atom.atomType = ArcType;
	atom.point = center;
	atom.point2 = start;
	atom.number = angle;
	atom.truthValue = false;
}

void Expression::defineMethod()
{
	if (atom.var == "+" && children.size() != 0) {
		atom.number = 0;
		for (unsigned int i = 0; i < children.size(); i++) {
			if (children[i]->atom.atomType == NumberType) {
				atom.number += children[i]->atom.number;
				atom.atomType = NumberType;
			}
			else
				throw InterpreterSemanticError("Error: Improper arguments for addition.");
		}
		euthanizeChildren();
	}
	else if (atom.var == "*" && children.size() != 0) {
		atom.number = 1;
		for (unsigned int i = 0; i < children.size(); i++) {
			if (children[i]->atom.atomType == NumberType) {
				atom.number *= children[i]->atom.number;
				atom.atomType = NumberType;
			}
			else
				throw InterpreterSemanticError("Error: Improper arguments for multiplication.");
		}
		euthanizeChildren();
	}

	else if (atom.var == "and" && children.size() != 0) {
		atom.truthValue = true;
		for (unsigned int i = 0; i < children.size(); i++) {
			if (children[i]->atom.atomType == BoolType) {
				atom.truthValue &= children[i]->atom.truthValue;
				atom.atomType = BoolType;
			}
			else
				throw InterpreterSemanticError("Error: Improper arguments for logic operator \"and\".");
		}
		euthanizeChildren();
	}
	else if (atom.var == "or" && children.size() != 0) {
		atom.truthValue = false;
		for (unsigned int i = 0; i < children.size(); i++) {
			if (children[i]->atom.atomType == BoolType) {
				atom.truthValue |= children[i]->atom.truthValue;
				atom.atomType = BoolType;
			}
			else
				throw InterpreterSemanticError("Error: Improper arguments for logic operator \"or\".");
		}
		euthanizeChildren();
	}
	else if (atom.var == "-") {
		if (children.size() == 2 && children[0]->atom.atomType == NumberType && children[1]->atom.atomType == NumberType) {
			atom.number = children[0]->atom.number - children[1]->atom.number;
			atom.atomType = NumberType;

		}
		else if (children.size() == 1 && children[0]->atom.atomType == NumberType) {
			atom.number = children[0]->atom.number * -1;
			atom.atomType = NumberType;

		}
		else
			throw InterpreterSemanticError("Error: Improper arguments for subtraction.");

		euthanizeChildren();
	}
	else if (atom.var == "/") {
		if (children.size() == 2 && children[0]->atom.atomType == NumberType && children[1]->atom.atomType == NumberType) {
			atom.number = children[0]->atom.number / children[1]->atom.number;
			atom.atomType = NumberType;
		}
		else
			throw InterpreterSemanticError("Error: Improper arguments for division.");

		euthanizeChildren();
	}

	else if (atom.var == "<") {
		if (children.size() == 2 && children[0]->atom.atomType == NumberType && children[1]->atom.atomType == NumberType) {
			children[0]->atom.number < children[1]->atom.number ? atom.truthValue = true : atom.truthValue = false;
			atom.atomType = BoolType;
		}
		else
			throw InterpreterSemanticError("Error: Improper arguments for logic operator \"<\".");

		euthanizeChildren();
	}
	else if (atom.var == "<=") {
		if (children.size() == 2 && children[0]->atom.atomType == NumberType && children[1]->atom.atomType == NumberType) {
			children[0]->atom.number <= children[1]->atom.number ? atom.truthValue = true : atom.truthValue = false;
			atom.atomType = BoolType;
		}
		else
			throw InterpreterSemanticError("Error: Improper arguments for logic operator \"<=\".");

		euthanizeChildren();
	}
	else if (atom.var == ">") {
		if (children.size() == 2 && children[0]->atom.atomType == NumberType && children[1]->atom.atomType == NumberType) {
			children[0]->atom.number > children[1]->atom.number ? atom.truthValue = true : atom.truthValue = false;
			atom.atomType = BoolType;
		}
		else
			throw InterpreterSemanticError("Error: Improper arguments for logic operator \">\".");

		euthanizeChildren();
	}
	else if (atom.var == ">=") {
		if (children.size() == 2 && children[0]->atom.atomType == NumberType && children[1]->atom.atomType == NumberType) {
			children[0]->atom.number >= children[1]->atom.number ? atom.truthValue = true : atom.truthValue = false;
			atom.atomType = BoolType;
		}
		else
			throw InterpreterSemanticError("Error: Improper arguments for logic operator \">=\".");

		euthanizeChildren();
	}
	else if (atom.var == "=") {
		if (children.size() == 2 && children[0]->atom.atomType == NumberType && children[1]->atom.atomType == NumberType) {
			children[0]->atom.number == children[1]->atom.number ? atom.truthValue = true : atom.truthValue = false;
			atom.atomType = BoolType;
		}
		else
			throw InterpreterSemanticError("Error: Improper arguments for logic operator \"=\".");

		euthanizeChildren();
	}
	else if (atom.var == "not") {
		if (children.size() == 1 && children[0]->atom.atomType == BoolType) {
			atom.truthValue = !children[0]->atom.truthValue;
			atom.atomType = BoolType;
		}
		else
			throw InterpreterSemanticError("Error: Improper arguments for logic operator \"not\".");

		euthanizeChildren();
	}
	// project 2
	else if (atom.var == "sin") {
		if (children.size() == 1 && children[0]->atom.atomType == NumberType) {
			atom.number = sin(children[0]->atom.number);
			atom.atomType = NumberType;
		}
		else
			throw InterpreterSemanticError("Error: Improper arguments for logic operator \"not\".");

		euthanizeChildren();
	}
	else if (atom.var == "cos") {
		if (children.size() == 1 && children[0]->atom.atomType == NumberType) {
			atom.number = cos(children[0]->atom.number);
			atom.atomType = NumberType;
		}
		else
			throw InterpreterSemanticError("Error: Improper arguments for logic operator \"not\".");

		euthanizeChildren();
	}
	else if (atom.var == "arctan") {
		if (children.size() == 2 && children[0]->atom.atomType == NumberType && children[1]->atom.atomType == NumberType) {
			atom.number = atan2(children[0]->atom.number, children[1]->atom.number);
			atom.atomType = NumberType;
		}
		else
			throw InterpreterSemanticError("Error: Improper arguments for logic operator \">=\".");

		euthanizeChildren();
	}
	else if (atom.var == "point") {
		if (atom.truthValue == true && parent != nullptr && parent->atom.var == "draw")
				environment->graphics.push_back(atom);

		else if (children.size() == 2 && children[0]->atom.atomType == NumberType && children[1]->atom.atomType == NumberType)
		{
			atom.point = make_tuple(children[0]->atom.number, children[1]->atom.number);
			atom.atomType = PointType;
			if (parent != nullptr && parent->atom.var == "draw")
				environment->graphics.push_back(atom);
		}
		else
			throw InterpreterSemanticError("Error: Improper arguments for making a point.");

		euthanizeChildren();
	}
	else if (atom.var == "line") {
		bool flag = false;
		if (atom.truthValue == true && parent != nullptr && parent->atom.var == "draw")
			environment->graphics.push_back(atom); flag = true;

		if (children.size() == 2 && children[0]->atom.atomType == PointType && children[1]->atom.atomType == PointType)
		{
			atom.point = children[0]->atom.point;
			atom.point2 = children[1]->atom.point;
			atom.atomType = LineType;
			if (parent != nullptr && parent->atom.var == "draw")
				environment->graphics.push_back(atom);
		}
		else if (flag == false)
			throw InterpreterSemanticError("Error: Improper arguments for making a line.");
		euthanizeChildren();
	}
	else if (atom.var == "arc") {
		bool flag = false;
		if (atom.truthValue == true && parent != nullptr && parent->atom.var == "draw")
			environment->graphics.push_back(atom); flag = true;

		if (children.size() == 3 && children[0]->atom.atomType == PointType && children[1]->atom.atomType == PointType && children[2]->atom.atomType == NumberType)
		{
			atom.point = children[0]->atom.point;
			atom.point2 = children[1]->atom.point;
			atom.number = children[2]->atom.number;
			atom.atomType = ArcType;
			if (parent != nullptr && parent->atom.var == "draw")
				environment->graphics.push_back(atom);
		}
		else if (flag == false)
			throw InterpreterSemanticError("Error: Improper arguments for making a line.");
		euthanizeChildren();
	}
	//
	else if (atom.atomType == SymbolType)
	{
		atom = environment->findVar(atom.var);

		euthanizeChildren();
		if (atom.atomType == NoneType)
			throw InterpreterSemanticError("Error: Unknown operation.");
	}
}

Expression Expression::evaluateTree()
{
	if (atom.var == "begin"){
		if (children.size() == 0)
			throw InterpreterSemanticError("Error: Cannot evaluate.");
		for (unsigned int i = 0; i < children.size(); i++)
			children[i]->evaluateTree();
		
		
		atom.atomType = children[children.size() - 1]->atom.atomType;
		atom.number = children[children.size() - 1]->atom.number;
		atom.truthValue = children[children.size() - 1]->atom.truthValue;
		atom.var = children[children.size() - 1]->atom.var;

		euthanizeChildren();
	}
	else if (atom.var == "define"){
		if (children.size() == 2 && children[0]->atom.atomType == SymbolType)
		{
			children[1]->evaluateTree();
			if (children[1]->atom.atomType == BoolType || children[1]->atom.atomType == NumberType || 
				children[1]->atom.atomType == PointType || children[1]->atom.atomType == LineType || children[1]->atom.atomType == ArcType)
				environment->setVariable(children[0]->atom.var, children[1]->atom);
			atom = environment->findVar(children[0]->atom.var);

			euthanizeChildren();
		}
		else
			throw InterpreterSemanticError("Error: Cannot evaluate.");
	}
	else if (atom.var == "if"){
		if (children.size() != 3)
			throw InterpreterSemanticError("Error: Cannot evaluate \"if\". Incorrect amount of children.");

		children[0]->evaluateTree();
		if (children[0]->atom.atomType == BoolType)
		{
			if (children[0]->atom.truthValue == true)
			{
				children[1]->evaluateTree();
				atom.atomType = children[1]->atom.atomType;
				atom.number = children[1]->atom.number;
				atom.truthValue = children[1]->atom.truthValue;
				atom.var = children[1]->atom.var;
				
				euthanizeChildren();
			}
			else if (children[0]->atom.truthValue == false)
			{
				children[2]->evaluateTree();
				atom.atomType = children[2]->atom.atomType;
				atom.number = children[2]->atom.number;
				atom.truthValue = children[2]->atom.truthValue;
				atom.var = children[2]->atom.var;

				euthanizeChildren();
			}
		}

		else {
			throw InterpreterSemanticError("Error: Cannot evaluate. First expression of \"if\" did not return a bool type.");
		}
	}

	else if (atom.var == "draw") {
		if (children.size() == 0)
			throw InterpreterSemanticError("Error: Cannot evaluate.");

		for (unsigned int i = 0; i < children.size(); i++) {
			if (children[i]->atom.atomType == SymbolType)
			{
				children[i]->atom = environment->findVar(children[i]->atom.var);
				children[i]->atom.truthValue = true;
				if (children[i]->atom.atomType == NoneType)
					throw InterpreterSemanticError("Error: Unknown operation.");
			}
			else if (children[i]->atom.atomType != PointType && children[i]->atom.atomType != LineType && children[i]->atom.atomType != ArcType) 
			{
				euthanizeChildren();
				throw InterpreterSemanticError("Error: Cannot evaluate.");
			}
		}

		for (unsigned int i = 0; i < children.size(); i++) {
			if (children[i]->atom.atomType == PointType || children[i]->atom.atomType == LineType || children[i]->atom.atomType == ArcType) {
					children[i]->evaluateTree();
			}
			else {
				euthanizeChildren();
				throw InterpreterSemanticError("Error: Cannot evaluate.");
			}
		}

		atom.atomType = NoneType;
		atom.var = "None";
		atom.point = children[0]->atom.point;

		euthanizeChildren();
	}

	else// if (atom.atomType == SymbolType) or point or arc or line
	{
		for (unsigned int i = 0; i < children.size(); i++){
			children[i]->evaluateTree();
		}
		defineMethod();
	}

	return *this;
}

void Expression::outputFinalAnswer()
{
	if (atom.atomType == NumberType)
		cout << "(" << atom.number << ")" << endl;
	
	else if (atom.atomType == SymbolType)
		cout << "(" << atom.var << ")" << endl;

	else if (atom.atomType == BoolType)
	{
		if (atom.truthValue == true)
			cout << "(" << "True" << ")" << endl;
		else if (atom.truthValue == false)
			cout << "(" << "False" << ")" << endl;
	}
		
	
}

bool Expression::operator==(const Expression & exp) const noexcept
{
	if (atom.atomType == exp.atom.atomType && atom.atomType == BoolType)
		return (atom.truthValue == exp.atom.truthValue && children.size() == exp.children.size());

	else if (atom.atomType == exp.atom.atomType && atom.atomType == NumberType) {

		return (almost_equal(atom.number, exp.atom.number) && children.size() == exp.children.size());
	}

	else if (atom.atomType == exp.atom.atomType && atom.atomType == SymbolType)
		return (atom.var == exp.atom.var && children.size() == exp.children.size());

	else if (atom.atomType == exp.atom.atomType && atom.atomType == PointType)
		return (atom.point == exp.atom.point && children.size() == exp.children.size());

	else if (atom.atomType == exp.atom.atomType && atom.atomType == LineType)
		return (atom.point == exp.atom.point && atom.point2 == exp.atom.point2 && children.size() == exp.children.size());

	else if (atom.atomType == exp.atom.atomType && atom.atomType == ArcType)
		return (atom.point == exp.atom.point && atom.point2 == exp.atom.point2 && atom.number == exp.atom.number &&
			children.size() == exp.children.size());

	return false;
}

void Expression::euthanizeChildren() {
	while (!children.empty()) {
		delete children[0];
		children.erase(children.begin());
	}
}

bool almost_equal(double x, double y)
{
	// the machine epsilon has to be scaled to the magnitude of the values used
	// and multiplied by the desired precision in ULPs (units in the last place)
	return std::abs(x - y) < numeric_limits<double>::epsilon();
}