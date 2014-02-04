#include "UnitsToolKit.h"

#include <QSharedPointer>
#include "RectangleToolKit.h"
#include "NotSingleShape.h"
#include <QDebug>

namespace KovtunMethod
{

QPair<QPointF, double> UnitsToolKit::calculateGravityCenter(const ClosedContour & contour, const QRectF & rectangle, const int maximumUnits)
{
    const qreal unitWidth = rectangle.width() / maximumUnits;
    const qreal unitHeight = rectangle.height() / maximumUnits;
    QRectF reusableUnit;
    QVector<QPointF> insideUnits;
    int errorUnitsCount = 0;
    int goodUnitsCount = 0;

    QVector<QPoint> unitsInSingleShape;

    for (int horIndex = 0; horIndex < maximumUnits; horIndex++)
    {
        const qreal newX = rectangle.left() + horIndex * unitWidth;
        reusableUnit.setX(newX);

        for (int vertIndex = 0; vertIndex < maximumUnits; vertIndex++)
        {
            const qreal newY = rectangle.top() + vertIndex * unitHeight;
            reusableUnit.setY(newY);
            reusableUnit.setWidth(unitWidth);
            reusableUnit.setHeight(unitHeight);

            if (RectangleToolKit::isAnyPointOfRectangleInsideOfContour(contour, reusableUnit))
            {
                // Механизм проверки того, что в переданном rectangle только одна фигура
                if (!unitsInSingleShape.contains(QPoint(horIndex, vertIndex)))
                {
                    if (unitsInSingleShape.isEmpty())
                    {
                        unitsInSingleShape.push_back(QPoint(horIndex, vertIndex));
                        putAllUnitNeighborsWhichItersectRectangleToVector(rectangle, reusableUnit, horIndex, vertIndex, contour, unitsInSingleShape);
                    }
                    else
                    {
                        throw NotSingleShape("rectangle содержит в себе более одной фигуры - центр тяжести определить невозможно!");
                    }
                }

                insideUnits.push_back(reusableUnit.center());
                if (RectangleToolKit::isAnyPointOfAnyLineOfContourInsideOfRectangle(contour, reusableUnit))
                {
                    errorUnitsCount++;
                }
                else
                {
                    goodUnitsCount++;
                }
            }
        }
    }

    const double error = goodUnitsCount > 0 ? ((double)errorUnitsCount / (double)goodUnitsCount) : errorUnitsCount;

    return QPair<QPointF, double>(calculateAveragePoint(insideUnits), error);
}

void UnitsToolKit::putAllUnitNeighborsWhichItersectRectangleToVector(
        const QRectF & targetRectangle,
        const QRectF & unit,
        const int horIndex,
        const int vertIndex,
        const ClosedContour & contour,
        QVector<QPoint> & neighbors)
{
    const qreal unitWidth = unit.width();
    const qreal unitHeight = unit.height();

    QVector<QPoint> needToCheck;

    needToCheck.push_back(QPoint(horIndex, vertIndex));

    QRectF reusableUnit;

    for (int index = 0; index < needToCheck.size(); index++)
    {
        const QPointF & currentUnitPosition = needToCheck[index];

        reusableUnit.setX(targetRectangle.left() + currentUnitPosition.x() * unitWidth);
        reusableUnit.setY(targetRectangle.top() + currentUnitPosition.y() * unitHeight);
        reusableUnit.setWidth(unitWidth);
        reusableUnit.setHeight(unitHeight);

        const qreal north = reusableUnit.top() - unitHeight;
        const qreal east = reusableUnit.left() + unitWidth;
        const qreal south = reusableUnit.top() + unitHeight;
        const qreal west = reusableUnit.left() - unitWidth;

        const QRectF northUnit(reusableUnit.x(), north, unitWidth, unitHeight);
        const QRectF eastUnit(east, reusableUnit.y(), unitWidth, unitHeight);
        const QRectF southUnit(reusableUnit.x(), south, unitWidth, unitHeight);
        const QRectF westUnit(west, reusableUnit.y(), unitWidth, unitHeight);

        const QPoint northPosition(currentUnitPosition.x(), currentUnitPosition.y() - 1);
        const QPoint sourhPosition(currentUnitPosition.x(), currentUnitPosition.y() + 1);
        const QPoint eastPosition(currentUnitPosition.x() + 1, currentUnitPosition.y());
        const QPoint westPosition(currentUnitPosition.x() - 1, currentUnitPosition.y());

        if (!neighbors.contains(northPosition)
            && targetRectangle.intersects(northUnit)
            && RectangleToolKit::isAnyPointOfRectangleInsideOfContour(contour, northUnit))
        {
            neighbors.push_back(northPosition);
            needToCheck.push_back(northPosition);
        }

        if (!neighbors.contains(eastPosition)
            && targetRectangle.intersects(eastUnit)
            && RectangleToolKit::isAnyPointOfRectangleInsideOfContour(contour, eastUnit))
        {
            neighbors.push_back(eastPosition);
            needToCheck.push_back(eastPosition);
        }

        if (!neighbors.contains(sourhPosition)
            && targetRectangle.intersects(southUnit)
            && RectangleToolKit::isAnyPointOfRectangleInsideOfContour(contour, southUnit))
        {
            neighbors.push_back(sourhPosition);
            needToCheck.push_back(sourhPosition);
        }

        if (!neighbors.contains(westPosition)
            && targetRectangle.intersects(westUnit)
            && RectangleToolKit::isAnyPointOfRectangleInsideOfContour(contour, westUnit))
        {
            neighbors.push_back(westPosition);
            needToCheck.push_back(westPosition);
        }
    }
}

const QPointF UnitsToolKit::calculateAveragePoint(const QVector<QPointF> & points)
{
    qreal averageX = 0;
    qreal averageY = 0;
    const int pointsCount = points.size();
    for (const auto & point : points)
    {
        averageX += point.x() / pointsCount;
        averageY += point.y() / pointsCount;
    }

    return QPointF(averageX, averageY);
}

QVector<QSharedPointer<MyQRectF> > UnitsToolKit::getDescribingRectanglesFrom(const ClosedContour & contour, const MyQRectF & rectangle, const int maximumUnits)
{
    const qreal unitWidth = rectangle.width() / maximumUnits;
    const qreal unitHeight = rectangle.height() / maximumUnits;

    const QVector<QVector<QPoint> > unitsGroups = extractUnitsGroups(contour, rectangle, maximumUnits, unitWidth, unitHeight);

    QVector<QSharedPointer<MyQRectF> > describingRectangles;

    for (const auto & unitsGroup : unitsGroups)
    {
        qreal north;
        qreal east;
        qreal south;
        qreal west;

        setToFarthestValues(unitsGroup, rectangle, unitWidth, unitHeight, north, east, south, west);

        describingRectangles.push_back(QSharedPointer<MyQRectF>(new MyQRectF(
                                                      QPointF(west, north),
                                                      QPointF(east, south),
                                                      rectangle.getName() + "part" + QString::number(describingRectangles.size()),
                                                      rectangle.getParentsGravityCenter(),
                                                      rectangle.getGrandParentsGravityCenter())));

        for (auto & sourcesNeighbor : rectangle.getNeighborhood())
        {
            if (RectangleToolKit::doRectanglesTouchEachOther(*sourcesNeighbor, *(describingRectangles.last()))
                || describingRectangles.last()->intersects(*sourcesNeighbor))
            {
                sourcesNeighbor->addNeighbor(*describingRectangles.last());
            }
        }
    }

    return describingRectangles;
}

QVector<QVector<QPoint> > UnitsToolKit::extractUnitsGroups(
        const ClosedContour & contour,
        const MyQRectF & rectangle,
        const int maximumUnits,
        const qreal unitWidth,
        const qreal unitHeight)
{
    QRectF reusableUnit;

    QVector<QVector<QPoint> > unitsGroups;

    for (int horIndex = 0; horIndex < maximumUnits; horIndex++)
    {
        const qreal newX = rectangle.left() + horIndex * unitWidth;
        reusableUnit.setX(newX);

        for (int vertIndex = 0; vertIndex < maximumUnits; vertIndex++)
        {
            const qreal newY = rectangle.top() + vertIndex * unitHeight;
            reusableUnit.setY(newY);
            reusableUnit.setWidth(unitWidth);
            reusableUnit.setHeight(unitHeight);

            if (RectangleToolKit::isAnyPointOfRectangleInsideOfContour(contour, reusableUnit))
            {
                bool oneOfGroupsContainCurrentUnit = false;
                const QPoint currentPosition(horIndex, vertIndex);
                for (const auto & unitsGroup : unitsGroups)
                {
                    if (unitsGroup.contains(currentPosition))
                    {
                        oneOfGroupsContainCurrentUnit = true;
                        break;
                    }
                }

                if (!oneOfGroupsContainCurrentUnit)
                {
                    unitsGroups.push_back(QVector<QPoint>());
                    unitsGroups.last().push_back(currentPosition);
                    putAllUnitNeighborsWhichItersectRectangleToVector(
                                rectangle,
                                reusableUnit,
                                horIndex,
                                vertIndex,
                                contour,
                                unitsGroups.last());
                }
            }
        }
    }

    return unitsGroups;
}

void UnitsToolKit::setToFarthestValues(
        const QVector<QPoint> & unitsGroup,
        const QRectF & rectangle,
        const qreal unitWidth,
        const qreal unitHeight,
        qreal & north,
        qreal & east,
        qreal & south,
        qreal & west)
{
    QRectF reusableUnit;

    const qreal newX = rectangle.left() + unitsGroup[0].x() * unitWidth;
    const qreal newY = rectangle.top() + unitsGroup[0].y() * unitHeight;
    reusableUnit.setX(newX);
    reusableUnit.setY(newY);
    reusableUnit.setWidth(unitWidth);
    reusableUnit.setHeight(unitHeight);

    north = reusableUnit.top();
    east = reusableUnit.right();
    south = reusableUnit.bottom();
    west = reusableUnit.left();

    for (int index = 1; index < unitsGroup.size(); index++)
    {
        const qreal newX = rectangle.left() + unitsGroup[index].x() * unitWidth;
        const qreal newY = rectangle.top() + unitsGroup[index].y() * unitHeight;
        reusableUnit.setX(newX);
        reusableUnit.setY(newY);
        reusableUnit.setWidth(unitWidth);
        reusableUnit.setHeight(unitHeight);

        if (reusableUnit.top() < north)
        {
            north = reusableUnit.top();
        }

        if (reusableUnit.right() > east)
        {
            east = reusableUnit.right();
        }

        if (reusableUnit.bottom() > south)
        {
            south = reusableUnit.bottom();
        }

        if (reusableUnit.left() < west)
        {
            west = reusableUnit.left();
        }
    }
}

}
