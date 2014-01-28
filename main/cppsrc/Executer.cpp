#include "Executer.h"
#include "RectangleToolKit.h"

#ifdef QT_DEBUG
#include <QDebug>
#endif

//#define KOVTUN_FRACTAL_METHOD

namespace KovtunMethod
{

Executer::Executer(const ClosedContour & contour) :
    executionStarted(false),
    contour(contour),
    activeRectangles(),
    filledRectangles(),
    aboutToGetFilledRectangles(),
    unitDimension(defaultUnitDimension),
    colorDictionary(),
    errors(),
    firstRectangleArea(-1)
{
}

void Executer::performNextStep()
{
    if (!executionStarted)
    {
        executionStarted = true;
        calculateFirstActiveRectangle();
        firstRectangleArea = activeRectangles[0]->getArea();
    }
    else
    {
        for (auto & rectangle : aboutToGetFilledRectangles)
        {
            for (auto & listener : listeners)
            {
                listener->onColorGathered(rectangle->getColor(), *rectangle);
            }
        }
        filledRectangles << aboutToGetFilledRectangles;
        aboutToGetFilledRectangles.clear();
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
                << createTopRightRectangleFrom(*activeRectangle, gravityCenter)
                << createTopLeftRectangleFrom(*activeRectangle, gravityCenter)
                << createBottomLeftRectangleFrom(*activeRectangle, gravityCenter)
                << createBottomRightRectangleFrom(*activeRectangle, gravityCenter);


        leaveOnlyAtLeastPartlyInsideOfContourRectangles(potentialActiveRectangles, contour);
        makeNeighbors(potentialActiveRectangles);
        shareNeighbors(activeRectangle, potentialActiveRectangles);
        moveAndFillRectanglesWhichShouldBeFilled(potentialActiveRectangles, aboutToGetFilledRectangles);

        newActiveRectangles << potentialActiveRectangles;
    }

    activeRectangles.clear();
    activeRectangles << newActiveRectangles;
}

QSharedPointer<MyQRectF> Executer::createTopLeftRectangleFrom(const MyQRectF & parent, const QPointF & parentGravityCenter) const
{
    bool ancestryHaveGravityCenterBeyondContour = false;

    if (parent.doesAncestryHaveGravityCenterBeyondContour())
    {
        ancestryHaveGravityCenterBeyondContour = true;
    }
    else if (!contour.containsInside(parentGravityCenter))
    {
        ancestryHaveGravityCenterBeyondContour = true;
    }

    return QSharedPointer<MyQRectF>(
                new MyQRectF(
                    parent.left(),
                    parent.top(),
                    parentGravityCenter.x() - parent.left(),
                    parentGravityCenter.y() - parent.top(),
                    parent.getName() + " 2",
                    &parentGravityCenter,
                    parent.getParentsGravityCenter(),
                    ancestryHaveGravityCenterBeyondContour));
}

QSharedPointer<MyQRectF> Executer::createTopRightRectangleFrom(const MyQRectF & parent, const QPointF & parentGravityCenter) const
{
    bool ancestryHaveGravityCenterBeyondContour = false;

    if (parent.doesAncestryHaveGravityCenterBeyondContour())
    {
        ancestryHaveGravityCenterBeyondContour = true;
    }
    else if (!contour.containsInside(parentGravityCenter))
    {
        ancestryHaveGravityCenterBeyondContour = true;
    }

    return QSharedPointer<MyQRectF>(
                new MyQRectF(
                    parentGravityCenter.x(),
                    parent.top(),
                    parent.right() - parentGravityCenter.x(),
                    parentGravityCenter.y() - parent.top(),
                    parent.getName() + " 1",
                    &parentGravityCenter,
                    parent.getParentsGravityCenter(),
                    ancestryHaveGravityCenterBeyondContour));
}

QSharedPointer<MyQRectF> Executer::createBottomRightRectangleFrom(const MyQRectF & parent, const QPointF & parentGravityCenter) const
{
    bool ancestryHaveGravityCenterBeyondContour = false;

    if (parent.doesAncestryHaveGravityCenterBeyondContour())
    {
        ancestryHaveGravityCenterBeyondContour = true;
    }
    else if (!contour.containsInside(parentGravityCenter))
    {
        ancestryHaveGravityCenterBeyondContour = true;
    }

    return QSharedPointer<MyQRectF>(
                new MyQRectF(
                    parentGravityCenter.x(),
                    parentGravityCenter.y(),
                    parent.right() - parentGravityCenter.x(),
                    parent.bottom() - parentGravityCenter.y(),
                    parent.getName() + " 4",
                    &parentGravityCenter,
                    parent.getParentsGravityCenter(),
                    ancestryHaveGravityCenterBeyondContour));
}

QSharedPointer<MyQRectF> Executer::createBottomLeftRectangleFrom(const MyQRectF & parent, const QPointF & parentGravityCenter) const
{
    bool ancestryHaveGravityCenterBeyondContour = false;

    if (parent.doesAncestryHaveGravityCenterBeyondContour())
    {
        ancestryHaveGravityCenterBeyondContour = true;
    }
    else if (!contour.containsInside(parentGravityCenter))
    {
        ancestryHaveGravityCenterBeyondContour = true;
    }

    return QSharedPointer<MyQRectF>(
                new MyQRectF(
                    parent.left(),
                    parentGravityCenter.y(),
                    parentGravityCenter.x() - parent.left(),
                    parent.bottom() - parentGravityCenter.y(),
                    parent.getName() + " 3",
                    &parentGravityCenter,
                    parent.getParentsGravityCenter(),
                    ancestryHaveGravityCenterBeyondContour));
}

void Executer::leaveOnlyAtLeastPartlyInsideOfContourRectangles(QVector<QSharedPointer<MyQRectF> > & rectangles, const ClosedContour & contour) const
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

void Executer::moveAndFillRectanglesWhichShouldBeFilled(
        QVector<QSharedPointer<MyQRectF> > & potentialRectangles,
        QVector<QSharedPointer<MyQRectF> > & certainRectangles)
{
#ifndef KOVTUN_FRACTAL_METHOD
        bool keyRectangleFilled = false;
        QColor keyRectangleColor;
#endif
        for (auto iterator = potentialRectangles.begin(); iterator != potentialRectangles.end(); iterator++)
        {
            const QSharedPointer<MyQRectF> & potentialRectangle = *iterator;

            const QPointF * gravityCenterOfParent = potentialRectangle->getParentsGravityCenter();
            const QPointF * gravityCenterOfGrandParent = potentialRectangle->getGrandParentsGravityCenter();

            if (gravityCenterOfParent == nullptr || gravityCenterOfGrandParent == nullptr)
            {
                break;
            }

            if (RectangleToolKit::isRectangleInsideOfContour(contour, *potentialRectangle))
            {
                const QLineF gravityCentersLine(*gravityCenterOfGrandParent, *gravityCenterOfParent);

                if (RectangleToolKit::doesLineIntersectRectangle(gravityCentersLine, *potentialRectangle))
                {
                    QColor color;
                    if (!potentialRectangle->doesAncestryHaveGravityCenterBeyondContour())
                    {
                        color = colorDictionary.getColorFor(*potentialRectangle);
                    }
                    else
                    {
                        color = colorDictionary.getColorFor(potentialRectangles);
                    }
                    potentialRectangle->setColor(color);

                    certainRectangles << potentialRectangle;

#ifndef KOVTUN_FRACTAL_METHOD
                    keyRectangleFilled = true;
                    keyRectangleColor = color;
#endif

                    potentialRectangles.erase(iterator);
                    break;
                }
            }
        }

#ifndef KOVTUN_FRACTAL_METHOD
        if (keyRectangleFilled)
        {
            for (auto iterator = potentialRectangles.begin(); iterator != potentialRectangles.end();)
            {
                QSharedPointer<MyQRectF> potentialRectangle = *iterator;
                if (RectangleToolKit::isRectangleInsideOfContour(contour, *potentialRectangle))
                {
                    potentialRectangle->setColor(keyRectangleColor);
                    certainRectangles << potentialRectangle;

                    iterator = potentialRectangles.erase(iterator);
                }
                else
                {
                    iterator++;
                }
            }
        }
#endif
}

void Executer::shareNeighbors(QSharedPointer<MyQRectF> & source, QSharedPointer<MyQRectF> & destination) const
{
    for (auto & sourcesNeighbor : source->getNeighborhood())
    {
        if (RectangleToolKit::doRectanglesTouchEachOther(*sourcesNeighbor, *destination))
        {
            sourcesNeighbor->addNeighbor(*destination);
        }
    }
}

void Executer::reset()
{
    executionStarted = false;
    activeRectangles.clear();
    filledRectangles.clear();
    aboutToGetFilledRectangles.clear();
    errors.clear();

    for (auto & listener : listeners)
    {
        listener->onReset();
    }
}

const MyQRectF & Executer::getActiveRectangle(const int index) const
{
    if (index < activeRectangles.size())
    {
        return *activeRectangles[index];
    }
    else
    {
        const int realIndex = index - activeRectangles.size();
        return *aboutToGetFilledRectangles[realIndex];
    }
}

int Executer::getActiveRectanglesCount() const
{
    return activeRectangles.size() + aboutToGetFilledRectangles.size();
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
                                     nullptr,
                                     false)));

    }
#ifdef QT_DEBUG
    else
    {
        qDebug() << "По какой-то причине контур пуст!";
    }
#endif
}

}
