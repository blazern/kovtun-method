#include "KovtunMethodExecuterQmlInterface.h"
#include <QtConcurrent/QtConcurrent>

KovtunMethodExecuterQmlInterface::KovtunMethodExecuterQmlInterface(QObject * parent) :
    QObject(parent),
    kovtunMethodExecuter(nullptr),
    inProgress(false),
    futureWatcher()
{
    QObject::connect(&futureWatcher, &QFutureWatcher<void>::finished,
                     this, &KovtunMethodExecuterQmlInterface::onStepPerformed);
}

void KovtunMethodExecuterQmlInterface::setKovtunMethodExecuter(KovtunMethodExecuter & kovtunMethodExecuter)
{
    this->kovtunMethodExecuter = &kovtunMethodExecuter;
}

void KovtunMethodExecuterQmlInterface::performNextStep()
{
    if (kovtunMethodExecuter != nullptr && !inProgress)
    {
        inProgress = true;
        futureWatcher.setFuture(QtConcurrent::run(this, &KovtunMethodExecuterQmlInterface::performNextStepNotConcurrently));
    }
}

void KovtunMethodExecuterQmlInterface::reset()
{
    if (kovtunMethodExecuter != nullptr && !inProgress)
    {
        kovtunMethodExecuter->reset();

        emit executionReset();
    }
}

void KovtunMethodExecuterQmlInterface::setUnitsDimension(const int dimension) const
{
    if (kovtunMethodExecuter != nullptr && !inProgress)
    {
        kovtunMethodExecuter->setUnitsDimension(dimension);
    }
}

int KovtunMethodExecuterQmlInterface::getUnitsDimension() const
{
    if (kovtunMethodExecuter != nullptr && !inProgress)
    {
        return kovtunMethodExecuter->getUnitsDimension();
    }
    else
    {
        return KovtunMethodExecuter::defaultUnitDimension;
    }
}

void KovtunMethodExecuterQmlInterface::onStepPerformed()
{
    inProgress = false;
    emit stepPerformed();
}

void KovtunMethodExecuterQmlInterface::performNextStepNotConcurrently()
{
    if (kovtunMethodExecuter != nullptr)
    {
        kovtunMethodExecuter->performNextStep();
    }
}
