#include "main_window.hpp"

#include <QWidget>
#include <QLayout>

MainWindow::MainWindow(QWidget * parent) {
	setMinimumSize(800, 600);
	resize(800, 600);

	auto layout = new QGridLayout(this);
	messageDisplay = new MessageWidget(this);
	replDisplay = new REPLWidget(this);
	//textbox->info("hi");
	layout->addWidget(messageDisplay, 0, 0);

	layout->addWidget(replDisplay, 2, 0);
	setLayout(layout);


	intercept = new QtInterpreter(this);
	QObject::connect(replDisplay, SIGNAL(lineEntered(QString)), intercept, SLOT(parseAndEvaluate(QString)));
	QObject::connect(intercept, SIGNAL(info(QString)), messageDisplay, SLOT(info(QString)));
	QObject::connect(intercept, SIGNAL(error(QString)), messageDisplay, SLOT(error(QString)));


}

//MainWindow::MainWindow(std::string filename, QWidget * parent = nullptr) {
//
//}
