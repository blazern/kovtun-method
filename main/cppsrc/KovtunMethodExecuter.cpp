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
        count = 0;
        calculateFirstActiveRectangle();

        firstRectangleArea = activeRectangles[0]->getSquare();
    }
    else
    {
        count++;
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

        if (tryToFill(activeRectangle))
        {
            continue;
        }

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
                        &gravityCenter,
                        activeRectangle->getParentsGravityCenter()));

        QSharedPointer<KovtunQRectF> topRightRectangle(
                    new KovtunQRectF(
                        gravityCenter.x(),
                        activeRectangle->top(),
                        activeRectangle->right() - gravityCenter.x(),
                        gravityCenter.y() - activeRectangle->top(),
                        activeRectangle->getName() + " 1",
                        &gravityCenter,
                        activeRectangle->getParentsGravityCenter()));

        QSharedPointer<KovtunQRectF> bottomRightRectangle(
                    new KovtunQRectF(
                        gravityCenter.x(),
                        gravityCenter.y(),
                        activeRectangle->right() - gravityCenter.x(),
                        activeRectangle->bottom() - gravityCenter.y(),
                        activeRectangle->getName() + " 4",
                        &gravityCenter,
                        activeRectangle->getParentsGravityCenter()));

        QSharedPointer<KovtunQRectF> bottomLeftRectangle(
                    new KovtunQRectF(
                        activeRectangle->left(),
                        gravityCenter.y(),
                        gravityCenter.x() - activeRectangle->left(),
                        activeRectangle->bottom() - gravityCenter.y(),
                        activeRectangle->getName() + " 3",
                        &gravityCenter,
                        activeRectangle->getParentsGravityCenter()));

        QVector<QSharedPointer<KovtunQRectF> > potentialActiveRectangles;

        if (RectangleToolKit::isAnyPointOfRectangleInsideOfContour(contour, *topLeftRectangle))
        {
            potentialActiveRectangles << topLeftRectangle;
        }

        if (RectangleToolKit::isAnyPointOfRectangleInsideOfContour(contour, *topRightRectangle))
        {
            potentialActiveRectangles << topRightRectangle;
        }

        if (RectangleToolKit::isAnyPointOfRectangleInsideOfContour(contour, *bottomRightRectangle))
        {
            potentialActiveRectangles << bottomRightRectangle;
        }

        if (RectangleToolKit::isAnyPointOfRectangleInsideOfContour(contour, *bottomLeftRectangle))
        {
            potentialActiveRectangles << bottomLeftRectangle;
        }

        makeNeighbors(potentialActiveRectangles);

        for (auto & rectangle : potentialActiveRectangles)
        {
            shareNeighbors(activeRectangle, rectangle);
        }

        newActiveRectangles << potentialActiveRectangles;
    }

    activeRectangles.clear();
    activeRectangles << newActiveRectangles;
}

void KovtunMethodExecuter::makeNeighbors(QVector<QSharedPointer<KovtunQRectF> > & futureNeighbors) const
{
    for (int firstIndex = 0; firstIndex < futureNeighbors.size(); firstIndex++)
    {
        for (int secondIndex = firstIndex + 1; secondIndex < futureNeighbors.size(); secondIndex++)
        {
            KovtunQRectF::makeNeighbors(futureNeighbors[firstIndex], futureNeighbors[secondIndex]);
        }
    }
}

bool KovtunMethodExecuter::tryToFill(QSharedPointer<KovtunQRectF> & rectangle)
{
    const QPointF * gravityCenterOfGrandParent = rectangle->getGrandParentsGravityCenter();
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
                                         nullptr,
                                         nullptr)));

    }
#ifdef QT_DEBUG
    else
    {
        qDebug() << "По какой-то причине контур пуст!";
    }
#endif
}
