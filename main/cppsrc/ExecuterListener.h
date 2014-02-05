#ifndef KOVTUNMETHODEXECUTERLISTENER_H
#define KOVTUNMETHODEXECUTERLISTENER_H

#include <QPointF>
#include <QColor>
#include "MyQRectF.h"

namespace KovtunMethod
{

class ExecuterListener
{
    friend class Executer;

protected:
    virtual void onStepStarted() = 0;
    virtual void onActiveRectangleProcessed() = 0;
    virtual void onGravityCenterCalculated(const QPointF & gravityCenter, const double error, const MyQRectF & rectangle) = 0;
    virtual void onColorGathered(const QColor & color, const MyQRectF & rectangle) = 0;
    virtual void onStepFinished() = 0;
    virtual void onReset() = 0;
};

}

#endif // KOVTUNMETHODEXECUTERLISTENER_H
