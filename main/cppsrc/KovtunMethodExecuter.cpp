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
    colorDictionary(),
    errors(),
    firstRectangleArea(-1)
{
}

void KovtunMethodExecuter::performNextStep()
{
    if (activeRectangles.size() == 0)
    {
        calculateFirstActiveRectangle();

        firstRectangleArea = activeRectangles[0]->getSquare();
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

        const QPair<QPointF, double> gravityCenterWithError = RectangleToolKit::calculateGravityCenter(contour, *activeRectangle, unitDimension);
        const QPointF & gravityCenter = gravityCenterWithError.first;
        const double error = gravityCenterWithError.second;

        errors << (error * (activeRectangle->getSquare() / firstRectangleArea)) / (activeRectangles.size() + filledRectangles.size());

        for (auto & listener : listeners)
        {
            listener->onGravityCenterCalculated(gravityCenter, error, *activeRectangle);
        }

        QSharedPointer<KovtunQRectF> topLeftRectangle(
                    new KovtunQRectF(
                        activeRectangle->left(),
                        activeRectangle->top(),
                        gravityCenter.x() - activeRectangle->left(),
                        gravityCenter.y() - activeRectangle->top(),
                        activeRectangle->getName() + " 2",
                        &gravityCenter));

        QSharedPointer<KovtunQRectF> topRightRectangle(
                    new KovtunQRectF(
                        gravityCenter.x(),
                        activeRectangle->top(),
                        activeRectangle->right() - gravityCenter.x(),
                        gravityCenter.y() - activeRectangle->top(),
                        activeRectangle->getName() + " 1",
                        &gravityCenter));

        QSharedPointer<KovtunQRectF> bottomRightRectangle(
                    new KovtunQRectF(
                        gravityCenter.x(),
                        gravityCenter.y(),
                        activeRectangle->right() - gravityCenter.x(),
                        activeRectangle->bottom() - gravityCenter.y(),
                        activeRectangle->getName() + " 4",
                        &gravityCenter));

        QSharedPointer<KovtunQRectF> bottomLeftRectangle(
                    new KovtunQRectF(
                        activeRectangle->left(),
                        gravityCenter.y(),
                        gravityCenter.x() - activeRectangle->left(),
                        activeRectangle->bottom() - gravityCenter.y(),
                        activeRectangle->getName() + " 3",
                        &gravityCenter));

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

        if(!tryToFill(topRightRectangle, activeRectangle->getParentsGravityCenter())) newActiveRectangles << topRightRectangle;
        if(!tryToFill(topLeftRectangle, activeRectangle->getParentsGravityCenter())) newActiveRectangles << topLeftRectangle;
        if(!tryToFill(bottomLeftRectangle, activeRectangle->getParentsGravityCenter())) newActiveRectangles << bottomLeftRectangle;
        if(!tryToFill(bottomRightRectangle, activeRectangle->getParentsGravityCenter())) newActiveRectangles << bottomRightRectangle;
    }

    activeRectangles.clear();
    activeRectangles << newActiveRectangles;
}

bool KovtunMethodExecuter::tryToFill(QSharedPointer<KovtunQRectF> & rectangle, const QPointF * gravityCenterOfGrandParent)
{
    if (rectangle->getParentsGravityCenter() != nullptr && gravityCenterOfGrandParent != nullptr)
    {
        if (!RectangleToolKit::isAnyPointOfAnyLineOfContourInsideOfRectangle(contour, *rectangle))
        {
            if (RectangleToolKit::isAnyPointOfRectangleInsideOfContour(contour, *rectangle))
            {
                const QLineF gravityCentersLine(*gravityCenterOfGrandParent, *rectangle->getParentsGravityCenter());

                if (RectangleToolKit::doesLineIntersectRectangle(gravityCentersLine, *rectangle))
                {
                    const QColor color = colorDictionary.getColorFor(*rectangle);
                    rectangle->setColor(color);

                    filledRectangles.push_back(rectangle);

                    for (auto & listener : listeners)
                    {
                        listener->onColorGathered(color, *rectangle);
                    }

                    return true;
                }
            }
        }
    }
    return false;
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
    errors.clear();

    for (auto & listener : listeners)
    {
        listener->onReset();
    }
}

double KovtunMethodExecuter::getCurrentError() const
{
    double resultError = 0;

    for (auto error : errors)
    {
        resultError += error;
    }

    return resultError;
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
                                         "0",
                                         nullptr)));

    }
#ifdef QT_DEBUG
    else
    {
        qDebug() << "По какой-то причине контур пуст!";
    }
#endif
}
