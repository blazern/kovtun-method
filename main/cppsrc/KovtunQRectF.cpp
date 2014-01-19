#include "KovtunQRectF.h"
#include <QDebug>

KovtunQRectF::KovtunQRectF() :
    QRectF(),
    neighbors(),
    color(0, 0, 0),
    colorInitialized(false)
{
}

KovtunQRectF::KovtunQRectF(const QPointF & topLeft, const QSizeF & size) :
    QRectF(topLeft, size),
    neighbors(),
    color(0, 0, 0),
    colorInitialized(false)
{
}

KovtunQRectF::KovtunQRectF(const QPointF & topLeft, const QPointF & bottomRight) :
    QRectF(topLeft, bottomRight),
    neighbors(),
    color(0, 0, 0),
    colorInitialized(false)
{
}

KovtunQRectF::KovtunQRectF(const qreal x, const qreal y, const qreal width, const qreal height) :
    QRectF(x, y, width, height),
    neighbors(),
    color(0, 0, 0),
    colorInitialized(false)
{
}

KovtunQRectF::KovtunQRectF(const QRect & rectangle) :
    QRectF(rectangle),
    neighbors(),
    color(0, 0, 0),
    colorInitialized(false)
{
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
