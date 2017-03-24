#include "interpreter.hpp"
#include "environment.hpp"

Interpreter::Interpreter()
{
	tree.environment = &theEnvironment;
}


bool Interpreter::parse(std::istream & expression) noexcept
{
	string tokenString = streamToString(expression);
	vector<string> token = tree.tokenize(tokenString);
	return tree.buildAST(token);
}

Expression Interpreter::eval()
{
	Expression a;
	a = tree.head->evaluateTree();
	// tree.postEvalDestroy(tree.head);
	return a;
}

string Interpreter::streamToString(std::istream & expression)
{
	std::string tempExpression;
	std::string finalExpression;
	std::getline(expression, tempExpression);
	while (!expression.fail()) {
		if (tempExpression.find(';') != -1)
			finalExpression.append(tempExpression.substr(0, tempExpression.find(';')));
		else {
			finalExpression.append(tempExpression);
		}

		finalExpression.append(" ");
		std::getline(expression, tempExpression);
		//std::cout << finalExpression << endl;
	}

	return finalExpression;

	/*string tokenString;
	char buffer[4096];
	while (in.read(buffer, sizeof(buffer)))
		tokenString.append(buffer, sizeof(buffer));
	tokenString.append(buffer, in.gcount());*/
	//string tokenString(std::istreambuf_iterator<char>(in), {});
}

string Interpreter::expressionToString(Expression result)
{
	output = "";
	if (result.atom.atomType == NumberType) {
		std::stringstream ss;
		ss << result.atom.number;
		output = "(" + ss.str() + ")";
	}

	else if (result.atom.atomType == SymbolType) {
		output = "(" + result.atom.var + ")";
	}

	else if (result.atom.atomType == BoolType) {
		if (result.atom.truthValue == true) {
			output = "(True)";
		}
		else if (result.atom.truthValue == false) {
			output = "(False)";
		}
	}
	cout << output << endl;
	return output;
}


