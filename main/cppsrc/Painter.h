#ifndef KOVTUNMETHODPAINTER_H
#define KOVTUNMETHODPAINTER_H

#include <QQuickPaintedItem>
#include <QPen>
#include "Executer.h"

namespace KovtunMethod
{

class Painter : public QQuickPaintedItem
{
    Q_OBJECT

public:
    explicit Painter(QQuickItem * parent = 0);

    virtual void paint(QPainter * const painter) final override;

    void setKovtunMethodExecuter(const Executer & kovtunMethodExecuter);

    Q_INVOKABLE void setFilledRectanglesLinesVisibility(const bool visible);
    Q_INVOKABLE void setGravityCentersVisibility(const bool visible);

private:
    const Executer * kovtunMethodExecuter;
    const int offsetFromItemEdges;   // Отступ от краёв области рисования
    QPen pen;
    bool filledRectanglesLinesAreVisible;
    bool gravityCentersAreVisible;

    double calculateScale() const;
    void drawContour(QPainter * const painter, const double scale);
    void drawActiveRectangles(QPainter * const painter, const double scale);
    void drawGravityCenters(QPainter * const painter, const double scale);
    void drawFilledRectangles(QPainter * const painter, const double scale);
};

}

#endif // KOVTUNMETHODPAINTER_H
