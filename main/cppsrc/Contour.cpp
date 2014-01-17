#include "Contour.h"

Contour::Contour() :
    points(),
    north(0),
    east(0),
    south(0),
    west(0)
{
}

const QLineF * Contour::getLines() const
{
    return lines.data();
}

int Contour::getLinesCount() const
{
    return lines.size();
}

void Contour::addPoint(const qreal x, const qreal y)
{
    if (points.size() >= 1)
    {
        if (lines.size() >= 1)
        {
            lines.removeLast();
        }

        const QPointF & lastPoint = points[points.size() - 1];
        lines.push_back(QLineF(lastPoint.x(), lastPoint.y(), x, y));
        lines.push_back(QLineF(x, y, points[0].x(), points[0].y()));
    }

    points.push_back(QPointF(x, y));

    recalculateSides();
}

void Contour::addPoint(const QPointF & point)
{
    addPoint(point.x(), point.y());
}

void Contour::recalculateSides()
{
    if (getPointsCount() > 0)
    {
        const QPointF * mostNorthPoint = &getPoint(0);
        const QPointF * mostEastPoint = &getPoint(0);
        const QPointF * mostSouthPoint = &getPoint(0);
        const QPointF * mostWestPoint = &getPoint(0);

        for (int index = 1; index < getPointsCount(); index++)
        {
            const QPointF * currentPoint = &getPoint(index);

            if (currentPoint->y() > mostNorthPoint->y())
            {
                mostNorthPoint = currentPoint;
            }

            if (currentPoint->x() > mostEastPoint->x())
            {
                mostEastPoint = currentPoint;
            }

            if (currentPoint->y() < mostSouthPoint->y())
            {
                mostSouthPoint = currentPoint;
            }

            if (currentPoint->x() < mostSouthPoint->x())
            {
                mostWestPoint = currentPoint;
            }
        }

        north = mostNorthPoint->y();
        east = mostEastPoint->x();
        south = mostSouthPoint->y();
        west = mostWestPoint->x();
    }
}

const QPointF & Contour::getPoint(const int index) const
{
    return points[index];
}

const QPointF * Contour::getPoints() const
{
    return points.data();
}

int Contour::getPointsCount() const
{
    return points.size();
}

// http://ru.wikipedia.org/wiki/%D0%97%D0%B0%D0%B4%D0%B0%D1%87%D0%B0_%D0%BE_%D0%BF%D1%80%D0%B8%D0%BD%D0%B0%D0%B4%D0%BB%D0%B5%D0%B6%D0%BD%D0%BE%D1%81%D1%82%D0%B8_%D1%82%D0%BE%D1%87%D0%BA%D0%B8_%D0%BC%D0%BD%D0%BE%D0%B3%D0%BE%D1%83%D0%B3%D0%BE%D0%BB%D1%8C%D0%BD%D0%B8%D0%BA%D1%83
bool Contour::containsInside(const QPointF & point) const
{
    const QLineF lineToEast(point.x(), point.y(), east, point.y());

    int intersectionsCount = 0;

    const QLineF * const lines = getLines();
    for (int index = 0; index < getLinesCount(); index++)
    {
        const QLineF & currentLine = lines[index];
        QPointF intersectionPoint;
        const QLineF::IntersectType intersectType = currentLine.intersect(lineToEast, &intersectionPoint);

        if (intersectType == QLineF::BoundedIntersection)
        {
            const bool pointIsOnBound = intersectionPoint == point;
            if (!pointIsOnBound)
            {
                intersectionsCount++;
            }
            else
            {
                return false;
            }
        }
    }

    return (intersectionsCount % 2) == 1;
}
