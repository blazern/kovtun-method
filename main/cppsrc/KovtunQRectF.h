#ifndef KOVTUNQRECTF_H
#define KOVTUNQRECTF_H

#include <QRectF>
#include <QSet>
#include <QColor>
#include <QSharedPointer>

class KovtunQRectF : public QRectF
{
public:
    explicit KovtunQRectF(const QString & name, const QPointF * parentsGravityCenter);
    explicit KovtunQRectF(const QPointF & topLeft, const QSizeF & size, const QString & name, const QPointF * parentsGravityCenter);
    explicit KovtunQRectF(const QPointF & topLeft, const QPointF & bottomRight, const QString & name, const QPointF * parentsGravityCenter);
    explicit KovtunQRectF(const qreal x, const qreal y, const qreal width, const qreal height, const QString & name, const QPointF * parentsGravityCenter);
    explicit KovtunQRectF(const QRect & rectangle, const QString & name, const QPointF * parentsGravityCenter);

    static void makeNeighbors(QSharedPointer<KovtunQRectF> & firstRectangle, QSharedPointer<KovtunQRectF> & secondRectangle);

    typedef QSet<QSharedPointer<KovtunQRectF> >::const_iterator neighborsIterator;

    inline neighborsIterator neighborsBegin() const     { return neighbors.constBegin(); }
    inline neighborsIterator neighborsEnd() const       { return neighbors.constEnd(); }

    inline const QString & getName() const              { return name; }

    inline double getSquare() const                     { return width() * height(); }

    const QColor getColor() const;
    void setColor(const QColor & color);

    bool isColorInitialized() const;

    const QPointF * getParentsGravityCenter() const      { return parentsGravityCenter; }

private:
    const QString name;
    QSet<QSharedPointer<KovtunQRectF> > neighbors;
    QColor color;
    bool colorInitialized;
    const QPointF * parentsGravityCenter;

    // Делает other соседом this, но не делает this соседом other!
    // Т.е. вызов данного метода не делает отношение симметричным.
    // Если Вы хотите задать симметричное отношение, то желательно использовать makeNeighbors()
    void addNeighbor(const QSharedPointer<KovtunQRectF> & other);

    QPointF * copy(const QPointF * point) const;
};

#endif // KOVTUNQRECTF_H
