#ifndef COLORSDICTIONARY_H
#define COLORSDICTIONARY_H

#include <QColor>
#include "MyQRectF.h"

namespace KovtunMethod
{

class ColorsDictionary
{
public:
    explicit ColorsDictionary();
    ColorsDictionary(const ColorsDictionary &) = delete;
    ColorsDictionary & operator=(const ColorsDictionary &) = delete;

    const QColor getColorFor(const MyQRectF & rectangle) const;

private:
    bool getAnyNeighborColor(const MyQRectF & rectangle, QColor & color) const;
};

}

#endif // COLORSDICTIONARY_H
