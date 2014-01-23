#ifndef RECTANGLETOOLKIT_H
#define RECTANGLETOOLKIT_H

#include <QRectF>
#include <QVector>
#include <QPair>
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

    static bool doesLineIntersectRectangle(const QLineF & line, const QRectF & rectangle);

    static bool doLinesIntersectWithoutLyingOnEachOther(const QLineF & firstLine, const QLineF & secondLine);

    // Возвращаемое значение для случая, когда контур не лежит в прямоугольнике (либо лежит на его границе), не определено
    // Используйте isAnyPointOfAnyLineOfContourInsideOfRectangle() для проверки.
    // Возвращает центр тяжести и погрешность вычислений.
    static QPair<QPointF, double> calculateGravityCenter(const ClosedContour & contour, const QRectF & rectangle, const int maximumUnits = 20);

    // Имеется в виду только касание, т.е. вызов функции для пересекающихся прямо угольников вернёт false
    static bool doRectanglesTouchEachOther(const QRectF & firstRectangle, const QRectF & secondRectangle);

private:
    static bool rectangleContainsInside(const QRectF & rectangle, const QPointF & point);

    static bool doLeftAndRightOSidesTouchEachOther(const QRectF & firstRectangle, const QRectF & secondRectangle);
    static bool doLeftAndRightSidesTouchEeachOtherRespectively(const QRectF & firstRectangle, const QRectF & secondRectangle);

    static bool doTopAndBottomSidesTouchEachOther(const QRectF & firstRectangle, const QRectF & secondRectangle);
    static bool doTopAndBottomSidesTouchEeachOtherRespectively(const QRectF & firstRectangle, const QRectF & secondRectangle);
};

#endif // RECTANGLETOOLKIT_H
