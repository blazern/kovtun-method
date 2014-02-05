#ifndef KOVTUNEXECUTERPROGRESSWATCHER_H
#define KOVTUNEXECUTERPROGRESSWATCHER_H

#include <QObject>
#include "ExecuterListener.h"
#include "Executer.h"

namespace KovtunMethod {

class ExecuterProgressWatcher : public QObject, public ExecuterListener
{
    Q_OBJECT
    Q_PROPERTY(double progress READ getProgress NOTIFY progressChanged)

public:
    explicit ExecuterProgressWatcher(const Executer & executer, QObject * parent = 0);

    Q_INVOKABLE double getProgress() const { return lastProgress; }

signals:
    void progressChanged();

protected:
    virtual void onStepStarted() final override;

    virtual void onActiveRectangleProcessed() final override;

    virtual void onGravityCenterCalculated(const QPointF &, const double, const MyQRectF &) final override {}

    virtual void onColorGathered(const QColor &, const MyQRectF &) final override {}

    virtual void onStepFinished() final override;

    virtual void onReset() final override;

private:
    const Executer & executer;
    double lastProgress;
    int lastActiveRectangleIndex;

};

}

#endif // KOVTUNEXECUTERPROGRESSWATCHER_H
