#include "ColorsDictionary.h"
#include <QDebug>
#include <utility>

namespace KovtunMethod
{

ColorsDictionary::ColorsDictionary() :
    alphabet(),
    currentLetter(0)
{
    for (int index = 0; index < ALPHABET_SIZE; index++)
    {
        QColor newColor(qrand() % 200, qrand() % 200, 50 + qrand() % 150);

        while (alphabet.contains(newColor))
        {
            newColor = QColor(qrand() % 200, qrand() % 200, 50 + qrand() % 150);
        }

        alphabet << newColor;
    }
}

const QColor ColorsDictionary::getColorFor(const MyQRectF & rectangle)
{
    QColor neighborColor;
    const bool anyNeighborIsColored = getAnyNeighborColor(rectangle, neighborColor);

    if (anyNeighborIsColored)
    {
        return neighborColor;
    }
    else
    {
        if (currentLetter >= alphabet.size())
        {
            currentLetter = 0;
        }
        return alphabet[currentLetter++];
    }
}

const QColor ColorsDictionary::getColorFor(const QVector<QSharedPointer<MyQRectF> > & rectangles)
{
    QColor neighborColor;

    for (const auto & rectangle : rectangles)
    {
        const bool anyNeighborIsColored = getAnyNeighborColor(*rectangle, neighborColor);

        if (anyNeighborIsColored)
        {
            return neighborColor;
        }
    }

    if (currentLetter >= alphabet.size())
    {
        currentLetter = 0;
    }
    return alphabet[currentLetter++];
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
