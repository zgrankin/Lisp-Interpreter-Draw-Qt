#include <QApplication>
#include <iostream>
#include <fstream>

#include "main_window.hpp"

using namespace std;

int drawFile(int argc, char *argv[])
{
	QApplication app(argc, argv);
	string filename = argv[1];
	MainWindow widget(filename);
	widget.show();
	return app.exec();
}

int drawNormal(int argc, char *argv[])
{
	QApplication app(argc, argv);
	MainWindow widget;
	widget.show();
	return app.exec();
}

int main(int argc, char *argv[])
{
	if (argc == 2) {
		drawFile(argc, argv);
	}

	else if (argc == 1) {
		drawNormal(argc, argv);
	}

	else {
		cerr << "Error: Improper arguments for vtdraw." << endl;
		return EXIT_FAILURE;
	}
}

