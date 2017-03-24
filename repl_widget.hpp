#ifndef _REPL_WIDGET_H_
#define _REPL_WIDGET_H_

#include <QWidget>
#include <QVector>

class QLineEdit;
class QLabel;
class QLayout;


class REPLWidget : public QWidget {
Q_OBJECT

protected:
	//void keyPressEvent(QKeyEvent *e);

public:
	// Default construct a REPLWidget
	REPLWidget(QWidget * parent = nullptr);


signals:
	// A signal that sends the current edited text as a QString when the return key is pressed.
	void lineEntered(QString);

private slots:
	void replSignalHold();
	void keyPressSignalHold();

private:
	QLineEdit * replBox;
	QLabel * replLabel;

	QVector<QString> history;
};

#endif