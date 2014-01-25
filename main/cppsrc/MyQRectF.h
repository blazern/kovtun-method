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
    explicit MyQRectF(const QPointF & topLeft, const QPointF & bottomRight, const QString & name, const QPointF * parentsGravityCenter, const QPointF * grandParentsGravityCenter);
    explicit MyQRectF(const qreal x, const qreal y, const qreal width, const qreal height, const QString & name, const QPointF * parentsGravityCenter, const QPointF * grandParentsGravityCenter);

    static void makeNeighbors(QSharedPointer<MyQRectF> & firstRectangle, QSharedPointer<MyQRectF> & secondRectangle);

    typedef QSet<QSharedPointer<MyQRectF> >::const_iterator neighborsIterator;

    inline neighborsIterator neighborsBegin() const     { return neighbors.constBegin(); }
    inline neighborsIterator neighborsEnd() const       { return neighbors.constEnd(); }

    inline const QString & getName() const              { return name; }

    inline double getSquare() const                     { return width() * height(); }

    const QColor getColor() const;
    void setColor(const QColor & color);

    bool isColorInitialized() const;

    const QPointF * getParentsGravityCenter() const         { return parentsGravityCenter; }
    const QPointF * getGrandParentsGravityCenter() const    { return grandParentsGravityCenter; }

private:
    const QString name;
    QSet<QSharedPointer<MyQRectF> > neighbors;
    QColor color;
    bool colorInitialized;
    const QPointF * parentsGravityCenter;
    const QPointF * grandParentsGravityCenter;

    // Делает other соседом this, но не делает this соседом other!
    // Т.е. вызов данного метода не делает отношение симметричным.
    // Если Вы хотите задать симметричное отношение, то желательно использовать makeNeighbors()
    void addNeighbor(const QSharedPointer<MyQRectF> & other);

    QPointF * copy(const QPointF * point) const;
};

}

#endif // KOVTUNQRECTF_H
