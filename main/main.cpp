#include "qtquick2controlsapplicationviewer.h"
#include "cppsrc/KovtunMethodPainter.h"
#include "cppsrc/KovtunMethodExecuter.h"
#include "cppsrc/KovtunMethodExecuterQmlInterface.h"
#include <QtQml>

int main(int argc, char *argv[])
{
    qmlRegisterType<KovtunMethodPainter>("KovtunMethod", 1, 0, "KovtunMethodPainter");

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

//    contour.addPoint(QPointF(0,0));
//    contour.addPoint(QPointF(1000,0));
//    contour.addPoint(QPointF(1000,400));
//    contour.addPoint(QPointF(1050,400));
//    contour.addPoint(QPointF(1050,300));
//    contour.addPoint(QPointF(1550,300));
//    contour.addPoint(QPointF(1550,500));
//    contour.addPoint(QPointF(1650,500));
//    contour.addPoint(QPointF(1650,100));
//    contour.addPoint(QPointF(2150,100));
//    contour.addPoint(QPointF(2150,1100));
//    contour.addPoint(QPointF(1650,1100));
//    contour.addPoint(QPointF(1650,600));
//    contour.addPoint(QPointF(1550,600));
//    contour.addPoint(QPointF(1550,800));
//    contour.addPoint(QPointF(1050,800));
//    contour.addPoint(QPointF(1050,700));
//    contour.addPoint(QPointF(1000,700));
//    contour.addPoint(QPointF(1000,1000));
//    contour.addPoint(QPointF(0,1000));

    KovtunMethodExecuter kovtunMethodExecuter(contour);

    KovtunMethodExecuterQmlInterface kovtunMethodExecuterQmlInterface(kovtunMethodExecuter);

    QtQuick2ControlsApplicationViewer viewer;
    viewer.setContextProperty("kovtunMethodExecuterQmlInterface", &kovtunMethodExecuterQmlInterface);
    viewer.setMainQmlFile(QStringLiteral("qml/kovtun-method/main.qml"));
    viewer.show();

    KovtunMethodPainter * const kovtunMethodPainter =
            dynamic_cast<KovtunMethodPainter*>(viewer.getObject("kovtunMethodPainter"));

    if (kovtunMethodPainter != nullptr)
    {
        kovtunMethodPainter->setKovtunMethodExecuter(kovtunMethodExecuter);
        kovtunMethodExecuterQmlInterface.setKovtunMethodPainter(*kovtunMethodPainter);
    }

    return app.exec();
}
