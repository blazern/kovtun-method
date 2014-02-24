#include "Executer.h"
#include "RectangleToolKit.h"
#include "NotSingleShape.h"
#include "UnitsToolKit.h"

#ifdef QT_DEBUG
#include <QDebug>
#endif

namespace KovtunMethod
{

Executer::Executer(const ClosedContour & contour) :
    executionStarted(false),
    contour(contour),
    currentActiveRectangles(),
    filledRectangles(),
    aboutToGetFilledRectangles(),
    unitDimension(DEFAULT_UNITS_DIMENSION),
    colorDictionary(),
    errors(),
    firstRectangleArea(-1),
    neighborMethodIsSet(false)
{
}

void Executer::performNextStep()
{
    for (auto & listener : listeners)
    {
        listener->onStepStarted();
    }

    if (!executionStarted)
    {
        executionStarted = true;
        calculateFirstActiveRectangle();
        firstRectangleArea = currentActiveRectangles[0]->getArea();
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
    
    for (int index = 0; index < currentActiveRectangles.size(); index++)
    {
        QSharedPointer<MyQRectF> & activeRectangle = currentActiveRectangles[index];

        QPair<QPointF, double> gravityCenterWithError;

        try
        {
            gravityCenterWithError =
                    UnitsToolKit::calculateGravityCenter(contour, *activeRectangle, unitDimension);
        }
        catch (const NotSingleShape &)
        {
            const QVector<QSharedPointer<MyQRectF> > dividedRectangles =
                    UnitsToolKit::getDescribingRectanglesFrom(contour, *activeRectangle, unitDimension);

            for (const auto & dividedRectangle : dividedRectangles)
            {
                currentActiveRectangles.insert(index + 1, dividedRectangle);
            }

            for (auto & listener : listeners)
            {
                listener->onActiveRectangleProcessed();
            }

            continue;
        }

        const QPointF & gravityCenter = gravityCenterWithError.first;
        const double error = gravityCenterWithError.second;
        
        errors << (error * (activeRectangle->getArea() / firstRectangleArea)) / (currentActiveRectangles.size() + filledRectangles.size());
        
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

        for (auto & listener : listeners)
        {
            listener->onActiveRectangleProcessed();
        }
    }
    
    currentActiveRectangles.clear();
    currentActiveRectangles << newActiveRectangles;
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
    if (!neighborMethodIsSet)
    {
        findAndFillKeyRectangle(potentialRectangles, certainRectangles);
    }
    else
    {
        const QPair<bool, QColor> fillingResult = findAndFillKeyRectangle(potentialRectangles, certainRectangles);

        const bool keyRectangleFilled = fillingResult.first;

        if (keyRectangleFilled)
        {
            fillInternalRectangles(potentialRectangles, certainRectangles, fillingResult.second);
        }
    }
}

QPair<bool, QColor> Executer::findAndFillKeyRectangle(QVector<QSharedPointer<MyQRectF> > & potentialRectangles, QVector<QSharedPointer<MyQRectF> > & certainRectangles)
{
    bool keyRectangleFilled = false;
    QColor keyRectangleColor;

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

            if (RectangleToolKit::isAnyPointOfLineInsideOfRectangle(gravityCentersLine, *potentialRectangle))
            {
                const QColor color = colorDictionary.getColorFor(*potentialRectangle);
                potentialRectangle->setColor(color);

                certainRectangles << potentialRectangle;

                keyRectangleFilled = true;
                keyRectangleColor = color;

                potentialRectangles.erase(iterator);
                break;
            }
        }
    }

    return QPair<bool, QColor>(keyRectangleFilled, keyRectangleColor);
}

void Executer::fillInternalRectangles(QVector<QSharedPointer<MyQRectF> > & candidates, QVector<QSharedPointer<MyQRectF> > & filledRectangles, const QColor & color)
{
    for (auto iterator = candidates.begin(); iterator != candidates.end();)
    {
        QSharedPointer<MyQRectF> potentialRectangle = *iterator;
        if (RectangleToolKit::isRectangleInsideOfContour(contour, *potentialRectangle))
        {
            potentialRectangle->setColor(color);
            filledRectangles << potentialRectangle;

            iterator = candidates.erase(iterator);
        }
        else
        {
            iterator++;
        }
    }
}

void Executer::shareNeighbors(QSharedPointer<MyQRectF> & source, QSharedPointer<MyQRectF> & destination) const
{
    for (auto & sourcesNeighbor : source->getNeighborhood())
    {
        if (RectangleToolKit::doRectanglesTouchEachOther(*sourcesNeighbor, *destination) || sourcesNeighbor->intersects(*destination))
        {
            sourcesNeighbor->addNeighbor(*destination);
        }
    }
}

void Executer::reset()
{
    executionStarted = false;
    currentActiveRectangles.clear();
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
    if (index < currentActiveRectangles.size())
    {
        return *currentActiveRectangles[index];
    }
    else
    {
        const int realIndex = index - currentActiveRectangles.size();
        return *aboutToGetFilledRectangles[realIndex];
    }
}

int Executer::getActiveRectanglesCount() const
{
    return currentActiveRectangles.size() + aboutToGetFilledRectangles.size();
}

int Executer::getCurrentActiveRectanglesCount() const
{
    return currentActiveRectangles.size();
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

void Executer::setNeighborMethodExecution()
{
    neighborMethodIsSet = true;
    reset();
}

void Executer::setFractalMethodEXecution()
{
    neighborMethodIsSet = false;
    reset();
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
        currentActiveRectangles.push_back(
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
