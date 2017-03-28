#ifndef _QGRAPHICSARC_Item_H_
#define _QGRAPHICSARC_Item_H_

#include <QGraphicsEllipseItem>
#include <tuple>

using std::tuple;
using std::make_tuple;
using std::get;

class QGraphicsArcItem : public QGraphicsEllipseItem {

public:
	QGraphicsArcItem(QGraphicsItem * parent = nullptr);
	QGraphicsArcItem(QGraphicsItem * parent, tuple<double, double> point1, tuple<double, double> point2, double angle);
	void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget = Q_NULLPTR);
	void doSomeCalculationsAndJanxRelatedToArcs();

private:
	tuple<double, double> center;
	tuple<double, double> start;
	double angleRadians;
	double angleDegrees;
};

#endif