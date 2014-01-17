#ifndef KOVTUNMETHODEXECUTERQMLINTERFACE_H
#define KOVTUNMETHODEXECUTERQMLINTERFACE_H

#include <QObject>
#include "KovtunMethodExecuter.h"
#include "KovtunMethodPainter.h"

class KovtunMethodExecuterQmlInterface : public QObject
{
    Q_OBJECT
public:
    explicit KovtunMethodExecuterQmlInterface(
            KovtunMethodExecuter & kovtunMethodExecuter,
            QObject * parent = 0);

    void setKovtunMethodPainter(KovtunMethodPainter & kovtunMethodPainter);

    Q_INVOKABLE void performNextStep();
    Q_INVOKABLE void reset();
    Q_INVOKABLE void setUnitsDimension(const int dimension);
    Q_INVOKABLE int getUnitsDimension();

private:
    KovtunMethodExecuter & kovtunMethodExecuter;
    KovtunMethodPainter * kovtunMethodPainter;
};

#endif // KOVTUNMETHODEXECUTERQMLINTERFACE_H
