#include "qgraphics_arc_item.hpp"

#include <QGraphicsEllipseItem>
#include <tuple>
#include <QPainter>
#include <math.h>
#include <cmath>

using std::get;
using std::pow;

QGraphicsArcItem::QGraphicsArcItem(QGraphicsItem * parent, tuple<double, double> point1, tuple<double, double> point2, double angle) {
	center = point1;
	start = point2;
	angleRadians = angle;
	doSomeCalculationsAndJanxRelatedToArcs();
}

void QGraphicsArcItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
	painter->drawArc(rect(), startAngle(), spanAngle()); // not setting these beforehand will cause image to not display immediately
}

void QGraphicsArcItem::doSomeCalculationsAndJanxRelatedToArcs()
{
	double centerX = get<0>(center);
	double centerY = get<1>(center);
	double startX = get<0>(start);
	double startY = get<1>(start);
	double radius = sqrt(pow(startX - centerX, 2) + pow(-startY + centerY, 2));
	double x = centerX - radius;
	double y = centerY - radius;
	double theSpanAngle = angleRadians * (180 / atan2(0, -1));
	double theStartAngle = atan2(-startY + centerY, startX - centerX) * (180 / atan2(0, -1));
	setRect(x, y, radius * 2, radius * 2);
	setStartAngle(16 * theStartAngle);
	setSpanAngle(16 * theSpanAngle);
}

