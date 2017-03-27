#ifndef _MAIN_WINDOW_H_
#define _MAIN_WINDOW_H_
#include "message_widget.hpp"
#include "repl_widget.hpp"
#include "qt_interpreter.hpp"
#include "canvas_widget.hpp"

#include <QWidget>

class MainWindow : public QWidget {
Q_OBJECT

public:
	// Default construct a MainWindow
	MainWindow(QWidget * parent = nullptr);

	// Default construct a MainWidow, using filename as the script file to attempt to preload
	MainWindow(std::string filename, QWidget * parent = nullptr);

	QString streamToQString(std::istream & expression);

private:
	MessageWidget * messageDisplay;
	REPLWidget * replDisplay;
	CanvasWidget * canvasDisplay;
	QtInterpreter * intercept;

};

#endif