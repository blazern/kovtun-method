#include "Contour.h"

Contour::Contour() :
    points(nullptr),
    size(0)
{
}

Contour::Contour(const Contour & other)
{
    size = other.size;
    if (points == nullptr)
    {
        delete[] points;
    }

    points = new QPointF[other.getPointsCount()];

    for (int index = 0; index < other.getPointsCount(); index++)
    {
        points[index] = other.points[index];
    }
}

void Contour::addPoint(const QPointF & point)
{
    QPointF * temp = new QPointF[size + 1];

    for (int index = 0; index < size; index++)
    {
        temp[index] = points[index];
    }

    temp[size] = point;

    delete[] points;

    points = temp;

    size++;
}

const QPointF & Contour::getPoint(const int index) const
{
    return points[index];
}

int Contour::getPointsCount() const
{
    return size;
}
