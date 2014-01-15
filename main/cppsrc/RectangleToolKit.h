#ifndef RECTANGLETOOLKIT_H
#define RECTANGLETOOLKIT_H

#include <QRectF>
#include <QVector>
#include "Contour.h"

class RectangleToolKit
{
    RectangleToolKit() = delete;
    ~RectangleToolKit() = delete;
    RectangleToolKit(const RectangleToolKit &) = delete;
    RectangleToolKit & operator=(const RectangleToolKit &) = delete;

public:
    static bool isAnyPointOfRectangleInsideOfContour(const Contour & contour, const QRectF & rectangle);

    static bool isAnyPointOfAnyLineOfContourInsideOfRectangle(const Contour & contour, const QRectF & rectangle);

    static bool doLinesIntersectWithoutLyingOnEachOther(const QLineF & firstLine, const QLineF & secondLine);

    // Возвращаемое значение для случая, когда контур не лежит в прямоугольнике (либо лежит на его границе), не определено
    // Используйте isAnyPointOfAnyLineOfContourInsideOfRectangle() для проверки.
    static QPointF calculateGravityCenter(const Contour & contour, const QRectF & rectangle);

private:
    static bool rectangleContainsInside(const QRectF & rectangle, const QPointF & point);
    static void addIntersectionIfExistsToVector(const QLineF & firstLine, const QLineF & secondLine, QVector<QPointF> & vector);
};

#endif // RECTANGLETOOLKIT_H
