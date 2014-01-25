#include "Executer.h"
#include "RectangleToolKit.h"

#ifdef QT_DEBUG
#include <QDebug>
#endif

namespace KovtunMethod
{

Executer::Executer(const ClosedContour & contour) :
    contour(contour),
    activeRectangles(),
    filledRectangles(),
    unitDimension(defaultUnitDimension),
    colorDictionary(),
    errors(),
    firstRectangleArea(-1)
{
}

void Executer::performNextStep()
{
    if (activeRectangles.size() == 0)
    {
        count = 0;
        calculateFirstActiveRectangle();

        firstRectangleArea = activeRectangles[0]->getArea();
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

void Executer::calculateNewActiveRectangles()
{
    QVector<QSharedPointer<MyQRectF> > newActiveRectangles;

    for (auto iterator = activeRectangles.begin(); iterator != activeRectangles.end(); iterator++)
    {
        QSharedPointer<MyQRectF> & activeRectangle = *iterator;

        if (tryToFill(activeRectangle))
        {
            continue;
        }

        const QPair<QPointF, double> gravityCenterWithError = RectangleToolKit::calculateGravityCenter(contour, *activeRectangle, unitDimension);
        const QPointF & gravityCenter = gravityCenterWithError.first;
        const double error = gravityCenterWithError.second;

        errors << (error * (activeRectangle->getArea() / firstRectangleArea)) / (activeRectangles.size() + filledRectangles.size());

        for (auto & listener : listeners)
        {
            listener->onGravityCenterCalculated(gravityCenter, error, *activeRectangle);
        }

        QSharedPointer<MyQRectF> topLeftRectangle(
                    new MyQRectF(
                        activeRectangle->left(),
                        activeRectangle->top(),
                        gravityCenter.x() - activeRectangle->left(),
                        gravityCenter.y() - activeRectangle->top(),
                        activeRectangle->getName() + " 2",
                        &gravityCenter,
                        activeRectangle->getParentsGravityCenter()));

        QSharedPointer<MyQRectF> topRightRectangle(
                    new MyQRectF(
                        gravityCenter.x(),
                        activeRectangle->top(),
                        activeRectangle->right() - gravityCenter.x(),
                        gravityCenter.y() - activeRectangle->top(),
                        activeRectangle->getName() + " 1",
                        &gravityCenter,
                        activeRectangle->getParentsGravityCenter()));

        QSharedPointer<MyQRectF> bottomRightRectangle(
                    new MyQRectF(
                        gravityCenter.x(),
                        gravityCenter.y(),
                        activeRectangle->right() - gravityCenter.x(),
                        activeRectangle->bottom() - gravityCenter.y(),
                        activeRectangle->getName() + " 4",
                        &gravityCenter,
                        activeRectangle->getParentsGravityCenter()));

        QSharedPointer<MyQRectF> bottomLeftRectangle(
                    new MyQRectF(
                        activeRectangle->left(),
                        gravityCenter.y(),
                        gravityCenter.x() - activeRectangle->left(),
                        activeRectangle->bottom() - gravityCenter.y(),
                        activeRectangle->getName() + " 3",
                        &gravityCenter,
                        activeRectangle->getParentsGravityCenter()));

        QVector<QSharedPointer<MyQRectF> > potentialActiveRectangles;

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

void Executer::makeNeighbors(QVector<QSharedPointer<MyQRectF> > & futureNeighbors) const
{
    for (int firstIndex = 0; firstIndex < futureNeighbors.size(); firstIndex++)
    {
        for (int secondIndex = firstIndex + 1; secondIndex < futureNeighbors.size(); secondIndex++)
        {
            futureNeighbors[firstIndex]->addNeighbor(*futureNeighbors[secondIndex]);
        }
    }
}

bool Executer::tryToFill(QSharedPointer<MyQRectF> & rectangle)
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

void Executer::shareNeighbors(QSharedPointer<MyQRectF> & source, QSharedPointer<MyQRectF> & destination) const
{
    for (const auto & sourcesNeighbor : source->getNeighborhood())
    {
        if (RectangleToolKit::doRectanglesTouchEachOther(*sourcesNeighbor, *destination))
        {
            destination->addNeighbor(*sourcesNeighbor);
        }
    }
}

void Executer::reset()
{
    activeRectangles.clear();
    filledRectangles.clear();
    errors.clear();

    for (auto & listener : listeners)
    {
        listener->onReset();
    }
}

double Executer::getCurrentError() const
{
    double resultError = 0;

    for (auto error : errors)
    {
        resultError += error;
    }

    return resultError;
}

void Executer::removeListener(ExecuterListener & listener)
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

void Executer::calculateFirstActiveRectangle()
{
    if (contour.getPointsCount() > 0)
    {
        activeRectangles.push_back(
                    QSharedPointer<MyQRectF>(
                        new MyQRectF(QPointF(contour.getWest(), contour.getNorth()),
                                     QPointF(contour.getEast(), contour.getSouth()),
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

}
