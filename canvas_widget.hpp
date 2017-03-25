#ifndef _CANVAS_WIDGET_H_
#define _CANVAS_WIDGET_H_

#include <QWidget>

class QGraphicsItem;
class QGraphicsScene;
class QGraphicsView;

class CanvasWidget : public QWidget {
Q_OBJECT

public:
	// Default construct a CanvasWidget
	CanvasWidget(QWidget * parent = nullptr);

public slots:
	// A public slot that accepts a signal in the form of a QGraphicsItem pointer containing an 
	// object derived from QGraphicsItem to draw
	void addGraphic(QGraphicsItem * item);

private:
	QGraphicsScene * scene;
	QGraphicsView * view;
};

#endif