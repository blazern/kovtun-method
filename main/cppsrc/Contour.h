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
    const QPointF * getPoints() const;
    int getPointsCount() const;

    // Содержит точку строго внутри, не на границе
    bool containsInside(const QPointF & point) const;

    inline qreal getNorth() const   { return north; }
    inline qreal getEast() const    { return east; }
    inline qreal getSouth() const   { return south; }
    inline qreal getWest() const    { return west; }

    void addPoint(const qreal x, const qreal y);
    void addPoint(const QPointF & point);

private:
    QVector<QPointF> points;
    QVector<QLineF> lines;

    qreal north;
    qreal east;
    qreal south;
    qreal west;

    void recalculateSides();
};

#endif // CONTOUR_H
