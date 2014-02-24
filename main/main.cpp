#include "qtquick2controlsapplicationviewer.h"
#include "cppsrc/Painter.h"
#include "cppsrc/Executer.h"
#include "cppsrc/ExecuterQmlInterface.h"
#include "cppsrc/ExecuterFileLogger.h"
#include "cppsrc/ExecuterProgressWatcher.h"
#include "cppsrc/ClosedContourParser.h"
#include "cppsrc/ClosedContourSetterQmlInterface.h"
#include "ApplicationWrapper.h"
#include <QtQml>
#include <QTime>
#include <stdexcept>

using namespace KovtunMethod;

int main(int argc, char *argv[])
{
    qsrand(QTime::currentTime().msec());

    qmlRegisterType<Painter>("KovtunMethod", 1, 0, "KovtunMethodPainter");
    qmlRegisterType<ExecuterQmlInterface>("KovtunMethod", 1, 0, "KovtunMethodExecuterQmlInterface");
    qmlRegisterType<ClosedContourSetterQmlInterface>("KovtunMethod", 1, 0, "ClosedContourSetterQmlInterface");

    Application app(argc, argv);

    QtQuick2ControlsApplicationViewer viewer;

    ApplicationWrapper appWrapper(viewer);

    viewer.show();

    return app.exec();
}
