#include "RectangleToolKit.h"

#include <QDebug>
#include <QPoint>
#include "NotSingleShape.h"

namespace KovtunMethod
{

bool RectangleToolKit::isAnyPointOfRectangleInsideOfContour(const ClosedContour & contour, const QRectF & rectangle)
{
    if (contour.containsInside(rectangle.topLeft())
        || contour.containsInside(rectangle.topRight())
        || contour.containsInside(rectangle.bottomLeft())
        || contour.containsInside(rectangle.bottomRight()))
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

bool RectangleToolKit::isRectangleInsideOfContour(const ClosedContour & contour, const QRectF & rectangle)
{
    if (!RectangleToolKit::isAnyPointOfAnyLineOfContourInsideOfRectangle(contour, rectangle))
    {
        if (RectangleToolKit::isAnyPointOfRectangleInsideOfContour(contour, rectangle))
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

    const QPointF lineCenterQPointF((line.x1() + line.x2()) / 2, (line.y1() + line.y2()) / 2);

    if (doLinesIntersectWithoutLyingOnEachOther(line, northLine)
        || doLinesIntersectWithoutLyingOnEachOther(line, eastLine)
        || doLinesIntersectWithoutLyingOnEachOther(line, southLine)
        || doLinesIntersectWithoutLyingOnEachOther(line, westLine)
        || rectangle.contains(lineCenterQPointF))
    {
        return true;
    }
    else
    {
        return false;
    }
}

bool RectangleToolKit::isLineADiagonalOfRectangle(const QLineF & line, const QRectF & rectangle)
{
    return ((line.p1() == rectangle.topLeft() && line.p2() == rectangle.bottomRight())
            || (line.p1() == rectangle.bottomRight() && line.p2() == rectangle.topLeft())
            || (line.p1() == rectangle.topRight() && line.p2() == rectangle.bottomLeft())
            || (line.p1() == rectangle.bottomLeft() && line.p2() == rectangle.topRight()));
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

}
