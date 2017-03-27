#include "main_window.hpp"

#include <fstream>
#include <QWidget>
#include <QLayout>
#include <QGraphicsScene>
#include <QGraphicsView>

MainWindow::MainWindow(QWidget * parent) {
	setMinimumSize(800, 600);
	resize(800, 600);

	auto layout = new QGridLayout(this);
	messageDisplay = new MessageWidget(this);
	replDisplay = new REPLWidget(this);
	canvasDisplay = new CanvasWidget(this);
	//textbox->info("hi");
	layout->addWidget(canvasDisplay, 1, 0);
	layout->addWidget(messageDisplay, 0, 0);
	layout->addWidget(replDisplay, 2, 0);
	setLayout(layout);

	intercept = new QtInterpreter(this);
	QObject::connect(replDisplay, SIGNAL(lineEntered(QString)), intercept, SLOT(parseAndEvaluate(QString)));
	QObject::connect(intercept, SIGNAL(info(QString)), messageDisplay, SLOT(info(QString)));
	QObject::connect(intercept, SIGNAL(error(QString)), messageDisplay, SLOT(error(QString)));
	QObject::connect(intercept, SIGNAL(drawGraphic(QGraphicsItem *)), canvasDisplay, SLOT(addGraphic(QGraphicsItem *)));
}

MainWindow::MainWindow(std::string filename, QWidget * parent)
{
	setMinimumSize(800, 600);
	resize(800, 600);

	auto layout = new QGridLayout(this);
	messageDisplay = new MessageWidget(this);
	replDisplay = new REPLWidget(this);
	canvasDisplay = new CanvasWidget(this);
	//textbox->info("hi");
	layout->addWidget(canvasDisplay, 1, 0);
	layout->addWidget(messageDisplay, 0, 0);
	layout->addWidget(replDisplay, 2, 0);
	setLayout(layout);

	intercept = new QtInterpreter(this);

	QObject::connect(replDisplay, SIGNAL(lineEntered(QString)), intercept, SLOT(parseAndEvaluate(QString)));
	QObject::connect(intercept, SIGNAL(info(QString)), messageDisplay, SLOT(info(QString)));
	QObject::connect(intercept, SIGNAL(error(QString)), messageDisplay, SLOT(error(QString)));
	QObject::connect(intercept, SIGNAL(drawGraphic(QGraphicsItem *)), canvasDisplay, SLOT(addGraphic(QGraphicsItem *)));

	ifstream instream(filename);
	intercept->parseAndEvaluate(streamToQString(instream));
}

QString MainWindow::streamToQString(std::istream & expression)
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
	}
	QString qtFinalExpression;
	qtFinalExpression = qtFinalExpression.fromStdString(finalExpression);

	return qtFinalExpression;

}
