#include "RectangleToolKit.h"
#include <QDebug>

bool RectangleToolKit::isAnyPointOfRectangleInsideOfContour(const Contour & contour, const QRectF & rectangle)
{
    if (contour.containsInside(rectangle.topLeft()) || contour.containsInside(rectangle.topRight())
        || contour.containsInside(rectangle.bottomLeft()) || contour.containsInside(rectangle.bottomRight()))
    {
        return true;
    }

    return isAnyPointOfAnyLineOfContourInsideOfRectangle(contour, rectangle);
}

bool RectangleToolKit::isAnyPointOfAnyLineOfContourInsideOfRectangle(const Contour & contour, const QRectF & rectangle)
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

QPointF RectangleToolKit::calculateGravityCenter(const Contour & contour, const QRectF & rectangle)
{
    QVector<QPointF> innerPoints;

    const QPointF * const points = contour.getPoints();
    for (int index = 0; index < contour.getPointsCount(); index++)
    {
        const QPointF & currentPoint = points[index];
        if (rectangleContainsInside(rectangle, currentPoint))
        {
            innerPoints.push_back(currentPoint);
        }
        else
        {
            if (rectangle.contains(currentPoint))
            {
                if (index - 1 >= 0 && rectangleContainsInside(rectangle, points[index - 1]))
                {
                    innerPoints.push_back(currentPoint);
                }
                else if (index + 1 < contour.getPointsCount() && rectangleContainsInside(rectangle, points[index + 1]))
                {
                    innerPoints.push_back(currentPoint);
                }
            }
        }
    }

    const QLineF northLine(rectangle.topLeft(), rectangle.topRight());
    const QLineF eastLine(rectangle.topRight(), rectangle.bottomRight());
    const QLineF southLine(rectangle.bottomRight(), rectangle.bottomLeft());
    const QLineF westLine(rectangle.bottomLeft(), rectangle.topLeft());

    const QLineF * const lines = contour.getLines();
    for (int index = 0; index < contour.getLinesCount(); index++)
    {
        const QLineF & currentLine = lines[index];
        addIntersectionIfExistsToVector(currentLine, northLine, innerPoints);
        addIntersectionIfExistsToVector(currentLine, eastLine, innerPoints);
        addIntersectionIfExistsToVector(currentLine, southLine, innerPoints);
        addIntersectionIfExistsToVector(currentLine, westLine, innerPoints);
    }

    if (!innerPoints.isEmpty())
    {
        qreal sumX = 0;
        qreal sumY = 0;

        for (const auto & innerPoint : innerPoints)
        {
            sumX += innerPoint.x();
            sumY += innerPoint.y();
        }

        return QPointF(sumX / innerPoints.size(), sumY / innerPoints.size());;
    }
    else
    {
        return QPointF();
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

void RectangleToolKit::addIntersectionIfExistsToVector(const QLineF & firstLine, const QLineF & secondLine, QVector<QPointF> & vector)
{
    if (doLinesIntersectWithoutLyingOnEachOther(firstLine, secondLine))
    {
        QPointF intersection;
        firstLine.intersect(secondLine, &intersection);
        vector.push_back(intersection);
    }
}
