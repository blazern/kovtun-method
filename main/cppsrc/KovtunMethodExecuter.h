#ifndef KOVTUNMETHODEXECTUTER_H
#define KOVTUNMETHODEXECTUTER_H

#include "Contour.h"
#include <QObject>
#include <QVector>
#include <QRectF>
#include <QMutex>

class KovtunMethodExecuter
{
    KovtunMethodExecuter(const KovtunMethodExecuter & other) = delete;
    KovtunMethodExecuter & operator=(const KovtunMethodExecuter & other) = delete;

public:
    explicit KovtunMethodExecuter(const ClosedContour & contour);

    void performNextStep();
    void reset();

    void setUnitsDimension(const int dimension)         { unitDimension = dimension; }
    int getUnitsDimension() const                       { return unitDimension; }

    inline const ClosedContour & getContour() const           { return contour; }

    inline const QRectF * getActiveRectangles() const   { return activeRectangles.data(); }
    inline int getActiveRectanglesCount() const         { return activeRectangles.size(); }

    inline const QRectF * getFilledRectangles() const   { return filledRectangles.data(); }
    inline int getFilledRectanglesCount() const         { return filledRectangles.size(); }


    // Если объект класса будет использоваться в многопоточной среде, то рекомендованно вызывать lock()
    // при начале обработки данных объекта, и unlock() при окончании.
    // Несоблюдение этого правила в многопоточной среде ведёт к неопределённому поведению.
    inline void lock() const    { mutex.lock(); }
    void unlock() const         { mutex.unlock(); }

private:
    const ClosedContour contour;
    QVector<QRectF> activeRectangles;
    QVector<QRectF> filledRectangles;
    int unitDimension;

    mutable QMutex mutex;

    void calculateFirstActiveRectangle();
};

#endif // KOVTUNMETHODEXECTUTER_H
