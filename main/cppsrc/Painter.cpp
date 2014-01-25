#include "Painter.h"
#include <QPainter>
#include <QDebug>

namespace KovtunMethod
{

Painter::Painter(QQuickItem * parent) :
    QQuickPaintedItem(parent),
    kovtunMethodExecuter(nullptr),
    offsetFromItemEdges(20),
    pen()
{
}

void Painter::paint(QPainter * const painter)
{
    const double scale = calculateScale();
    painter->scale(scale, scale);
    painter->translate(offsetFromItemEdges / scale, offsetFromItemEdges / scale);
    pen.setWidthF(1 / scale);

    if (kovtunMethodExecuter != nullptr)
    {
        drawActiveRectangles(painter);
        drawFilledRectangles(painter);
        drawContour(painter, scale);
    }
}

void Painter::drawContour(QPainter * const painter, const double scale)
{
    pen.setColor("black");
    pen.setWidthF(3 / scale);
    painter->setPen(pen);
    const ClosedContour & contour = kovtunMethodExecuter->getContour();

    if (contour.getPointsCount() > 1)
    {
        painter->drawLines(contour.getLines(), contour.getLinesCount());
    }
    else if (contour.getPointsCount() == 1)
    {
        painter->drawPoint(contour.getPoint(0));
    }
}

void Painter::drawActiveRectangles(QPainter * const painter)
{
    pen.setColor("gray");
    painter->setPen(pen);

    for (int index = 0; index < kovtunMethodExecuter->getActiveRectanglesCount(); index++)
    {
        painter->drawRect(kovtunMethodExecuter->getActiveRectangle(index));
    }
}

void Painter::drawFilledRectangles(QPainter * const painter)
{
    for (int index = 0; index < kovtunMethodExecuter->getFilledRectanglesCount(); index++)
    {
        const MyQRectF & filledRectangle = kovtunMethodExecuter->getFilledRectangle(index);

        const QColor color = filledRectangle.getColor();

        painter->setBrush(color);
        painter->setPen(color);

        painter->drawRect(filledRectangle);
    }
}

void Painter::setKovtunMethodExecuter(const Executer & kovtunMethodExecuter)
{
    this->kovtunMethodExecuter = &kovtunMethodExecuter;
}

double Painter::calculateScale() const
{
    const ClosedContour & contour = kovtunMethodExecuter->getContour();
    if (contour.getPointsCount() > 0)
    {
        const QPointF * pointWithBiggerX = &contour.getPoint(0);
        const QPointF * pointWithBiggerY = &contour.getPoint(0);

        for (int index = 1; index < contour.getPointsCount(); index++)
        {
            const QPointF * currentPoint = &contour.getPoint(index);

            if (currentPoint->x() > pointWithBiggerX->x())
            {
                pointWithBiggerX = currentPoint;
            }

            if (currentPoint->y() > pointWithBiggerY->y())
            {
                pointWithBiggerY = currentPoint;
            }
        }

        const double scaleByX = (width() - offsetFromItemEdges * 2) / pointWithBiggerX->x();
        const double scaleByY = (height() - offsetFromItemEdges * 2) / pointWithBiggerY->y();

        return scaleByX < scaleByY ? scaleByX : scaleByY;
    }

    return 1;
}

}
