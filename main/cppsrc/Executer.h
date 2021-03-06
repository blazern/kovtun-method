#ifndef KOVTUNMETHODEXECUTER_H
#define KOVTUNMETHODEXECUTER_H

#include "ClosedContour.h"
#include "MyQRectF.h"
#include "ColorsDictionary.h"
#include "ExecuterListener.h"
#include <QObject>
#include <QVector>
#include <QRectF>
#include <QSharedPointer>
#include <QSet>
#include <QPair>

namespace KovtunMethod
{

class Executer
{
    Executer(const Executer &) = delete;
    Executer & operator=(const Executer &) = delete;

public:
    explicit Executer(const ClosedContour & contour);

    void performNextStep();
    void reset();

    void setUnitsDimension(const int dimension) { unitDimension = dimension; }
    int getUnitsDimension() const { return unitDimension; }

    inline const ClosedContour & getContour() const { return contour; }

    const MyQRectF & getActiveRectangle(const int index) const;
    int getActiveRectanglesCount() const;
    int getCurrentActiveRectanglesCount() const;

    inline const MyQRectF & getFilledRectangle(const int index) const { return *filledRectangles[index]; }
    inline int getFilledRectanglesCount() const { return filledRectangles.size(); }

    double getCurrentError() const;

    void setNeighborMethodExecution();
    void setFractalMethodEXecution();

    bool isNeighborMethodSet() const { return neighborMethodIsSet; }

    inline void addListener(ExecuterListener & listener) { listeners.insert(&listener); }
    void removeListener(ExecuterListener & listener);

    static const int DEFAULT_UNITS_DIMENSION = 50;

private:
    bool executionStarted;
    const ClosedContour contour;
    QVector<QSharedPointer<MyQRectF> > currentActiveRectangles;
    QVector<QSharedPointer<MyQRectF> > filledRectangles;
    QVector<QSharedPointer<MyQRectF> > aboutToGetFilledRectangles;
    int unitDimension;
    ColorsDictionary colorDictionary;
    QVector<double> errors;
    double firstRectangleArea;
    bool neighborMethodIsSet;

    QSet<ExecuterListener *> listeners;

    void calculateFirstActiveRectangle();
    void calculateNewActiveRectangles();
    QSharedPointer<MyQRectF> createTopLeftRectangleFrom(const MyQRectF & parent, const QPointF & parentGravityCenter) const;
    QSharedPointer<MyQRectF> createTopRightRectangleFrom(const MyQRectF & parent, const QPointF & parentGravityCenter) const;
    QSharedPointer<MyQRectF> createBottomRightRectangleFrom(const MyQRectF & parent, const QPointF & parentGravityCenter) const;
    QSharedPointer<MyQRectF> createBottomLeftRectangleFrom(const MyQRectF & parent, const QPointF & parentGravityCenter) const;
    void leaveOnlyAtLeastPartlyInsideOfContourRectangles(QVector<QSharedPointer<MyQRectF> > & rectangles, const ClosedContour & contour) const;
    void shareNeighbors(QSharedPointer<MyQRectF> & source, QVector<QSharedPointer<MyQRectF> > & destination) const;
    void shareNeighbors(QSharedPointer<MyQRectF> & source, QSharedPointer<MyQRectF> & destination) const;
    void makeNeighbors(QVector<QSharedPointer<MyQRectF> > & futureNeighbors) const;
    void moveAndFillRectanglesWhichShouldBeFilled(
            QVector<QSharedPointer<MyQRectF> > & potentialRectangles,
            QVector<QSharedPointer<MyQRectF> > & certainRectangles);
    QPair<bool, QColor> findAndFillKeyRectangle(
            QVector<QSharedPointer<MyQRectF> > & potentialRectangles,
            QVector<QSharedPointer<MyQRectF> > & certainRectangles);
    void fillInternalRectangles(QVector<QSharedPointer<MyQRectF> > & potentialRectangles, QVector<QSharedPointer<MyQRectF> > & filledRectangles, const QColor & color);
};

}

#endif // KOVTUNMETHODEXECUTER_H
