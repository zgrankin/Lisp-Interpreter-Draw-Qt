#include <QApplication>
#include <iostream>

#include "main_window.hpp"

using namespace std;

int main(int argc, char *argv[])
{
	//cout << "Fuck yeah. QT sux." << endl;
	QApplication app(argc, argv);

	MainWindow widget;
	widget.show();
	//widget.minimumSize
	//widget.showMaximized();

	return app.exec();
}