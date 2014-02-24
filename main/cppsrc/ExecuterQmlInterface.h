#ifndef KOVTUNMETHODEXECUTERQMLINTERFACE_H
#define KOVTUNMETHODEXECUTERQMLINTERFACE_H

#include <QObject>
#include <QFutureWatcher>
#include "Executer.h"
#include "Painter.h"

namespace KovtunMethod
{

class ExecuterQmlInterface : public QObject
{
    Q_OBJECT
public:
    explicit ExecuterQmlInterface(QObject * parent = 0);

    void setKovtunMethodExecuter(Executer & kovtunMethodExecuter);

    Q_INVOKABLE void performNextStep();
    Q_INVOKABLE void reset();
    Q_INVOKABLE void setUnitsDimension(const int dimension) const;
    Q_INVOKABLE int getUnitsDimension() const;
    Q_INVOKABLE double getCurrentError() const;
    Q_INVOKABLE double getActiveRectanglesCount() const;
    Q_INVOKABLE double getFilledRectanglesCount() const;
    Q_INVOKABLE int getUsedColorsCount() const;

    Q_INVOKABLE void setNeighborMethodExecution();
    Q_INVOKABLE void setFractalMethodEXecution();
    Q_INVOKABLE bool isNeighborMethodSet() const;

signals:
    void stepStartedPerforming();
    void stepPerformed();
    void executionReset();

private slots:
    void onStepPerformed();

private:
    Executer * kovtunMethodExecuter;

    bool inProgress;

    QFutureWatcher<void> futureWatcher;

    void performNextStepNotConcurrently();
};

}

#endif // KOVTUNMETHODEXECUTERQMLINTERFACE_H
