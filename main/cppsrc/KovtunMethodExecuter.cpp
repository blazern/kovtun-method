#include "KovtunMethodExecuter.h"
#include "RectangleToolKit.h"

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
    else
    {
        QVector<QRectF> newActiveRectangles;

        for (QVector<QRectF>::iterator iterator = activeRectangles.begin(); iterator != activeRectangles.end(); )
        {
            const QRectF & activeRectangle = *iterator;

            if (!RectangleToolKit::isAnyPointOfRectangleInsideOfContour(contour, activeRectangle))
            {
                filledRectangles.push_back(*iterator);
                iterator = activeRectangles.erase(iterator);
                continue;
            }

            const QPointF gravityCenter = RectangleToolKit::calculateGravityCenter(contour, activeRectangle);

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

            iterator++;
        }

        activeRectangles << newActiveRectangles;
    }
}

void KovtunMethodExecuter::calculateFirstActiveRectangle()
{
    if (contour.getPointsCount() > 0)
    {
        activeRectangles.push_back(QRectF(QPointF(contour.getWest(), contour.getNorth()),
                                          QPointF(contour.getEast(), contour.getSouth())));
    }
#ifdef QT_DEBUG
    else
    {
        qDebug() << "По какой-то причине контур пуст!";
    }
#endif
}
