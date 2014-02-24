#ifndef APPLICATIONWRAPPER_H
#define APPLICATIONWRAPPER_H

#include <QObject>
#include "qtquick2controlsapplicationviewer.h"
#include "cppsrc/Painter.h"
#include "cppsrc/Executer.h"
#include "cppsrc/ExecuterQmlInterface.h"
#include "cppsrc/ExecuterFileLogger.h"
#include "cppsrc/ExecuterProgressWatcher.h"
#include "cppsrc/ClosedContour.h"
#include "cppsrc/ClosedContourSetterQmlInterface.h"

class ApplicationWrapper : public QObject
{
    Q_OBJECT
public:
    explicit ApplicationWrapper(
            QtQuick2ControlsApplicationViewer & applicationViewer,
            QObject * parent = 0);
    ~ApplicationWrapper();

private slots:
    void onContourPointed(const KovtunMethod::ClosedContour & contour);

private:
    static const QString defaultContourJson;

    KovtunMethod::Executer * kovtunMethodExecuter;
    KovtunMethod::ExecuterFileLogger fileLogger;
    KovtunMethod::ExecuterProgressWatcher progressWatcher;
    KovtunMethod::Painter * kovtunMethodPainter;
    KovtunMethod::ExecuterQmlInterface * kovtunMethodQmlInterface;
    KovtunMethod::ClosedContourSetterQmlInterface * closedContourSetterQmlInterface;

    void resetExecuter();
};

#endif // APPLICATIONWRAPPER_H
