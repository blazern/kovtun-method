#include "KovtunMethodExecuter.h"
#include "RectangleToolKit.h"

#ifdef QT_DEBUG
#include <QDebug>
#endif

KovtunMethodExecuter::KovtunMethodExecuter(const ClosedContour & contour) :
    contour(contour),
    activeRectangles(),
    filledRectangles(),
    unitDimension(20)
{
}

void KovtunMethodExecuter::performNextStep()
{
    if (activeRectangles.size() == 0)
    {
        calculateFirstActiveRectangle();
    }
    else
    {
        QVector<QRectF> newActiveRectangles;

        for (QVector<QRectF>::iterator iterator = activeRectangles.begin(); iterator != activeRectangles.end(); iterator = activeRectangles.erase(iterator))
        {
            const QRectF & activeRectangle = *iterator;

            if (!RectangleToolKit::isAnyPointOfAnyLineOfContourInsideOfRectangle(contour, activeRectangle))
            {
                if (RectangleToolKit::isAnyPointOfRectangleInsideOfContour(contour, activeRectangle))
                {
                    filledRectangles.push_back(*iterator);
                }
                continue;
            }

            const QPointF gravityCenter = RectangleToolKit::calculateGravityCenter(contour, activeRectangle, unitDimension);

            newActiveRectangles.push_back(QRectF(
                                              activeRectangle.left(),
                                              activeRectangle.top(),
                                              gravityCenter.x() - activeRectangle.left(),
                                              gravityCenter.y() - activeRectangle.top()));

            newActiveRectangles.push_back(QRectF(
                                              gravityCenter.x(),
                                              activeRectangle.top(),
                                              activeRectangle.right() - gravityCenter.x(),
                                              gravityCenter.y() - activeRectangle.top()));

            newActiveRectangles.push_back(QRectF(
                                              gravityCenter.x(),
                                              gravityCenter.y(),
                                              activeRectangle.right() - gravityCenter.x(),
                                              activeRectangle.bottom() - gravityCenter.y()));

            newActiveRectangles.push_back(QRectF(
                                              activeRectangle.left(),
                                              gravityCenter.y(),
                                              gravityCenter.x() - activeRectangle.left(),
                                              activeRectangle.bottom() - gravityCenter.y()));
        }

        activeRectangles << newActiveRectangles;
    }
}

void KovtunMethodExecuter::reset()
{
    activeRectangles.clear();
    filledRectangles.clear();
}

void KovtunMethodExecuter::calculateFirstActiveRectangle()
{
    if (contour.getPointsCount() > 0)
    {
        activeRectangles.push_back(QRectF(QPointF(contour.getWest(), contour.getSouth()),
                                          QPointF(contour.getEast(), contour.getNorth())));
    }
#ifdef QT_DEBUG
    else
    {
        qDebug() << "По какой-то причине контур пуст!";
    }
#endif
}
