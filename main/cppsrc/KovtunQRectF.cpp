#include "KovtunQRectF.h"
#include <QDebug>

KovtunQRectF::KovtunQRectF(const QString name) :
    QRectF(),
    name(name),
    neighbors(),
    color(0, 0, 0),
    colorInitialized(false)
{
}

KovtunQRectF::KovtunQRectF(const QPointF & topLeft, const QSizeF & size, const QString name) :
    QRectF(topLeft, size),
    name(name),
    neighbors(),
    color(0, 0, 0),
    colorInitialized(false)
{
}

KovtunQRectF::KovtunQRectF(const QPointF & topLeft, const QPointF & bottomRight, const QString name) :
    QRectF(topLeft, bottomRight),
    name(name),
    neighbors(),
    color(0, 0, 0),
    colorInitialized(false)
{
}

KovtunQRectF::KovtunQRectF(const qreal x, const qreal y, const qreal width, const qreal height, const QString name) :
    QRectF(x, y, width, height),
    name(name),
    neighbors(),
    color(0, 0, 0),
    colorInitialized(false)
{
}

KovtunQRectF::KovtunQRectF(const QRect & rectangle, const QString name) :
    QRectF(rectangle),
    name(name),
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
