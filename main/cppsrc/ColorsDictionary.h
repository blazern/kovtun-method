#ifndef COLORSDICTIONARY_H
#define COLORSDICTIONARY_H

#include <QColor>
#include <QVector>
#include "MyQRectF.h"

namespace KovtunMethod
{

class ColorsDictionary
{
public:
    explicit ColorsDictionary();
    ColorsDictionary(const ColorsDictionary &) = delete;
    ColorsDictionary & operator=(const ColorsDictionary &) = delete;

    const QColor getColorFor(const MyQRectF & rectangle);

private:
    static const int COLOR_MAX = 200;
    static const int ALPHABET_SIZE = 50;
    QVector<QColor> alphabet;
    int currentLetter;

    bool getAnyNeighborColor(const MyQRectF & rectangle, QColor & color) const;
};

}

#endif // COLORSDICTIONARY_H
