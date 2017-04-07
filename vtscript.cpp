#include "interpreter.hpp"
#include <fstream>
#include <string>

bool evaluateCommandLine(int argc, char *argv[])
{
	string expression, argument;
	argument = argv[1];
	if (argument == "-e") {

		expression = argv[2];

		std::istringstream ss(expression);
		Interpreter a;

		if (a.parse(ss)) {
			try {
				cout << a.expressionToString(a.eval()) << endl;
			}
			catch (InterpreterSemanticError &error) {
				std::cout << error.what() << std::endl;
				return false;
			}
		}
		else {
			std::cerr << "Error: Parsing failed." << std::endl;
			return false;
		}
	}
	return true;
}

bool evaluateFile(int argc, char *argv[])
{
	std::string filename = argv[1];
	std::ifstream instream(filename);

	Interpreter a;

	if (a.parse(instream)) {
		try {
			//a.eval().outputFinalAnswer();
			cout << a.expressionToString(a.eval()) << endl;
		}
		catch (InterpreterSemanticError &error) {
			std::cout << error.what() << std::endl;
			return false;
		}
	}
	else {
		std::cerr << "Error: Parsing failed." << std::endl;
		return false;
	}
	return true;
}

int main(int argc, char *argv[])
{
	string expression, argument;
	if (argc == 3) {
		if (!evaluateCommandLine(argc, argv)) {
			return EXIT_FAILURE;
		}
	}

	else if (argc == 2) {
		if (!evaluateFile(argc, argv)) {
			return EXIT_FAILURE;
		}
	}

	else if (argc == 1)
	{
		std::string b;
		Interpreter a;
		while (!cin.eof()) {
			std::cout << "vtscript> ";
			getline(cin, b);
			std::istringstream ss(b);
			if (a.parse(ss)) {
				try {
					cout << a.expressionToString(a.eval()) << endl;
				}
				catch (InterpreterSemanticError &error) {
					std::cerr << error.what() << std::endl;
				}
			}
			else if (!cin.eof()) {
				std::cerr << "Error: Parsing failed." << std::endl;
			}
		}
	}
	else {
		std::cerr << "Error: Invalid Arguments." << std::endl;
	}

	return 0;
}