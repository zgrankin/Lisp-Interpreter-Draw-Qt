#include <QApplication>
#include <iostream>
#include <fstream>

#include "main_window.hpp"

using namespace std;

int main(int argc, char *argv[])
{
	QApplication app(argc, argv);

	if (argc == 2) {
		string filename = argv[1];

		MainWindow widget(filename);

		widget.show();
		return app.exec();
	}

	else if (argc == 1) {
		MainWindow widget;
		widget.show();
		return app.exec();
	}

	else {
		cerr << "Error: Improper arguments for vtdraw." << endl;
		return EXIT_FAILURE;
	}

	return app.exec();
}