#include "qtquick2controlsapplicationviewer.h"
#include "cppsrc/KovtunMethodPainter.h"
#include "cppsrc/KovtunMethodExecuter.h"
#include "cppsrc/KovtunMethodExecuterQmlInterface.h"
#include "cppsrc/KovtunMethodExecuterFileLogger.h"
#include <QtQml>

int main(int argc, char *argv[])
{
    qmlRegisterType<KovtunMethodPainter>("KovtunMethod", 1, 0, "KovtunMethodPainter");
    qmlRegisterType<KovtunMethodExecuterQmlInterface>("KovtunMethod", 1, 0, "KovtunMethodExecuterQmlInterface");

    Application app(argc, argv);

    ClosedContour contour;

    contour.addPoint(QPointF(0,0));
    contour.addPoint(QPointF(100,0));
    contour.addPoint(QPointF(100,40));
    contour.addPoint(QPointF(105,40));
    contour.addPoint(QPointF(105,30));
    contour.addPoint(QPointF(155,30));
    contour.addPoint(QPointF(155,50));
    contour.addPoint(QPointF(165,50));
    contour.addPoint(QPointF(165,10));
    contour.addPoint(QPointF(215,10));
    contour.addPoint(QPointF(215,110));
    contour.addPoint(QPointF(165,110));
    contour.addPoint(QPointF(165,60));
    contour.addPoint(QPointF(155,60));
    contour.addPoint(QPointF(155,80));
    contour.addPoint(QPointF(105,80));
    contour.addPoint(QPointF(105,70));
    contour.addPoint(QPointF(100,70));
    contour.addPoint(QPointF(100,100));
    contour.addPoint(QPointF(0,100));

    KovtunMethodExecuter kovtunMethodExecuter(contour);

    KovtunMethodExecuterFileLogger fileLogger;
    kovtunMethodExecuter.addListener(fileLogger);

    QtQuick2ControlsApplicationViewer viewer;
    viewer.setMainQmlFile(QStringLiteral("qml/kovtun-method/main.qml"));
    viewer.show();

    KovtunMethodPainter * const kovtunMethodPainter =
            dynamic_cast<KovtunMethodPainter*>(viewer.getObject("kovtunMethodPainter"));

    KovtunMethodExecuterQmlInterface * const kovtunMethodQmlInterface =
            dynamic_cast<KovtunMethodExecuterQmlInterface*>(viewer.getObject("kovtunMethodExecuter"));

    if (kovtunMethodPainter != nullptr && kovtunMethodQmlInterface != nullptr)
    {
        kovtunMethodPainter->setKovtunMethodExecuter(kovtunMethodExecuter);
        kovtunMethodQmlInterface->setKovtunMethodExecuter(kovtunMethodExecuter);
    }

    return app.exec();
}
