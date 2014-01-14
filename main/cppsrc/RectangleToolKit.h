#ifndef RECTANGLETOOLKIT_H
#define RECTANGLETOOLKIT_H

#include <QRectF>
#include "Contour.h"

class RectangleToolKit
{
    RectangleToolKit() = delete;
    ~RectangleToolKit() = delete;
    RectangleToolKit(const RectangleToolKit &) = delete;
    RectangleToolKit & operator=(const RectangleToolKit &) = delete;

public:
    static QPointF calculateGravityCenter(const Contour & contour, const QRectF & rectangle);

    static bool isAnyPointOfRectangleInsideOfContour(const Contour & contour, const QRectF & rectangle);
};

#endif // RECTANGLETOOLKIT_H
