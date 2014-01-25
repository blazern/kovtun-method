#include "ColorsDictionary.h"
#include <QDebug>

namespace KovtunMethod
{

ColorsDictionary::ColorsDictionary()
{
}

const QColor ColorsDictionary::getColorFor(const MyQRectF & rectangle) const
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

bool ColorsDictionary::getAnyNeighborColor(const MyQRectF & rectangle, QColor & color) const
{
    for (const auto & neighbor : rectangle.getNeighborhood())
    {
        if (neighbor->isColorInitialized())
        {
            color = neighbor->getColor();
            return true;
        }
    }

    return false;
}

}
