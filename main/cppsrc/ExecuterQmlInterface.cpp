#include "ExecuterQmlInterface.h"
#include <QtConcurrent/QtConcurrent>
#include <QVector>

namespace KovtunMethod
{

ExecuterQmlInterface::ExecuterQmlInterface(QObject * parent) :
    QObject(parent),
    kovtunMethodExecuter(nullptr),
    inProgress(false),
    futureWatcher()
{
    QObject::connect(&futureWatcher, &QFutureWatcher<void>::finished,
                     this, &ExecuterQmlInterface::onStepPerformed);
}

void ExecuterQmlInterface::setKovtunMethodExecuter(Executer & kovtunMethodExecuter)
{
    this->kovtunMethodExecuter = &kovtunMethodExecuter;
}

void ExecuterQmlInterface::performNextStep()
{
    if (kovtunMethodExecuter != nullptr && !inProgress)
    {
        inProgress = true;
        futureWatcher.setFuture(QtConcurrent::run(this, &ExecuterQmlInterface::performNextStepNotConcurrently));
    }
}

void ExecuterQmlInterface::reset()
{
    if (kovtunMethodExecuter != nullptr && !inProgress)
    {
        kovtunMethodExecuter->reset();

        emit executionReset();
    }
}

void ExecuterQmlInterface::setUnitsDimension(const int dimension) const
{
    if (kovtunMethodExecuter != nullptr && !inProgress)
    {
        kovtunMethodExecuter->setUnitsDimension(dimension);
    }
}

int ExecuterQmlInterface::getUnitsDimension() const
{
    if (kovtunMethodExecuter != nullptr && !inProgress)
    {
        return kovtunMethodExecuter->getUnitsDimension();
    }
    else
    {
        return Executer::defaultUnitDimension;
    }
}

double ExecuterQmlInterface::getCurrentError() const
{
    if (kovtunMethodExecuter != nullptr)
    {
        return kovtunMethodExecuter->getCurrentError();
    }
    else
    {
        return -1;
    }
}

double ExecuterQmlInterface::getActiveRectanglesCount() const
{
    if (kovtunMethodExecuter != nullptr)
    {
        return kovtunMethodExecuter->getActiveRectanglesCount();
    }
    else
    {
        return -1;
    }
}

double ExecuterQmlInterface::getFilledRectanglesCount() const
{
    if (kovtunMethodExecuter != nullptr)
    {
        return kovtunMethodExecuter->getFilledRectanglesCount();
    }
    else
    {
        return -1;
    }
}

int ExecuterQmlInterface::getUsedColorsCount() const
{
    if (kovtunMethodExecuter != nullptr)
    {
        QVector<QColor> usedColors;

        for (int index = 0; index < kovtunMethodExecuter->getFilledRectanglesCount(); index++)
        {
            const MyQRectF & currentFilledRectangle = kovtunMethodExecuter->getFilledRectangle(index);
            const QColor & currentColor = currentFilledRectangle.getColor();

            if (!usedColors.contains(currentColor))
            {
                usedColors.push_back(currentColor);
            }
        }

        return usedColors.size();
    }
    else
    {
        return -1;
    }
}

void ExecuterQmlInterface::onStepPerformed()
{
    inProgress = false;
    emit stepPerformed();
}

void ExecuterQmlInterface::performNextStepNotConcurrently()
{
    if (kovtunMethodExecuter != nullptr)
    {
        kovtunMethodExecuter->performNextStep();
    }
}

}
