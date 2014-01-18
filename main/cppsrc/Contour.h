#ifndef CONTOUR_H
#define CONTOUR_H

#include <QPointF>
#include <QVector>
#include <QLineF>

// Класс является представлением закрытого контура с отсутствием самопересечений.
//
// Наличие самопересечений никак не проверяется, поэтому если пользователь класса их создаст,
// то поведение некоторых методов (например containsInside()) может быть некорректно.
//
// Ручное замыкание контура не требуется, т.е. первая и последняя точка контура соединяются автоматически,
// и вызов getLines()[getLinesCount() - 1] вернёт линию, соединяющую их.
class ClosedContour
{
public:
    explicit ClosedContour();
    ClosedContour(const ClosedContour &) = default;
    ClosedContour & operator=(const ClosedContour &) = default;

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

    inline qreal getWidth() const   { return north - south; }
    inline qreal getHeight() const  { return east - west; }

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
