#include "KovtunQRectF.h"
#include <QDebug>

KovtunQRectF::KovtunQRectF(const QPointF & topLeft, const QPointF & bottomRight, const QString & name, const QPointF * parentsGravityCenter, const QPointF * grandParentsGravityCenter) :
    QRectF(topLeft, bottomRight),
    name(name),
    neighbors(),
    color(0, 0, 0),
    colorInitialized(false),
    parentsGravityCenter(copy(parentsGravityCenter)),
    grandParentsGravityCenter(copy(grandParentsGravityCenter))
{
}

KovtunQRectF::KovtunQRectF(const qreal x, const qreal y, const qreal width, const qreal height, const QString & name, const QPointF * parentsGravityCenter, const QPointF * grandParentsGravityCenter) :
    QRectF(x, y, width, height),
    name(name),
    neighbors(),
    color(0, 0, 0),
    colorInitialized(false),
    parentsGravityCenter(copy(parentsGravityCenter)),
    grandParentsGravityCenter(copy(grandParentsGravityCenter))
{
}

QPointF * KovtunQRectF::copy(const QPointF * point) const
{
    return point == nullptr ? nullptr : new QPointF(*point);
}

void KovtunQRectF::makeNeighbors(QSharedPointer<KovtunQRectF> & firstRectangle, QSharedPointer<KovtunQRectF> & secondRectangle)
{
    firstRectangle->addNeighbor(secondRectangle);
    secondRectangle->addNeighbor(firstRectangle);
}

void KovtunQRectF::addNeighbor(const QSharedPointer<KovtunQRectF> & other)
{
    neighbors.insert(other);
}

const QColor KovtunQRectF::getColor() const
{
    return color;
}

void KovtunQRectF::setColor(const QColor & color)
{
    colorInitialized = true;
    this->color = color;
}

bool KovtunQRectF::isColorInitialized() const
{
    return colorInitialized;
}
