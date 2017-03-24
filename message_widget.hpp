#ifndef _MESSAGE_WIDGET_H_
#define _MESSAGE_WIDGET_H_

#include <QWidget>

class QLineEdit;
class QLayout;
class QLabel;


class MessageWidget : public QWidget {
Q_OBJECT

public:
	// Default construct a MessageWidget displaying no text
	MessageWidget(QWidget * parent = nullptr);

public slots:
	// a public slot accepting an informational message to display, clearing any error formatting
	void info(QString message);

	// a public slot accepting an error message to display as selected text highlighted with a red background.
	void error(QString message);

private:
	QLineEdit * messageBox;
	QLabel * messageLabel;

	QPalette failureRed;
	QPalette successBlue;

};

#endif