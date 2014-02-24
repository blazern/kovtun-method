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
    ExecuterProgressWatcher(const ExecuterProgressWatcher &) = delete;
    ExecuterProgressWatcher & operator=(const ExecuterProgressWatcher &) = delete;
    explicit ExecuterProgressWatcher(QObject * parent = 0);

    void setKovtunMethodExecuter(const Executer & executer);

    Q_INVOKABLE double getProgress() const { return lastProgress; }

    virtual void onStepStarted() final override;

    virtual void onActiveRectangleProcessed() final override;

    virtual void onGravityCenterCalculated(const QPointF &, const double, const MyQRectF &) final override {}

    virtual void onColorGathered(const QColor &, const MyQRectF &) final override {}

    virtual void onStepFinished() final override;

    virtual void onReset() final override;

signals:
    void progressChanged();

private:
    const Executer * executer;
    double lastProgress;
    int lastActiveRectangleIndex;

};

}

#endif // KOVTUNEXECUTERPROGRESSWATCHER_H
