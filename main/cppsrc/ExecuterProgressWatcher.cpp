#include "ExecuterProgressWatcher.h"

namespace KovtunMethod {

ExecuterProgressWatcher::ExecuterProgressWatcher(QObject *parent) :
    QObject(parent),
    executer(nullptr),
    lastProgress(0),
    lastActiveRectangleIndex(0)
{
}

void ExecuterProgressWatcher::setKovtunMethodExecuter(const Executer & executer)
{
    this->executer = &executer;
}

void ExecuterProgressWatcher::onStepStarted()
{
    lastProgress = 0;
    lastActiveRectangleIndex = 0;
}

void ExecuterProgressWatcher::onActiveRectangleProcessed()
{
    if (executer != nullptr)
    {
        lastProgress = (double) lastActiveRectangleIndex / executer->getCurrentActiveRectanglesCount();
        lastActiveRectangleIndex++;

        emit progressChanged();
    }
}

void ExecuterProgressWatcher::onStepFinished()
{
    lastProgress = 1;
    emit progressChanged();
}

void ExecuterProgressWatcher::onReset()
{
    onStepStarted();
}

}
