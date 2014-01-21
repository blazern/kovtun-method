#include "ColorsDictionary.h"
#include <QDebug>

ColorsDictionary::ColorsDictionary()
{
}

const QColor ColorsDictionary::getColorFor(const KovtunQRectF & rectangle) const
{
    QColor neighborColor;
    const bool anyNeighborIsColored = getAnyNeighborColor(rectangle, neighborColor);

    if (anyNeighborIsColored)
    {
        return neighborColor;
    }
    else
    {
        return QColor(0, qrand() % 255, qrand() % 255);
    }
}

bool ColorsDictionary::getAnyNeighborColor(const KovtunQRectF & rectangle, QColor & color) const
{
    for (KovtunQRectF::neighborsIterator iterator = rectangle.neighborsBegin();
         iterator != rectangle.neighborsEnd();
         iterator++)
    {
        const KovtunQRectF & neighbor = **iterator;

        if (neighbor.isColorInitialized())
        {
            color = neighbor.getColor();
            return true;
        }
    }

    return false;
}
