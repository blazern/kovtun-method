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
        const int newRed = neighborColor.red() + 20;

        if (newRed <= 255)
        {
            return QColor(newRed, neighborColor.green(), neighborColor.blue());
        }
        else
        {
            const int newGreen = neighborColor.green() + 20;

            if (newGreen <= 255)
            {
                return QColor(neighborColor.red(), newGreen, neighborColor.blue());
            }
            else
            {
                return neighborColor;
            }
        }
    }
    else
    {
        return QColor(qrand() % 255, qrand() % 255, qrand() % 255);
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
