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

bool RectangleToolKit::doesLineIntersectRectangle(const QLineF & line, const QRectF & rectangle)
{
    // Дублирование кода функции isAnyPointOfAnyLineOfContourInsideOfRectangle()
    const QLineF northLine(rectangle.topLeft(), rectangle.topRight());
    const QLineF eastLine(rectangle.topRight(), rectangle.bottomRight());
    const QLineF southLine(rectangle.bottomRight(), rectangle.bottomLeft());
    const QLineF westLine(rectangle.bottomLeft(), rectangle.topLeft());

    if (doLinesIntersectWithoutLyingOnEachOther(line, northLine)
        || doLinesIntersectWithoutLyingOnEachOther(line, eastLine)
        || doLinesIntersectWithoutLyingOnEachOther(line, southLine)
        || doLinesIntersectWithoutLyingOnEachOther(line, westLine))
    {
        return true;
    }
    else
    {
        return false;
    }
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
//        if (intersection != secondLine.p1() && intersection != secondLine.p2()
//            && intersection != secondLine.p1() && intersection != secondLine.p2())
//        {
            // Если линии пересекаются, то они могут лежать друг на друге.
            // Чтобы сделать проверку ещё и на это, мы сдвигаем одну из линий и проверяем, параллельны ли линии,
            // если да - то они лежали друг на друге.
            const QLineF movedFirstLine(firstLine.x1(), firstLine.y1() + 1, firstLine.x2(), firstLine.y2() + 1);
            if (movedFirstLine.intersect(secondLine, nullptr) != QLineF::NoIntersection)
            {
                return true;
            }
//        }
    }

    return false;
}

QPair<QPointF, double> RectangleToolKit::calculateGravityCenter(const ClosedContour & contour, const QRectF & rectangle, const int maximumUnits)
{
    qreal rectWidth = rectangle.width();
    qreal rectHeight = rectangle.height();

    qreal unitWidth = rectWidth / maximumUnits;
    qreal unitHeight = rectHeight / maximumUnits;

    QRectF unit;

    QVector<QPointF> insideUnits;

    int errorUnitsCount = 0;
    int goodUnitsCount = 0;

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
                insideUnits.push_back(unit.center());
                if (isAnyPointOfAnyLineOfContourInsideOfRectangle(contour, unit))
                {
                    errorUnitsCount++;
                }
                else
                {
                    goodUnitsCount++;
                }
            }
        }
    }

    qreal averageX = 0;
    qreal averageY = 0;
    const int pointsCount = insideUnits.size();
    for (const auto & point : insideUnits)
    {
        averageX += point.x() / pointsCount;
        averageY += point.y() / pointsCount;
    }

    const QPointF gravityCenter(averageX, averageY);
    const double error = goodUnitsCount > 0 ? (double ((double)errorUnitsCount / (double)goodUnitsCount)) : errorUnitsCount;

    return QPair<QPointF, double>(gravityCenter, error);
}

bool RectangleToolKit::doRectanglesTouchEachOther(const QRectF & firstRectangle, const QRectF & secondRectangle)
{
    bool touched = doLeftAndRightOSidesTouchEachOther(firstRectangle, secondRectangle);

    if (touched) {
        return true;
    }

    touched = doLeftAndRightOSidesTouchEachOther(secondRectangle, firstRectangle);

    if (touched) {
        return true;
    }

    touched = doTopAndBottomSidesTouchEachOther(firstRectangle, secondRectangle);

    if (touched) {
        return true;
    }

    return doTopAndBottomSidesTouchEachOther(secondRectangle, firstRectangle);
}

bool RectangleToolKit::doLeftAndRightOSidesTouchEachOther(const QRectF & firstRectangle, const QRectF & secondRectangle)
{

    if (doLeftAndRightSidesTouchEeachOtherRespectively(firstRectangle, secondRectangle)
        || doLeftAndRightSidesTouchEeachOtherRespectively(secondRectangle, firstRectangle))
    {
        return true;
    }

    return false;
}

bool RectangleToolKit::doLeftAndRightSidesTouchEeachOtherRespectively(const QRectF & firstRectangle, const QRectF & secondRectangle)
{
    if (firstRectangle.right() == secondRectangle.left())
    {
        if (firstRectangle.top() >= secondRectangle.top() && firstRectangle.top() <= secondRectangle.bottom())
        {
            return true;
        }

        if (firstRectangle.bottom() <= secondRectangle.bottom() && firstRectangle.bottom() >= secondRectangle.top())
        {
            return true;
        }
    }

    return false;
}

bool RectangleToolKit::doTopAndBottomSidesTouchEachOther(const QRectF & firstRectangle, const QRectF & secondRectangle)
{
    if (doTopAndBottomSidesTouchEeachOtherRespectively(firstRectangle, secondRectangle)
        || doTopAndBottomSidesTouchEeachOtherRespectively(secondRectangle, firstRectangle))
    {
        return true;
    }

    return false;
}

bool RectangleToolKit::doTopAndBottomSidesTouchEeachOtherRespectively(const QRectF & firstRectangle, const QRectF & secondRectangle)
{
    if (firstRectangle.bottom() == secondRectangle.top())
    {
        if (firstRectangle.left() >= secondRectangle.left() && firstRectangle.left() <= secondRectangle.right())
        {
            return true;
        }

        if (firstRectangle.right() <= secondRectangle.right() && firstRectangle.right() >= secondRectangle.left())
        {
            return true;
        }
    }

    return false;
}
