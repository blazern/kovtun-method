#include "KovtunMethodExecuterQmlInterface.h"

KovtunMethodExecuterQmlInterface::KovtunMethodExecuterQmlInterface(KovtunMethodExecuter & kovtunMethodExecuter,
        QObject * parent) :
    QObject(parent),
    kovtunMethodExecuter(kovtunMethodExecuter),
    kovtunMethodPainter(nullptr)
{
}

void KovtunMethodExecuterQmlInterface::setKovtunMethodPainter(KovtunMethodPainter * const kovtunMethodPainter)
{
    this->kovtunMethodPainter = kovtunMethodPainter;
}

void KovtunMethodExecuterQmlInterface::performNextStep()
{
    kovtunMethodExecuter.performNextStep();
    if (kovtunMethodPainter != nullptr)
    {
        kovtunMethodPainter->update();
    }
}
