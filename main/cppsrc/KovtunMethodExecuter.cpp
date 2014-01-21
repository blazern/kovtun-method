#include "KovtunMethodExecuter.h"
#include "RectangleToolKit.h"

#ifdef QT_DEBUG
#include <QDebug>
#endif

KovtunMethodExecuter::KovtunMethodExecuter(const ClosedContour & contour) :
    contour(contour),
    activeRectangles(),
    filledRectangles(),
    unitDimension(defaultUnitDimension),
    colorDictionary()
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
        calculateNewActiveRectangles();
    }

    for (auto & listener : listeners)
    {
        listener->onStepFinished();
    }
}

void KovtunMethodExecuter::calculateNewActiveRectangles()
{
    QVector<QSharedPointer<KovtunQRectF> > newActiveRectangles;

    for (auto iterator = activeRectangles.begin(); iterator != activeRectangles.end(); iterator++)
    {
        QSharedPointer<KovtunQRectF> & activeRectangle = *iterator;

        if (!RectangleToolKit::isAnyPointOfAnyLineOfContourInsideOfRectangle(contour, *activeRectangle))
        {
            if (RectangleToolKit::isAnyPointOfRectangleInsideOfContour(contour, *activeRectangle))
            {
                const QColor color = colorDictionary.getColorFor(*activeRectangle);
                activeRectangle->setColor(color);

                filledRectangles.push_back(activeRectangle);

                for (auto & listener : listeners)
                {
                    listener->onColorGathered(color, *activeRectangle);
                }
            }
            continue;
        }

        const QPointF gravityCenter = RectangleToolKit::calculateGravityCenter(contour, *activeRectangle, unitDimension);

        for (auto & listener : listeners)
        {
            listener->onGravityCenterCalculated(gravityCenter, *activeRectangle);
        }

        QSharedPointer<KovtunQRectF> topLeftRectangle(
                    new KovtunQRectF(
                        activeRectangle->left(),
                        activeRectangle->top(),
                        gravityCenter.x() - activeRectangle->left(),
                        gravityCenter.y() - activeRectangle->top(),
                        activeRectangle->getName() + " 2"));

        QSharedPointer<KovtunQRectF> topRightRectangle(
                    new KovtunQRectF(
                        gravityCenter.x(),
                        activeRectangle->top(),
                        activeRectangle->right() - gravityCenter.x(),
                        gravityCenter.y() - activeRectangle->top(),
                        activeRectangle->getName() + " 1"));

        QSharedPointer<KovtunQRectF> bottomRightRectangle(
                    new KovtunQRectF(
                        gravityCenter.x(),
                        gravityCenter.y(),
                        activeRectangle->right() - gravityCenter.x(),
                        activeRectangle->bottom() - gravityCenter.y(),
                        activeRectangle->getName() + " 4"));

        QSharedPointer<KovtunQRectF> bottomLeftRectangle(
                    new KovtunQRectF(
                        activeRectangle->left(),
                        gravityCenter.y(),
                        gravityCenter.x() - activeRectangle->left(),
                        activeRectangle->bottom() - gravityCenter.y(),
                        activeRectangle->getName() + " 3"));

        KovtunQRectF::makeNeighbors(topLeftRectangle, topRightRectangle);
        KovtunQRectF::makeNeighbors(topLeftRectangle, bottomLeftRectangle);
        KovtunQRectF::makeNeighbors(topLeftRectangle, bottomRightRectangle);

        KovtunQRectF::makeNeighbors(bottomLeftRectangle, bottomRightRectangle);
        KovtunQRectF::makeNeighbors(bottomLeftRectangle, topRightRectangle);

        KovtunQRectF::makeNeighbors(bottomRightRectangle, topRightRectangle);

        shareNeighbors(activeRectangle, topLeftRectangle);
        shareNeighbors(activeRectangle, topRightRectangle);
        shareNeighbors(activeRectangle, bottomRightRectangle);
        shareNeighbors(activeRectangle, bottomLeftRectangle);

        newActiveRectangles.push_back(topRightRectangle);
        newActiveRectangles.push_back(topLeftRectangle);
        newActiveRectangles.push_back(bottomLeftRectangle);
        newActiveRectangles.push_back(bottomRightRectangle);
    }

    activeRectangles.clear();
    activeRectangles << newActiveRectangles;
}

void KovtunMethodExecuter::shareNeighbors(QSharedPointer<KovtunQRectF> & source, QSharedPointer<KovtunQRectF> & destination) const
{
    for (KovtunQRectF::neighborsIterator iterator = source->neighborsBegin();
         iterator != source->neighborsEnd();
         iterator++)
    {
        QSharedPointer<KovtunQRectF> sourcesNeighbor = *iterator;

        if (RectangleToolKit::doRectanglesTouchEachOther(*sourcesNeighbor, *destination))
        {
            KovtunQRectF::makeNeighbors(sourcesNeighbor, destination);
        }
    }
}

void KovtunMethodExecuter::reset()
{
    activeRectangles.clear();
    filledRectangles.clear();

    for (auto & listener : listeners)
    {
        listener->onReset();
    }
}

void KovtunMethodExecuter::removeListener(KovtunMethodExecuterListener & listener)
{
    for (auto iterator = listeners.begin(); iterator != listeners.end(); iterator++)
    {
        if (&listener == *iterator)
        {
            listeners.erase(iterator);
            return;
        }
    }
}

void KovtunMethodExecuter::calculateFirstActiveRectangle()
{
    if (contour.getPointsCount() > 0)
    {
        activeRectangles.push_back(
                    QSharedPointer<KovtunQRectF>(
                        new KovtunQRectF(QPointF(contour.getWest(), contour.getSouth()),
                                         QPointF(contour.getEast(), contour.getNorth()),
                                         "0")));
    }
#ifdef QT_DEBUG
    else
    {
        qDebug() << "По какой-то причине контур пуст!";
    }
#endif
}
