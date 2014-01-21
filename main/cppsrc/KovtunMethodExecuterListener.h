#ifndef KOVTUNMETHODEXECUTERLISTENER_H
#define KOVTUNMETHODEXECUTERLISTENER_H

#include <QPointF>
#include <QColor>
#include "KovtunQRectF.h"

class KovtunMethodExecuterListener
{
    friend class KovtunMethodExecuter;

protected:
    virtual void onGravityCenterCalculated(const QPointF & gravityCenter, const KovtunQRectF & rectangle) = 0;
    virtual void onColorGathered(const QColor & color, const KovtunQRectF & rectangle) = 0;
    virtual void onStepFinished() = 0;
    virtual void onReset() = 0;
};

#endif // KOVTUNMETHODEXECUTERLISTENER_H
