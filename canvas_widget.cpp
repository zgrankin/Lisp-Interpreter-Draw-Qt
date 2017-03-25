#include "canvas_widget.hpp"

#include <QWidget>
#include <QGraphicsItem>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QLayout>

CanvasWidget::CanvasWidget(QWidget * parent)
{
	scene = new QGraphicsScene(this);
	view = new QGraphicsView(this);
	view->setScene(scene);

	auto layout = new QGridLayout;


	layout->addWidget(view, 0, 0);
	setLayout(layout);
}

void CanvasWidget::addGraphic(QGraphicsItem * item)
{
	scene->addItem(item);
}


