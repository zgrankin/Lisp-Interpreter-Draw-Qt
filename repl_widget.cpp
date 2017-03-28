#include "repl_widget.hpp"

#include <QLineEdit>
#include <QLabel>
#include <QWidget>
#include <QLayout>
#include <QKeyEvent>
#include <iostream>

REPLWidget::REPLWidget(QWidget * parent) : QWidget(parent)
{
	history.push_back("");
	historyPosition = 0;
	replBox = new QLineEdit(this);
	replLabel = new QLabel("vtscript> ", this);

	auto layout = new QGridLayout(this);

	layout->addWidget(replLabel, 2, 0);
	layout->addWidget(replBox, 2, 1);
	setLayout(layout);

	QObject::connect(replBox, SIGNAL(returnPressed()), this, SLOT(replSignalHold()));
}

void REPLWidget::replSignalHold()
{
	emit lineEntered(replBox->text());
	history.insert(1, replBox->text());
	historyPosition = 0;
	replBox->clear();
}


void REPLWidget::keyPressEvent(QKeyEvent *e)
{
	if (e->key() == 0x01000013 && historyPosition < history.size()) { // up key Qt::Key_Up
		if (historyPosition != history.size() - 1)
			historyPosition++;
		replBox->clear();
		replBox->setText(history[historyPosition]);
	}
	else if (e->key() == 0x01000015 && historyPosition < history.size()) { // down key Qt::Key_Down
		if (historyPosition > 0)
			historyPosition--;
		replBox->clear();
		replBox->setText(history[historyPosition]);		
	}
	QWidget::keyPressEvent(e);
}
