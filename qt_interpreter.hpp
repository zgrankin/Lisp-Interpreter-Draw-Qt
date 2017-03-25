#ifndef _QT_INTERPRETER_H_
#define _QT_INTERPRETER_H_

#include "interpreter.hpp"

#include <QObject>
#include <QGraphicsItem>
#include <string>

using std::string;


class QtInterpreter : public QObject {
Q_OBJECT

public:
	// Default construct an QtInterpreter with the default environment and an empty AST
	QtInterpreter(QObject * parent = nullptr);



signals:
	// a signal emitting an informational message
	void info(QString message);

	// a signal emitting an error message
	void error(QString message);	
	
	// a signal emitting a graphic to be drawn as a pointer
	void drawGraphic(QGraphicsItem * item);

public slots:
	// a public slot that accepts and expression string and parses/evaluates it
	void parseAndEvaluate(QString entry);

private:
	Interpreter vtscript;
	string parseThis;
	string evaluatedExpression;
};

#endif