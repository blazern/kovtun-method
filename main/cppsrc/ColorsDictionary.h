#ifndef COLORSDICTIONARY_H
#define COLORSDICTIONARY_H

#include <QColor>
#include "KovtunQRectF.h"

class ColorsDictionary
{
public:
    explicit ColorsDictionary();
    ColorsDictionary(const ColorsDictionary &) = delete;
    ColorsDictionary & operator=(const ColorsDictionary &) = delete;

    const QColor getColorFor(const KovtunQRectF & rectangle) const;

private:
    bool getAnyNeighborColor(const KovtunQRectF & rectangle, QColor & color) const;
};

#endif // COLORSDICTIONARY_H
