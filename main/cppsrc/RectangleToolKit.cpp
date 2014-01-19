#include "RectangleToolKit.h"
#include <QDebug>

bool RectangleToolKit::isAnyPointOfRectangleInsideOfContour(const ClosedContour & contour, const QRectF & rectangle)
{
    if (contour.containsInside(rectangle.topLeft()) || contour.containsInside(rectangle.topRight())
        || contour.containsInside(rectangle.bottomLeft()) || contour.containsInside(rectangle.bottomRight()))
    {
        return true;
    }

    return isAnyPointOfAnyLineOfContourInsideOfRectangle(contour, rectangle);
}

bool RectangleToolKit::isAnyPointOfAnyLineOfContourInsideOfRectangle(const ClosedContour & contour, const QRectF & rectangle)
{
    const QPointF * const points = contour.getPoints();
    for (int index = 0; index < contour.getPointsCount(); index++)
    {
        const QPointF & currentPoint = points[index];
        if (rectangleContainsInside(rectangle, currentPoint))
        {
            return true;
        }
    }

    const QLineF northLine(rectangle.topLeft(), rectangle.topRight());
    const QLineF eastLine(rectangle.topRight(), rectangle.bottomRight());
    const QLineF southLine(rectangle.bottomRight(), rectangle.bottomLeft());
    const QLineF westLine(rectangle.bottomLeft(), rectangle.topLeft());

    const QLineF * const contourLines = contour.getLines();
    for (int index = 0; index < contour.getLinesCount(); index++)
    {
        const QLineF & currentLine = contourLines[index];

        if (doLinesIntersectWithoutLyingOnEachOther(currentLine, northLine)
            || doLinesIntersectWithoutLyingOnEachOther(currentLine, eastLine)
            || doLinesIntersectWithoutLyingOnEachOther(currentLine, southLine)
            || doLinesIntersectWithoutLyingOnEachOther(currentLine, westLine))
        {
            return true;
        }
    }
    return false;
}

bool RectangleToolKit::rectangleContainsInside(const QRectF & rectangle, const QPointF & point)
{
    if (rectangle.contains(point))
    {
        if (point.x() != rectangle.left()
            && point.x() != rectangle.right()
            && point.y() != rectangle.top()
            && point.y() != rectangle.bottom())
        {
            return true;
        }
    }
    return false;
}

bool RectangleToolKit::doLinesIntersectWithoutLyingOnEachOther(const QLineF & firstLine, const QLineF & secondLine)
{
    QPointF intersection;
    if (firstLine.intersect(secondLine, &intersection) == QLineF::BoundedIntersection)
    {
        if (intersection != secondLine.p1() && intersection != secondLine.p2()
            && intersection != secondLine.p1() && intersection != secondLine.p2())
        {
            // Если линии пересекаются, то они могут лежать друг на друге.
            // Чтобы сделать проверку ещё и на это, мы сдвигаем одну из линий и проверяем, параллельны ли линии,
            // если да - то они лежали друг на друге.
            const QLineF movedFirstLine(firstLine.x1(), firstLine.y1() + 1, firstLine.x2(), firstLine.y2() + 1);
            if (movedFirstLine.intersect(secondLine, nullptr) != QLineF::NoIntersection)
            {
                return true;
            }
        }
    }

    return false;
}

QPointF RectangleToolKit::calculateGravityCenter(const ClosedContour & contour, const QRectF & rectangle, const int maximumUnits)
{
    qreal rectWidth = rectangle.width();
    qreal rectHeight = rectangle.height();

    qreal unitWidth = rectWidth / maximumUnits;
    qreal unitHeight = rectHeight / maximumUnits;

    QRectF unit;

    QVector<QPointF> insidePoints;

    for (int horIndex = 0; horIndex < maximumUnits; horIndex++)
    {
        const qreal newX = rectangle.left() + horIndex * unitWidth;
        unit.setX(newX);

        for (int vertIndex = 0; vertIndex < maximumUnits; vertIndex++)
        {
            const qreal newY = rectangle.top() + vertIndex * unitHeight;
            unit.setY(newY);
            unit.setWidth(unitWidth);
            unit.setHeight(unitHeight);

            if (isAnyPointOfRectangleInsideOfContour(contour, unit))
            {
                insidePoints.push_back(unit.center());
            }
        }
    }

    qreal averageX = 0;
    qreal averageY = 0;
    const int pointsCount = insidePoints.size();
    for (const auto & point : insidePoints)
    {
        averageX += point.x() / pointsCount;
        averageY += point.y() / pointsCount;
    }

    return QPointF(averageX, averageY);
}

bool RectangleToolKit::doRectanglesTouchEachOther(const QRectF & firstRectangle, const QRectF & secondRectangle)
{
    const bool touched = doLeftAndRightOrTopAndBottomSidesTouchEachOther(firstRectangle, secondRectangle);

    return touched ? true : doLeftAndRightOrTopAndBottomSidesTouchEachOther(secondRectangle, firstRectangle);
}

bool RectangleToolKit::doLeftAndRightOrTopAndBottomSidesTouchEachOther(const QRectF & firstRectangle, const QRectF & secondRectangle)
{
    if (secondRectangle.right() == firstRectangle.left())
    {
        if (secondRectangle.top() >= firstRectangle.top()
            || secondRectangle.bottom() <= firstRectangle.bottom()
            || secondRectangle.center().y() > firstRectangle.top()
            || secondRectangle.center().y() < firstRectangle.bottom())
        {
            return true;
        }
    }

    if (secondRectangle.bottom() == firstRectangle.top())
    {
        if (secondRectangle.right() >= firstRectangle.right()
            || secondRectangle.left() <= firstRectangle.left()
            || secondRectangle.center().x() > firstRectangle.right()
            || secondRectangle.center().x() < firstRectangle.left())
        {
            return true;
        }
    }

    return false;
}
