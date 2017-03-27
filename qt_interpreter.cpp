#include "interpreter.hpp"
#include "qt_interpreter.hpp"
#include "interpreter_semantic_error.hpp"
#include "qgraphics_arc_item.hpp"

#include <string>
#include <QPixmap>
#include <QColor>
#include <QGraphicsScene>
#include <QPainter>
using std::string;

QtInterpreter::QtInterpreter(QObject * parent) : QObject(parent)
{
	/*QPainter painter;
	painter.setBrush(QBrush(Qt::black));
	painter.drawEllipse(x, y, 150, 50, 0, 16 * 360);
	graphic = new QGraphicsPixmapItem(QPixmap::fromImage);*/

	/*QPen pen;
	pen.setColor(QColor(Qt::black));
	QGraphicsEllipseItem * item = new QGraphicsEllipseItem(x, y, 4, 4, pen, QBrush(Qt::SolidPattern));
	emit drawGraphic(graphic);*/
}

void QtInterpreter::parseAndEvaluate(QString entry)
{
	parseThis = entry.toStdString();
	std::istringstream ss(parseThis);
	bool parse = vtscript.parse(ss);
	if (parse == true) {
		try {
			evaluatedExpression = vtscript.expressionToString(vtscript.eval());
			if (evaluatedExpression == "(None)" && vtscript.theEnvironment.graphics.size() > 0) {
				std::size_t SIZE = vtscript.theEnvironment.graphics.size();
				for (int i = 0; i < SIZE; i++){
					if (vtscript.theEnvironment.graphics[i].atomType == PointType) {
						auto point = vtscript.theEnvironment.graphics[i].point;
						double x = get<0>(point);
						double y = get<1>(point);
						QGraphicsEllipseItem * graphic = new QGraphicsEllipseItem();
						graphic->setRect(qreal(x-2), qreal(y-2), qreal(4), qreal(4)); // first two arguments mark the top left corner
						graphic->setBrush(QBrush(Qt::black));
						emit drawGraphic(graphic);	
					}
					else if (vtscript.theEnvironment.graphics[i].atomType == LineType) {
						auto start = vtscript.theEnvironment.graphics[i].point;
						auto end = vtscript.theEnvironment.graphics[i].point2;
						double x1 = get<0>(start);
						double y1 = get<1>(start);
						double x2 = get<0>(end);
						double y2 = get<1>(end);
						QGraphicsLineItem * graphic = new QGraphicsLineItem();
						graphic->setLine(qreal(x1), qreal(y1), qreal(x2), qreal(y2));
						emit drawGraphic(graphic);
					}
					else if (vtscript.theEnvironment.graphics[i].atomType == ArcType) {
						auto center = vtscript.theEnvironment.graphics[i].point;
						auto start = vtscript.theEnvironment.graphics[i].point2;
						double radians = vtscript.theEnvironment.graphics[i].number;
						QGraphicsArcItem * graphic = new QGraphicsArcItem(nullptr, center, start, radians);
						emit drawGraphic(graphic);
					}
				}
			}
			emit info(QString(evaluatedExpression.c_str()));
		}
		catch (InterpreterSemanticError  &e) {
			emit error(QString("Error: could not evaluate expression."));
		}		
	}
	else {
		emit error(QString("Error: Could not parse input."));
	}
}