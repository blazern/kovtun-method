#ifndef KOVTUNMETHODEXECUTERQMLINTERFACE_H
#define KOVTUNMETHODEXECUTERQMLINTERFACE_H

#include <QObject>
#include <QFutureWatcher>
#include "KovtunMethodExecuter.h"
#include "KovtunMethodPainter.h"

class KovtunMethodExecuterQmlInterface : public QObject
{
    Q_OBJECT
public:
    explicit KovtunMethodExecuterQmlInterface(QObject * parent = 0);

    void setKovtunMethodExecuter(KovtunMethodExecuter & kovtunMethodExecuter);

    Q_INVOKABLE void performNextStep();
    Q_INVOKABLE void reset();
    Q_INVOKABLE void setUnitsDimension(const int dimension) const;
    Q_INVOKABLE int getUnitsDimension() const;

signals:
    void stepPerformed();
    void executionReset();

private slots:
    void onStepPerformed();

private:
    KovtunMethodExecuter * kovtunMethodExecuter;

    bool inProgress;

    QFutureWatcher<void> futureWatcher;

    void performNextStepNotConcurrently();
};

#endif // KOVTUNMETHODEXECUTERQMLINTERFACE_H
