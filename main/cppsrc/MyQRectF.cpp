#include "MyQRectF.h"
#include <QDebug>

namespace KovtunMethod
{

MyQRectF::MyQRectF(const MyQRectF & other) :
    QRectF(other),
    name(other.name),
    neighbors(other.neighbors),
    color(other.color),
    colorInitialized(other.colorInitialized),
    parentsGravityCenter(copy(other.parentsGravityCenter)),
    grandParentsGravityCenter(copy(other.grandParentsGravityCenter))
{
}

MyQRectF::~MyQRectF()
{
    for (auto & neighbor : neighbors)
    {
        neighbor->neighbors.remove(this);
    }

    if (parentsGravityCenter != nullptr)
    {
        delete parentsGravityCenter;
    }

    if (grandParentsGravityCenter != nullptr)
    {
        delete grandParentsGravityCenter;
    }
}

MyQRectF::MyQRectF(
        const QPointF & topLeft,
        const QPointF & bottomRight,
        const QString & name,
        const QPointF * parentsGravityCenter,
        const QPointF * grandParentsGravityCenter) :
    QRectF(topLeft, bottomRight),
    name(name),
    neighbors(),
    color(0, 0, 0),
    colorInitialized(false),
    parentsGravityCenter(copy(parentsGravityCenter)),
    grandParentsGravityCenter(copy(grandParentsGravityCenter))
{
}

MyQRectF::MyQRectF(
        const qreal x,
        const qreal y,
        const qreal width,
        const qreal height,
        const QString & name,
        const QPointF * parentsGravityCenter,
        const QPointF * grandParentsGravityCenter) :
    QRectF(x, y, width, height),
    name(name),
    neighbors(),
    color(0, 0, 0),
    colorInitialized(false),
    parentsGravityCenter(copy(parentsGravityCenter)),
    grandParentsGravityCenter(copy(grandParentsGravityCenter))
{
}

QPointF * MyQRectF::copy(const QPointF * point) const
{
    return point == nullptr ? nullptr : new QPointF(*point);
}

void MyQRectF::addNeighbor(MyQRectF & other)
{
    neighbors.insert(&other);
    other.neighbors.insert(this);
}

const QColor MyQRectF::getColor() const
{
    return color;
}

void MyQRectF::setColor(const QColor & color)
{
    colorInitialized = true;
    this->color = color;
}

bool MyQRectF::isColorInitialized() const
{
    return colorInitialized;
}

}

//uint qHash(const KovtunMethod::MyQRectF & rectangle)
//{
// return qHash(rectangle.getName());
//}
