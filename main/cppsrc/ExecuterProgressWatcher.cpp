#include "ExecuterProgressWatcher.h"

namespace KovtunMethod {

ExecuterProgressWatcher::ExecuterProgressWatcher(const Executer & executer, QObject *parent) :
    QObject(parent),
    executer(executer),
    lastProgress(0),
    lastActiveRectangleIndex(0)
{
}

void ExecuterProgressWatcher::onStepStarted()
{
    lastProgress = 0;
    lastActiveRectangleIndex = 0;
}

void ExecuterProgressWatcher::onActiveRectangleProcessed()
{
    lastProgress = (double) lastActiveRectangleIndex / executer.getCurrentActiveRectanglesCount();
    lastActiveRectangleIndex++;

    emit progressChanged();
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
