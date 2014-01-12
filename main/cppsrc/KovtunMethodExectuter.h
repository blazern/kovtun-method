#ifndef KOVTUNMETHODEXECTUTER_H
#define KOVTUNMETHODEXECTUTER_H

#include <QVector>
#include <QRectF>
#include "Contour.h"

class KovtunMethodExecuter
{
    KovtunMethodExecuter(const KovtunMethodExecuter & other) = delete;
    KovtunMethodExecuter & operator=(const KovtunMethodExecuter & other) = delete;

public:
    explicit KovtunMethodExecuter(const Contour & contour);

    void performNextStep();

    const Contour & getContour() const;
    const QVector<QRectF> getCurrentActiveRectangles() const;
    const QVector<QRectF> getCurrentFilledRectangles() const;

private:
    const Contour contour;
};

#endif // KOVTUNMETHODEXECTUTER_H
