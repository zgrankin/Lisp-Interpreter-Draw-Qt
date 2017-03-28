#include "interpreter.hpp"
#include <fstream>
#include <string>

int main(int argc, char *argv[])
{
	string expression;
	string argument;

	if (argc == 3)
	{
		argument = argv[1];
		if (argument == "-e") {

			expression = argv[2];

			std::istringstream ss(expression);
			Interpreter a;

			if (a.parse(ss)) {
				try {
					//a.eval().outputFinalAnswer();
					cout << a.expressionToString(a.eval()) << endl;
				}
				catch (InterpreterSemanticError &error) {
					std::cout << error.what() << std::endl;
					return EXIT_FAILURE;
				}
			}
			else {
				std::cerr << "Error: Parsing failed." << std::endl;
				return EXIT_FAILURE;
			}
		}
	}

	else if (argc == 2)
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
				return EXIT_FAILURE;
			}
		}
		else {
			std::cerr << "Error: Parsing failed." << std::endl;
			return EXIT_FAILURE;
		}

	}
	
	else if (argc == 1)
	{
		std::string b;
		Interpreter a;
		while (!cin.eof())
		{
			std::cout << "vtscript> ";
			getline(cin, b);

			std::istringstream ss(b);

			if (a.parse(ss)) {
				try {
					//a.eval().outputFinalAnswer();
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