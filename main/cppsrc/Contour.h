#ifndef CONTOUR_H
#define CONTOUR_H

#include <QPointF>

class Contour
{
public:
    explicit Contour();
    Contour(const Contour & other);
    Contour & operator=(const Contour & other) = default;

    const QPointF & getPoint(const int index) const;
    int getPointsCount() const;

    void addPoint(const QPointF & point);

private:
    // Первоначально планировалось использовать класс-контейнер, но приложение некорректно работало из-за контейнеров,
    // ошибка так и не было обнаружена, и было решени использовать массив, потому что он приложение не крашил.
    QPointF * points;
    int size;
};

#endif // CONTOUR_H
