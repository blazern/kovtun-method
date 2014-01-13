#include "Contour.h"

Contour::Contour() :
    points()
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

void Contour::addPoint(const QPointF & point)
{
    if (points.size() >= 1)
    {
        if (lines.size() >= 1)
        {
            lines.removeLast();
        }

        const QPointF & lastPoint = points[points.size() - 1];
        lines.push_back(QLineF(lastPoint, point));
        lines.push_back(QLineF(point, points[0]));
    }

    points.push_back(point);
}

const QPointF & Contour::getPoint(const int index) const
{
    return points[index];
}

int Contour::getPointsCount() const
{
    return points.size();
}
