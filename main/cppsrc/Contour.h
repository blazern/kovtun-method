#ifndef CONTOUR_H
#define CONTOUR_H

#include <QPointF>
#include <QVector>
#include <QLineF>

class Contour
{
public:
    explicit Contour();
    Contour(const Contour &) = default;
    Contour & operator=(const Contour &) = default;

    const QLineF * getLines() const;
    int getLinesCount() const;

    const QPointF & getPoint(const int index) const;
    int getPointsCount() const;

    void addPoint(const QPointF & point);

private:
    QVector<QPointF> points;
    QVector<QLineF> lines;
};

#endif // CONTOUR_H
