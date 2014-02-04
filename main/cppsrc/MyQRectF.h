#ifndef KOVTUNQRECTF_H
#define KOVTUNQRECTF_H

#include <QRectF>
#include <QSet>
#include <QColor>
#include <QSharedPointer>

namespace KovtunMethod
{

class MyQRectF : public QRectF
{
public:
    MyQRectF(const MyQRectF & other);
    MyQRectF & operator=(const MyQRectF &) = delete;
    ~MyQRectF();
    explicit MyQRectF(
            const QPointF & topLeft,
            const QPointF & bottomRight,
            const QString & name,
            const QPointF * parentsGravityCenter,
            const QPointF * grandParentsGravityCenter);
    explicit MyQRectF(
            const qreal x,
            const qreal y,
            const qreal width,
            const qreal height,
            const QString & name,
            const QPointF * parentsGravityCenter,
            const QPointF * grandParentsGravityCenter);

    typedef QSet<MyQRectF*> Neighborhood;

    void addNeighbor(MyQRectF & other);
    inline const Neighborhood & getNeighborhood() const { return neighbors; }

    inline const QString & getName() const { return name; }

    const QColor getColor() const;
    void setColor(const QColor & color);
    bool isColorInitialized() const;

    const QPointF * getParentsGravityCenter() const { return parentsGravityCenter; }
    const QPointF * getGrandParentsGravityCenter() const { return grandParentsGravityCenter; }

    inline double getArea() const { return width() * height(); }

private:
    const QString name;
    Neighborhood neighbors;
    QColor color;
    bool colorInitialized;
    const QPointF * const parentsGravityCenter;
    const QPointF * const grandParentsGravityCenter;

    QPointF * copy(const QPointF * point) const;
};

}

//uint qHash(const KovtunMethod::MyQRectF & rectangle);

#endif // KOVTUNQRECTF_H
