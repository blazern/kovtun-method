#ifndef KOVTUNMETHODPAINTER_H
#define KOVTUNMETHODPAINTER_H

#include <QQuickPaintedItem>
#include "KovtunMethodExectuter.h"

class KovtunMethodPainter : public QQuickPaintedItem
{
    Q_OBJECT

public:
    explicit KovtunMethodPainter(QQuickItem * parent = 0);

    virtual void paint(QPainter * const painter) final override;

    void setKovtunMethodExecuter(const KovtunMethodExecuter & kovtunMethodExecuter);

private:
    const KovtunMethodExecuter * kovtunMethodExecuter;

    double calculateScale() const;
    void drawContour(QPainter * const painter, const double scale);
};

#endif // KOVTUNMETHODPAINTER_H
