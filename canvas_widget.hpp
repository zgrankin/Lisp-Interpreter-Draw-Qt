#ifndef _CANVAS_WIDGET_H_
#define _CANVAS_WIDGET_H_

class CanvasWidget : public QObject {
	// Default construct a CanvasWidget
	CanvasWidget(QWidget * parent = nullptr);

	// A public slot that accepts a signal in the form of a QGraphicsItem pointer containing an 
	// object derived from QGraphicsItem to draw
	void addGraphic(QGraphicsItem * item);
};

#endif