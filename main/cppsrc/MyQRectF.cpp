#include "MyQRectF.h"
#include <QDebug>

namespace KovtunMethod
{

MyQRectF::MyQRectF(const QPointF & topLeft, const QPointF & bottomRight, const QString & name, const QPointF * parentsGravityCenter, const QPointF * grandParentsGravityCenter) :
    QRectF(topLeft, bottomRight),
    name(name),
    neighbors(),
    color(0, 0, 0),
    colorInitialized(false),
    parentsGravityCenter(copy(parentsGravityCenter)),
    grandParentsGravityCenter(copy(grandParentsGravityCenter))
{
}

MyQRectF::MyQRectF(const qreal x, const qreal y, const qreal width, const qreal height, const QString & name, const QPointF * parentsGravityCenter, const QPointF * grandParentsGravityCenter) :
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

void MyQRectF::makeNeighbors(QSharedPointer<MyQRectF> & firstRectangle, QSharedPointer<MyQRectF> & secondRectangle)
{
    firstRectangle->addNeighbor(secondRectangle);
    secondRectangle->addNeighbor(firstRectangle);
}

void MyQRectF::addNeighbor(const QSharedPointer<MyQRectF> & other)
{
    neighbors.insert(other);
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
