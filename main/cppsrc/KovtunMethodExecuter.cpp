#include "KovtunMethodExecuter.h"

#ifdef QT_DEBUG
#include <QDebug>
#endif

KovtunMethodExecuter::KovtunMethodExecuter(const Contour & contour) :
    contour(contour),
    activeRectangles(),
    filledRectangles()
{
}

void KovtunMethodExecuter::performNextStep()
{
    if (activeRectangles.size() == 0)
    {
        calculateFirstActiveRectangle();
    }
}

void KovtunMethodExecuter::calculateFirstActiveRectangle()
{
    if (contour.getPointsCount() > 0)
    {
        const QPointF * mostNorthPoint = &contour.getPoint(0);
        const QPointF * mostEastPoint = &contour.getPoint(0);
        const QPointF * mostSouthPoint = &contour.getPoint(0);
        const QPointF * mostWestPoint = &contour.getPoint(0);

        for (int index = 1; index < contour.getPointsCount(); index++)
        {
            const QPointF * currentPoint = &contour.getPoint(index);

            if (currentPoint->y() > mostNorthPoint->y())
            {
                mostNorthPoint = currentPoint;
            }

            if (currentPoint->x() > mostEastPoint->x())
            {
                mostEastPoint = currentPoint;
            }

            if (currentPoint->y() < mostSouthPoint->y())
            {
                mostSouthPoint = currentPoint;
            }

            if (currentPoint->x() < mostSouthPoint->x())
            {
                mostWestPoint = currentPoint;
            }
        }

        activeRectangles.push_back(QRectF(QPointF(mostWestPoint->x(), mostNorthPoint->y()),
                                          QPointF(mostEastPoint->x(), mostSouthPoint->y())));
    }
#ifdef QT_DEBUG
    else
    {
        qDebug() << "По какой-то причине контур пуст!";
    }
#endif
}
