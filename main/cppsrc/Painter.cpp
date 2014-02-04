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
    filledRectanglesLinesAreVisible(false),
    gravityCentersAreVisible(false)
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
        drawFilledRectangles(painter, scale);
        if (gravityCentersAreVisible)
        {
            drawGravityCenters(painter, scale);
        }
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
//    pen.setColor("gray");
//    pen.setWidthF(1 / scale);
//    painter->setPen(pen);

//    const auto unitsDimension = kovtunMethodExecuter->getUnitsDimension();

//    for (int index = 0; index < kovtunMethodExecuter->getActiveRectanglesCount(); index++)
//    {
//        pen.setColor("gray");
//        painter->setPen(pen);
//        auto width = kovtunMethodExecuter->getActiveRectangle(index).width() / unitsDimension;
//        auto height = kovtunMethodExecuter->getActiveRectangle(index).height() / unitsDimension;

//        for (auto horIndex = 0; horIndex < unitsDimension; horIndex++)
//        {
//            for (auto verIndex = 0; verIndex < unitsDimension; verIndex++)
//            {
//                const auto & active = kovtunMethodExecuter->getActiveRectangle(index);
//                QRectF atata(active.left() + width * horIndex, active.top() + height * verIndex, width, height);
//                painter->drawRect(atata);
//            }
//        }

//        pen.setColor("red");
//        painter->setPen(pen);
//        painter->drawRect(kovtunMethodExecuter->getActiveRectangle(index));
//    }

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
    pen.setWidthF(5 / scale);
    for (int index = 0; index < kovtunMethodExecuter->getActiveRectanglesCount(); index++)
    {
        const MyQRectF & activeRectangle = kovtunMethodExecuter->getActiveRectangle(index);
        const QPointF * const parentsGravityCenter = activeRectangle.getParentsGravityCenter();
        if (parentsGravityCenter!= nullptr)
        {
            pen.setColor("black");
            painter->setPen(pen);
            painter->drawPoint(*parentsGravityCenter);
        }

        const QPointF * const grandParentGravityCenter = activeRectangle.getGrandParentsGravityCenter();
        if (grandParentGravityCenter!= nullptr)
        {
            pen.setColor("red");
            painter->setPen(pen);
            painter->drawPoint(*grandParentGravityCenter);
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

void Painter::setGravityCentersVisibility(const bool visible)
{
    gravityCentersAreVisible = visible;
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
