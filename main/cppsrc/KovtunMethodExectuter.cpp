#include "KovtunMethodExectuter.h"

KovtunMethodExecuter::KovtunMethodExecuter(const Contour & contour) :
    contour(contour)
{
}

const Contour & KovtunMethodExecuter::getContour() const
{
    return contour;
}

const QVector<QRectF> KovtunMethodExecuter::getCurrentActiveRectangles() const
{
    QVector<QRectF> rectangles;

    rectangles.push_back(QRectF(10, 10, 100, 100));
    return rectangles;
}

const QVector<QRectF> KovtunMethodExecuter::getCurrentFilledRectangles() const
{
    return QVector<QRectF>();
}
