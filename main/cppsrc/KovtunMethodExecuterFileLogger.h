#ifndef KOVTUNMETHODEXECUTERFILELOGGER_H
#define KOVTUNMETHODEXECUTERFILELOGGER_H

#include "KovtunMethodExecuterListener.h"

class KovtunMethodExecuterFileLogger : public KovtunMethodExecuterListener
{
public:
    explicit KovtunMethodExecuterFileLogger();

    virtual void onGravityCenterCalculated(const QPointF & gravityCenter, const double error, const KovtunQRectF & rectangle) final override;

    virtual void onColorGathered(const QColor & color, const KovtunQRectF & rectangle) final override;

    virtual void onStepFinished() final override;

    virtual void onReset() final override;

private:
    int stepIndex;
    QString output;
};

#endif // KOVTUNMETHODEXECUTERFILELOGGER_H
