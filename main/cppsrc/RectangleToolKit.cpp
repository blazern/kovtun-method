#include "RectangleToolKit.h"

QPointF RectangleToolKit::calculateGravityCenter(const Contour &, const QRectF & rectangle)
{
    return rectangle.center();
}

bool RectangleToolKit::isAnyPointOfRectangleInsideOfContour(const Contour & contour, const QRectF & rectangle)
{
    if (contour.containsInside(rectangle.topLeft()) || contour.containsInside(rectangle.topRight())
        || contour.containsInside(rectangle.bottomLeft()) || contour.containsInside(rectangle.bottomRight()))
    {
        return true;
    }

    const QLineF northLine(rectangle.topLeft(), rectangle.topRight());
    const QLineF eastLine(rectangle.topRight(), rectangle.bottomRight());
    const QLineF southLine(rectangle.bottomRight(), rectangle.bottomLeft());
    const QLineF westLine(rectangle.bottomLeft(), rectangle.topLeft());

    const QLineF * const contourLines = contour.getLines();
    for (int index = 0; index < contour.getLinesCount(); index++)
    {
        const QLineF & currentLine = contourLines[index];

        if (currentLine.intersect(northLine, nullptr) == QLineF::BoundedIntersection
            || currentLine.intersect(eastLine, nullptr) == QLineF::BoundedIntersection
            || currentLine.intersect(southLine, nullptr) == QLineF::BoundedIntersection
            || currentLine.intersect(westLine, nullptr) == QLineF::BoundedIntersection)
        {
            return true;
        }
    }
    return false;
}
