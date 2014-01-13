#include "KovtunMethodPainter.h"
#include <QPainter>
#include <QDebug>

KovtunMethodPainter::KovtunMethodPainter(QQuickItem * parent) :
    QQuickPaintedItem(parent),
    kovtunMethodExecuter(nullptr),
    offset(20)
{
}

void KovtunMethodPainter::paint(QPainter * const painter)
{
    const double scale = calculateScale();
    painter->scale(scale, scale);
    painter->translate(scale, scale);

    if (kovtunMethodExecuter != nullptr)
    {
        drawContour(painter);
        drawActiveRectangles(painter);
    }
}

void KovtunMethodPainter::drawContour(QPainter * const painter)
{
    const Contour & contour = kovtunMethodExecuter->getContour();

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
    painter->setPen(QColor("blue"));
    painter->drawRects(kovtunMethodExecuter->getActiveRectangles(), kovtunMethodExecuter->getActiveRectanglesCount());
}

void KovtunMethodPainter::setKovtunMethodExecuter(const KovtunMethodExecuter & kovtunMethodExecuter)
{
    this->kovtunMethodExecuter = &kovtunMethodExecuter;
}

double KovtunMethodPainter::calculateScale() const
{
    const Contour & contour = kovtunMethodExecuter->getContour();
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

        const double scaleByX = (width() - offset * 2) / pointWithBiggerX->x();
        const double scaleByY = (height() - offset * 2) / pointWithBiggerY->y();

        return scaleByX < scaleByY ? scaleByX : scaleByY;
    }

    return 1;
}
