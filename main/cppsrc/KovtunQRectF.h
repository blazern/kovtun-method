#ifndef KOVTUNQRECTF_H
#define KOVTUNQRECTF_H

#include <QRectF>
#include <QSet>
#include <QColor>
#include <QSharedPointer>

class KovtunQRectF : public QRectF
{
public:
    explicit KovtunQRectF(const QString name);
    explicit KovtunQRectF(const QPointF & topLeft, const QSizeF & size, const QString name);
    explicit KovtunQRectF(const QPointF & topLeft, const QPointF & bottomRight, const QString name);
    explicit KovtunQRectF(const qreal x, const qreal y, const qreal width, const qreal height, const QString name);
    explicit KovtunQRectF(const QRect & rectangle, const QString name);

    static void makeNeighbors(QSharedPointer<KovtunQRectF> & firstRectangle, QSharedPointer<KovtunQRectF> & secondRectangle);

    typedef QSet<QSharedPointer<KovtunQRectF> >::const_iterator neighborsIterator;

    neighborsIterator neighborsBegin() const    { return neighbors.constBegin(); }
    neighborsIterator neighborsEnd() const      { return neighbors.constEnd(); }

    const QString & getName() const             { return name; }

    const QColor getColor() const;
    void setColor(const QColor & color);

    bool isColorInitialized() const;

private:
    const QString name;
    QSet<QSharedPointer<KovtunQRectF> > neighbors;
    QColor color;
    bool colorInitialized;

    // Делает other соседом this, но не делает this соседом other!
    // Т.е. вызов данного метода не делает отношение симметричным.
    // Если Вы хотите задать симметричное отношение, то желательно использовать makeNeighbors()
    void addNeighbor(const QSharedPointer<KovtunQRectF> & other);
};

#endif // KOVTUNQRECTF_H
