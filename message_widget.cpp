#include "message_widget.hpp"
#include "interpreter.hpp"

#include <QWidget>
#include <QLabel>
#include <QPalette>
#include <QDoubleSpinBox>
#include <QLayout>
#include <QLineEdit>

MessageWidget::MessageWidget(QWidget * parent) {

	messageLabel = new QLabel("Message: ", this);
	messageBox = new QLineEdit(this);
	messageBox->setReadOnly(true);

	//QGridLayout * layout = new QGridLayout(this);
	auto layout = new QGridLayout(this); // or use auto
	//QLayout * layout = new QGridLayout(this);

	layout->addWidget(messageBox, 0, 1);
	layout->addWidget(messageLabel, 0, 0);
	setLayout(layout);

	failureRed.setColor(QPalette::Highlight, Qt::red);
}

void MessageWidget::info(QString message)
{
	messageBox->clear();
	messageBox->insert(message);
	messageBox->setPalette(successBlue);
}

void MessageWidget::error(QString message)
{
	string hello;
	hello = message.toStdString();
	messageBox->clear();
	messageBox->insert(message);
	messageBox->setPalette(failureRed);
	messageBox->selectAll();
}
