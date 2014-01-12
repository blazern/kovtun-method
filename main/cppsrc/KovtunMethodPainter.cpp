#include "KovtunMethodPainter.h"
#include <QPainter>
#include <QDebug>

KovtunMethodPainter::KovtunMethodPainter(QQuickItem * parent) :
    QQuickPaintedItem(parent),
    kovtunMethodExecuter(nullptr)
{
}

void KovtunMethodPainter::paint(QPainter * const painter)
{
    const double scale = calculateScale();

    if (kovtunMethodExecuter != nullptr)
    {
        drawContour(painter, scale);
    }
}

void KovtunMethodPainter::drawContour(QPainter * const painter, const double scale)
{
    const Contour & contour = kovtunMethodExecuter->getContour();

    if (contour.getPointsCount() > 1)
    {
        const QPointF * previousPoint = &contour.getPoint(0);

        for (int index = 1; index < contour.getPointsCount(); index++)
        {
            const QPointF * currentPoint = &contour.getPoint(index);

            painter->drawLine(previousPoint->x() * scale, previousPoint->y() * scale, currentPoint->x() * scale, currentPoint->y() * scale);
            previousPoint = currentPoint;
        }
    }
    else if (contour.getPointsCount() == 1)
    {
        painter->drawPoint(contour.getPoint(0));
    }
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
        double biggestX = contour.getPoint(0).x();

        for (int index = 1; index < contour.getPointsCount(); index++)
        {
            const double currentX = contour.getPoint(index).x();
            if (currentX > biggestX)
            {
                biggestX = currentX;
            }
        }

        double biggestY = contour.getPoint(0).y();

        for (int index = 1; index < contour.getPointsCount(); index++)
        {
            const double currentY = contour.getPoint(index).y();
            if (currentY > biggestY)
            {
                biggestY = currentY;
            }
        }

        if (biggestX > biggestY)
        {
            return (width() / biggestX);
        }
        else
        {
            return (height() / biggestY);
        }
    }

    return 1;
}
