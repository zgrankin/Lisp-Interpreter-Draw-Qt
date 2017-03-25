#include "interpreter.hpp"
#include "qt_interpreter.hpp"

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
		}
		catch (InterpreterSemanticError) {
			emit error(QString("Error: could not evaluate expression."));
		}

		emit info(QString(evaluatedExpression.c_str()));
	}
	else {
		evaluatedExpression = "Error: Could not parse input.";
		emit error(QString("Error: could not parse input."));
	}
}