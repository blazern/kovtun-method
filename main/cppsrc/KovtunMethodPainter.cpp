#include "KovtunMethodPainter.h"
#include <QPainter>
#include <QDebug>

KovtunMethodPainter::KovtunMethodPainter(QQuickItem * parent) :
    QQuickPaintedItem(parent),
    kovtunMethodExecuter(nullptr),
    offsetFromItemEdges(20),
    pen()
{
}

void KovtunMethodPainter::paint(QPainter * const painter)
{
    const double scale = calculateScale();
    painter->scale(scale, scale);
    painter->translate(offsetFromItemEdges / scale, offsetFromItemEdges / scale);
    pen.setWidth(1 / scale);

    if (kovtunMethodExecuter != nullptr)
    {
        drawActiveRectangles(painter);
        drawFilledRectangles(painter);
        drawContour(painter);
    }
}

void KovtunMethodPainter::drawContour(QPainter * const painter)
{
    pen.setColor("black");
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

void KovtunMethodPainter::drawActiveRectangles(QPainter * const painter)
{
    pen.setColor("gray");
    painter->setPen(pen);

    for (int index = 0; index < kovtunMethodExecuter->getActiveRectanglesCount(); index++)
    {
        painter->drawRect(kovtunMethodExecuter->getActiveRectangle(index));
    }
}

void KovtunMethodPainter::drawFilledRectangles(QPainter * const painter)
{
//    pen.setColor("green");
//    painter->setPen(pen);
//    painter->setBrush(QColor("green"));
    for (int index = 0; index < kovtunMethodExecuter->getFilledRectanglesCount(); index++)
    {
        const KovtunQRectF & filledRectangle = kovtunMethodExecuter->getFilledRectangle(index);

        const QColor color = filledRectangle.getColor();

        painter->setBrush(color);
        painter->setPen(color);

        painter->drawRect(filledRectangle);
    }
}

void KovtunMethodPainter::setKovtunMethodExecuter(const KovtunMethodExecuter & kovtunMethodExecuter)
{
    this->kovtunMethodExecuter = &kovtunMethodExecuter;
}

double KovtunMethodPainter::calculateScale() const
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
