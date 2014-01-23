#ifndef KOVTUNMETHODEXECTUTER_H
#define KOVTUNMETHODEXECTUTER_H

#include "ClosedContour.h"
#include "KovtunQRectF.h"
#include "ColorsDictionary.h"
#include "KovtunMethodExecuterListener.h"
#include <QObject>
#include <QVector>
#include <QRectF>
#include <QSharedPointer>
#include <QSet>

class KovtunMethodExecuter
{
    KovtunMethodExecuter(const KovtunMethodExecuter &) = delete;
    KovtunMethodExecuter & operator=(const KovtunMethodExecuter &) = delete;

public:
    explicit KovtunMethodExecuter(const ClosedContour & contour);

    void performNextStep();
    void reset();

    void setUnitsDimension(const int dimension)                                     { unitDimension = dimension; }
    int getUnitsDimension() const                                                   { return unitDimension; }

    inline const ClosedContour & getContour() const                                 { return contour; }

    inline const KovtunQRectF & getActiveRectangle(const int index) const           { return *activeRectangles[index]; }
    inline int getActiveRectanglesCount() const                                     { return activeRectangles.size(); }

    inline const KovtunQRectF & getFilledRectangle(const int index) const           { return *filledRectangles[index]; }
    inline int getFilledRectanglesCount() const                                     { return filledRectangles.size(); }

    double getCurrentError() const;

    inline void addListener(KovtunMethodExecuterListener & listener)                { listeners.insert(&listener); }
    void removeListener(KovtunMethodExecuterListener & listener);

    static const int defaultUnitDimension = 20;

private:
    const ClosedContour contour;
    QVector<QSharedPointer<KovtunQRectF> > activeRectangles;
    QVector<QSharedPointer<KovtunQRectF> > filledRectangles;
    int unitDimension;
    ColorsDictionary colorDictionary;
    QVector<double> errors;
    double firstRectangleArea;

    QSet<KovtunMethodExecuterListener *> listeners;

    void calculateFirstActiveRectangle();
    void calculateNewActiveRectangles();
    void shareNeighbors(QSharedPointer<KovtunQRectF> & source, QSharedPointer<KovtunQRectF> & destination) const;
    bool tryToFill(QSharedPointer<KovtunQRectF> & rectangle, const QPointF * gravityCenterOfGrandParent);
};

#endif // KOVTUNMETHODEXECTUTER_H
