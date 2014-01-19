#ifndef RECTANGLETOOLKIT_H
#define RECTANGLETOOLKIT_H

#include <QRectF>
#include <QVector>
#include "ClosedContour.h"

class RectangleToolKit
{
    RectangleToolKit() = delete;
    ~RectangleToolKit() = delete;
    RectangleToolKit(const RectangleToolKit &) = delete;
    RectangleToolKit & operator=(const RectangleToolKit &) = delete;

public:
    static bool isAnyPointOfRectangleInsideOfContour(const ClosedContour & contour, const QRectF & rectangle);

    static bool isAnyPointOfAnyLineOfContourInsideOfRectangle(const ClosedContour & contour, const QRectF & rectangle);

    static bool doLinesIntersectWithoutLyingOnEachOther(const QLineF & firstLine, const QLineF & secondLine);

    // Возвращаемое значение для случая, когда контур не лежит в прямоугольнике (либо лежит на его границе), не определено
    // Используйте isAnyPointOfAnyLineOfContourInsideOfRectangle() для проверки.
    static QPointF calculateGravityCenter(const ClosedContour & contour, const QRectF & rectangle, const int maximumUnits = 20);

    // Имеется в виду только касание, т.е. вызов функции для пересекающихся прямо угольников вернёт false
    static bool doRectanglesTouchEachOther(const QRectF & firstRectangle, const QRectF & secondRectangle);

private:
    static bool rectangleContainsInside(const QRectF & rectangle, const QPointF & point);

    static bool doLeftAndRightOrTopAndBottomSidesTouchEachOther(const QRectF & firstRectangle, const QRectF & secondRectangle);
};

#endif // RECTANGLETOOLKIT_H
