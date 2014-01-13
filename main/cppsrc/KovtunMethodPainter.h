#ifndef KOVTUNMETHODPAINTER_H
#define KOVTUNMETHODPAINTER_H

#include <QQuickPaintedItem>
#include "KovtunMethodExecuter.h"

class KovtunMethodPainter : public QQuickPaintedItem
{
    Q_OBJECT

public:
    explicit KovtunMethodPainter(QQuickItem * parent = 0);

    virtual void paint(QPainter * const painter) final override;

    void setKovtunMethodExecuter(const KovtunMethodExecuter & kovtunMethodExecuter);

private:
    const KovtunMethodExecuter * kovtunMethodExecuter;
    const int offset;   // Отступ от краёв области рисования

    double calculateScale() const;
    void drawContour(QPainter * const painter);
    void drawActiveRectangles(QPainter * const painter);
};

#endif // KOVTUNMETHODPAINTER_H
