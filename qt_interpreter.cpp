#include "interpreter.hpp"
#include "qt_interpreter.hpp"
#include "interpreter_semantic_error.hpp"

#include <string>
using std::string;

QtInterpreter::QtInterpreter(QObject * parent) : QObject(parent)
{

}

void QtInterpreter::parseAndEvaluate(QString entry)
{
	parseThis = entry.toStdString();
	std::istringstream ss(parseThis);
	bool parse = vtscript.parse(ss);
	if (parse == true) {
		try {
			evaluatedExpression = vtscript.expressionToString(vtscript.eval());
			emit info(QString(evaluatedExpression.c_str()));
		}
		catch (InterpreterSemanticError  &e) {
			emit error(QString("Error: could not evaluate expression."));
		}		
	}
	else {
		emit error(QString("Error: Could not parse input."));
	}
}