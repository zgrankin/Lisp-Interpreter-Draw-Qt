#include "repl_widget.hpp"

#include <QLineEdit>
#include <QLabel>
#include <QLayout>
#include <iostream>

REPLWidget::REPLWidget(QWidget * parent)
{
	replBox = new QLineEdit(this);
	replLabel = new QLabel("vtscript> ", this);

	auto layout = new QGridLayout(this);

	layout->addWidget(replLabel, 2, 0);
	layout->addWidget(replBox, 2, 1);
	setLayout(layout);

	QObject::connect(replBox, SIGNAL(returnPressed()), this, SLOT(replSignalHold()));
	//QObject::connect(replBox, SIGNAL(keyPressEvent()), this, SLOT(keyPressSignalHold()));
}

void REPLWidget::replSignalHold()
{
	emit lineEntered(replBox->text());
	history.push_back(replBox->text());
	replBox->clear();
}

void REPLWidget::keyPressSignalHold()
{
	std::cout << "Key event grabbed";
}

void keyPressEvent(QKeyEvent *e)
{
	// Otherwise pass to the graphics view
	//QGraphicsView::keyPressEvent(e)
}
