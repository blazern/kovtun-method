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
            filledRectangles.push_back(activeRectangle);
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

        QVector<QSharedPointer<MyQRectF> > potentialActiveRectangles;

        potentialActiveRectangles
                << createTopLeftRectangleFrom(*activeRectangle, gravityCenter)
                   << createTopRightRectangleFrom(*activeRectangle, gravityCenter)
                      << createBottomRightRectangleFrom(*activeRectangle, gravityCenter)
                         << createBottomLeftRectangleFrom(*activeRectangle, gravityCenter);

        leaveOnlyInsideOfContourRectangles(potentialActiveRectangles, contour);
        makeNeighbors(potentialActiveRectangles);
        shareNeighbors(activeRectangle, potentialActiveRectangles);

        newActiveRectangles << potentialActiveRectangles;
    }

    activeRectangles.clear();
    activeRectangles << newActiveRectangles;
}

QSharedPointer<MyQRectF> Executer::createTopLeftRectangleFrom(const MyQRectF & parent, const QPointF & parentGravityCenter) const
{
    return QSharedPointer<MyQRectF>(
                new MyQRectF(
                    parent.left(),
                    parent.top(),
                    parentGravityCenter.x() - parent.left(),
                    parentGravityCenter.y() - parent.top(),
                    parent.getName() + " 2",
                    &parentGravityCenter,
                    parent.getParentsGravityCenter()));
}

QSharedPointer<MyQRectF> Executer::createTopRightRectangleFrom(const MyQRectF & parent, const QPointF & parentGravityCenter) const
{
    return QSharedPointer<MyQRectF>(
                new MyQRectF(
                    parentGravityCenter.x(),
                    parent.top(),
                    parent.right() - parentGravityCenter.x(),
                    parentGravityCenter.y() - parent.top(),
                    parent.getName() + " 1",
                    &parentGravityCenter,
                    parent.getParentsGravityCenter()));
}

QSharedPointer<MyQRectF> Executer::createBottomRightRectangleFrom(const MyQRectF & parent, const QPointF & parentGravityCenter) const
{
    return QSharedPointer<MyQRectF>(
                new MyQRectF(
                    parentGravityCenter.x(),
                    parentGravityCenter.y(),
                    parent.right() - parentGravityCenter.x(),
                    parent.bottom() - parentGravityCenter.y(),
                    parent.getName() + " 4",
                    &parentGravityCenter,
                    parent.getParentsGravityCenter()));
}

QSharedPointer<MyQRectF> Executer::createBottomLeftRectangleFrom(const MyQRectF & parent, const QPointF & parentGravityCenter) const
{
    return QSharedPointer<MyQRectF>(
                new MyQRectF(
                    parent.left(),
                    parentGravityCenter.y(),
                    parentGravityCenter.x() - parent.left(),
                    parent.bottom() - parentGravityCenter.y(),
                    parent.getName() + " 3",
                    &parentGravityCenter,
                    parent.getParentsGravityCenter()));
}

void Executer::leaveOnlyInsideOfContourRectangles(QVector<QSharedPointer<MyQRectF> > & rectangles, const ClosedContour & contour) const
{
    for (auto iterator = rectangles.begin(); iterator != rectangles.end();)
    {
        if (!RectangleToolKit::isAnyPointOfRectangleInsideOfContour(contour, **iterator))
        {
            iterator = rectangles.erase(iterator);
            continue;
        }
        iterator++;
    }
}

void Executer::shareNeighbors(QSharedPointer<MyQRectF> & source, QVector<QSharedPointer<MyQRectF> > & destinations) const
{
    for (auto & destination : destinations)
    {
        shareNeighbors(source, destination);
    }
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
    const QPointF * gravityCenterOfParent = rectangle->getParentsGravityCenter();
    const QPointF * gravityCenterOfGrandParent = rectangle->getGrandParentsGravityCenter();

    if (gravityCenterOfParent == nullptr || gravityCenterOfGrandParent == nullptr)
    {
        return false;
    }

    if (RectangleToolKit::isRectangleInsideOfContour(contour, *rectangle))
    {
        const QLineF gravityCentersLine(*gravityCenterOfGrandParent, *gravityCenterOfParent);

        if (RectangleToolKit::doesLineIntersectRectangle(gravityCentersLine, *rectangle))
        {
            const QColor color = colorDictionary.getColorFor(*rectangle);
            rectangle->setColor(color);

            for (auto & listener : listeners)
            {
                listener->onColorGathered(color, *rectangle);
            }

            return true;
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
