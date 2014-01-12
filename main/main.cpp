#include "qtquick2controlsapplicationviewer.h"
#include "cppsrc/KovtunMethodPainter.h"
#include "cppsrc/KovtunMethodExectuter.h"
#include <QtQml>

int main(int argc, char *argv[])
{
    qmlRegisterType<KovtunMethodPainter>("KovtunMethod", 1, 0, "KovtunMethodPainter");

    Application app(argc, argv);

    QtQuick2ControlsApplicationViewer viewer;
    viewer.setMainQmlFile(QStringLiteral("qml/kovtun-method/main.qml"));
    viewer.show();

    KovtunMethodPainter * const kovtunMethodPainter =
            dynamic_cast<KovtunMethodPainter*>(viewer.getObject("kovtunMethodPainter"));

    if (kovtunMethodPainter != nullptr)
    {
        Contour contour;

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
        kovtunMethodPainter->setKovtunMethodExecuter(kovtunMethodExecuter);

        kovtunMethodPainter->update();
    }

    return app.exec();
}
