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

private:
    const Executer * kovtunMethodExecuter;
    const int offsetFromItemEdges;   // Отступ от краёв области рисования
    QPen pen;

    double calculateScale() const;
    void drawContour(QPainter * const painter, const double scale);
    void drawActiveRectangles(QPainter * const painter);
    void drawFilledRectangles(QPainter * const painter);
};

}

#endif // KOVTUNMETHODPAINTER_H
