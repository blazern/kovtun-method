#ifndef UNITSTOOLKIT_H
#define UNITSTOOLKIT_H

#include <QPair>
#include <QPointF>
#include <QRectF>
#include "ClosedContour.h"
#include "MyQRectF.h"

namespace KovtunMethod
{

class UnitsToolKit
{
    UnitsToolKit() = delete;
    ~UnitsToolKit() = delete;
    UnitsToolKit(const UnitsToolKit &) = delete;
    UnitsToolKit & operator=(const UnitsToolKit &) = delete;

public:
    // Возвращаемое значение для случая, когда контур не лежит в прямоугольнике (либо лежит на его границе), не определено
    // Используйте isAnyPointOfAnyLineOfContourInsideOfRectangle() для проверки.
    // Возвращает центр тяжести и погрешность вычислений.
    static QPair<QPointF, double> calculateGravityCenter(
            const ClosedContour & contour,
            const QRectF & rectangle,
            const int maximumUnits = 20); // throws NotSingleShape, если обнаружено,
                                          // что в принятом rectangle больше одной фигуры,
                                          // потому что центр тяжести можно определить только для 1 фигуры.

    static QVector<QSharedPointer<MyQRectF> > getDescribingRectanglesFrom(
            const ClosedContour & contour,
            const MyQRectF & rectangle,
            const int maximumUnits = 20);

private:
    static const QPointF calculateAveragePoint(const QVector<QPointF> & points);
    static void putAllUnitNeighborsWhichItersectRectangleToVector(
            const QRectF & targetRectangle,
            const QRectF & unit,
            const int horIndex,
            const int vertIndex,
            const ClosedContour & contour,
            QVector<QPoint> & neighbors);

    static QVector<QVector<QPoint> > extractUnitsGroups(
            const ClosedContour & contour,
            const MyQRectF & rectangle,
            const int maximumUnits,
            const qreal unitWidth,
            const qreal unitHeight);

    static void setToFarthestValues(
            const QVector<QPoint> & unitsGroup,
            const QRectF & rectangle,
            const qreal unitWidth,
            const qreal unitHeight,
            qreal & north,
            qreal & east,
            qreal & south,
            qreal & west);

};

}

#endif // UNITSTOOLKIT_H
