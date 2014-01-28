#include "Painter.h"
#include <QPainter>
#include <QDebug>

namespace KovtunMethod
{

Painter::Painter(QQuickItem * parent) :
    QQuickPaintedItem(parent),
    kovtunMethodExecuter(nullptr),
    offsetFromItemEdges(20),
    pen(),
    filledRectanglesLinesAreVisible(false)
{
    pen.setJoinStyle(Qt::MiterJoin);
}

void Painter::paint(QPainter * const painter)
{
    const double scale = calculateScale();
    painter->scale(scale, scale);
    painter->translate(offsetFromItemEdges / scale, offsetFromItemEdges / scale);

    if (kovtunMethodExecuter != nullptr)
    {
        drawActiveRectangles(painter, scale);
        drawGravityCenters(painter, scale);
        drawFilledRectangles(painter, scale);
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

void Painter::drawActiveRectangles(QPainter * const painter, const double scale)
{
    pen.setColor("gray");
    pen.setWidthF(1 / scale);
    painter->setPen(pen);

    for (int index = 0; index < kovtunMethodExecuter->getActiveRectanglesCount(); index++)
    {
        painter->drawRect(kovtunMethodExecuter->getActiveRectangle(index));
    }
}

void Painter::drawGravityCenters(QPainter * const painter, const double scale)
{
    pen.setColor("black");
    pen.setWidthF(2 / scale);
    painter->setPen(pen);

    for (int index = 0; index < kovtunMethodExecuter->getActiveRectanglesCount(); index++)
    {
        const MyQRectF & activeRectangle = kovtunMethodExecuter->getActiveRectangle(index);
        const QPointF * const parentsGravityCenter = activeRectangle.getParentsGravityCenter();
        if (parentsGravityCenter!= nullptr)
        {
            painter->drawPoint(*parentsGravityCenter);
        }
    }
}

void Painter::drawFilledRectangles(QPainter * const painter, const double scale)
{
    if (filledRectanglesLinesAreVisible)
    {
        pen.setColor("grey");
    }
    pen.setWidthF(1 / scale);

    for (int index = 0; index < kovtunMethodExecuter->getFilledRectanglesCount(); index++)
    {
        const MyQRectF & filledRectangle = kovtunMethodExecuter->getFilledRectangle(index);

        const QColor color = filledRectangle.getColor();

        painter->setBrush(color);
        if (!filledRectanglesLinesAreVisible)
        {
            pen.setColor(color);
        }
        painter->setPen(pen);

        painter->drawRect(filledRectangle);
    }
}

void Painter::setKovtunMethodExecuter(const Executer & kovtunMethodExecuter)
{
    this->kovtunMethodExecuter = &kovtunMethodExecuter;
}

void Painter::setFilledRectanglesLinesVisibility(const bool visible)
{
    filledRectanglesLinesAreVisible = visible;
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
