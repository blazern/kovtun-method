#ifndef KOVTUNMETHODEXECUTERFILELOGGER_H
#define KOVTUNMETHODEXECUTERFILELOGGER_H

#include "ExecuterListener.h"

namespace KovtunMethod
{

class ExecuterFileLogger : public ExecuterListener
{
public:
    explicit ExecuterFileLogger();

protected:
    virtual void onActiveRectangleProcessed() final override {}

    virtual void onStepStarted() final override {}

    virtual void onGravityCenterCalculated(const QPointF & gravityCenter, const double error, const MyQRectF & rectangle) final override;

    virtual void onColorGathered(const QColor & color, const MyQRectF & rectangle) final override;

    virtual void onStepFinished() final override;

    virtual void onReset() final override;

private:
    int stepIndex;
    QString output;
};

}

#endif // KOVTUNMETHODEXECUTERFILELOGGER_H
