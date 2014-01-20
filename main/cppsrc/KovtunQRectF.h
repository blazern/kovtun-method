#ifndef KOVTUNQRECTF_H
#define KOVTUNQRECTF_H

#include <QRectF>
#include <QSet>
#include <QColor>
#include <QSharedPointer>

class KovtunQRectF : public QRectF
{
public:
    explicit KovtunQRectF();
    explicit KovtunQRectF(const QPointF & topLeft, const QSizeF & size);
    explicit KovtunQRectF(const QPointF & topLeft, const QPointF & bottomRight);
    explicit KovtunQRectF(const qreal x, const qreal y, const qreal width, const qreal height);
    explicit KovtunQRectF(const QRect & rectangle);

    typedef QSet<QSharedPointer<KovtunQRectF> >::const_iterator neighborsIterator;

    neighborsIterator neighborsBegin() const    { return neighbors.constBegin(); }
    neighborsIterator neighborsEnd() const      { return neighbors.constEnd(); }

    static void makeNeighbors(QSharedPointer<KovtunQRectF> & firstRectangle, QSharedPointer<KovtunQRectF> & secondRectangle);

    const QColor getColor() const;
    void setColor(const QColor & color);

    bool isColorInitialized() const;

private:
    QSet<QSharedPointer<KovtunQRectF> > neighbors;
    QColor color;
    bool colorInitialized;

    // Делает other соседом this, но не делает this соседом other!
    // Т.е. вызов данного метода не делает отношение симметричным.
    // Если Вы хотите задать симметричное отношение, то желательно использовать makeNeighbors()
    void addNeighbor(const QSharedPointer<KovtunQRectF> & other);
};

#endif // KOVTUNQRECTF_H
