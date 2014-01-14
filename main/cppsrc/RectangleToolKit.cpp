#include "RectangleToolKit.h"

QPointF RectangleToolKit::calculateGravityCenter(const Contour & contour, const QRectF & rectangle)
{
    return rectangle.center();
}
