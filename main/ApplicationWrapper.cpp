#include "ApplicationWrapper.h"

#include <stdexcept>
#include "cppsrc/ClosedContourSetterQmlInterface.h"

#ifdef QT_DEBUG
#include <QDebug>
#endif

QString const ApplicationWrapper::defaultContourJson("{"
                                                     "\"points\": ["
                                                         "{ \"x\":0, \"y\":0 },"
                                                         "{ \"x\":100, \"y\":0 },"
                                                         "{ \"x\":100, \"y\":40 },"
                                                         "{ \"x\":105, \"y\":40 },"
                                                         "{ \"x\":105, \"y\":30 },"
                                                         "{ \"x\":155, \"y\":30 },"
                                                         "{ \"x\":155, \"y\":50 },"
                                                         "{ \"x\":165, \"y\":50 },"
                                                         "{ \"x\":165, \"y\":10 },"
                                                         "{ \"x\":215, \"y\":10 },"
                                                         "{ \"x\":215, \"y\":110 },"
                                                         "{ \"x\":165, \"y\":110 },"
                                                         "{ \"x\":165, \"y\":60 },"
                                                         "{ \"x\":155, \"y\":60 },"
                                                         "{ \"x\":155, \"y\":80 },"
                                                         "{ \"x\":105, \"y\":80 },"
                                                         "{ \"x\":105, \"y\":70 },"
                                                         "{ \"x\":100, \"y\":70 },"
                                                         "{ \"x\":100, \"y\":100 },"
                                                         "{ \"x\":0, \"y\":100 }"
                                                     "]"
                                                 "}");

ApplicationWrapper::ApplicationWrapper(
        QtQuick2ControlsApplicationViewer & applicationViewer,
        QObject * parent) :
    QObject(parent),
    kovtunMethodExecuter(nullptr),
    fileLogger(),
    progressWatcher(),
    kovtunMethodPainter(nullptr),
    kovtunMethodQmlInterface(nullptr),
    closedContourSetterQmlInterface(nullptr)
{
    KovtunMethod::ClosedContour contour;
    try
    {
        contour = KovtunMethod::ClosedContourParser::parse(defaultContourJson);
    }
    catch (const std::exception & exception)
    {
#ifdef QT_DEBUG
        qDebug() << exception.what();
#endif
    }

    kovtunMethodExecuter = new KovtunMethod::Executer(contour);

    fileLogger.setKovtunMethodExecuter(*kovtunMethodExecuter);
    kovtunMethodExecuter->addListener(fileLogger);

    progressWatcher.setKovtunMethodExecuter(*kovtunMethodExecuter);
    kovtunMethodExecuter->addListener(progressWatcher);

    applicationViewer.setContextProperty("executerProgressWatcher", &progressWatcher);
    applicationViewer.setMainQmlFile(QStringLiteral("qml/kovtun-method/main.qml"));

    kovtunMethodPainter =
            dynamic_cast<KovtunMethod::Painter*>(applicationViewer.getObject("kovtunMethodPainter"));

    kovtunMethodQmlInterface =
            dynamic_cast<KovtunMethod::ExecuterQmlInterface*>(applicationViewer.getObject("kovtunMethodExecuter"));

    closedContourSetterQmlInterface =
            dynamic_cast<KovtunMethod::ClosedContourSetterQmlInterface*>(applicationViewer.getObject("closedContourSetter"));

    if (kovtunMethodPainter != nullptr && kovtunMethodQmlInterface != nullptr && closedContourSetterQmlInterface != nullptr)
    {
        kovtunMethodPainter->setKovtunMethodExecuter(*kovtunMethodExecuter);
        kovtunMethodQmlInterface->setKovtunMethodExecuter(*kovtunMethodExecuter);

        QObject::connect(closedContourSetterQmlInterface, &KovtunMethod::ClosedContourSetterQmlInterface::contourPointed,
                         this, &ApplicationWrapper::onContourPointed);
    }
#ifdef QT_DEBUG
    else
    {
        qDebug() << "По какой-то причине в main.qml отсутствует один из необходимых для работы объектов";
    }
#endif

}

ApplicationWrapper::~ApplicationWrapper()
{
    resetExecuter();
}

void ApplicationWrapper::onContourPointed(const KovtunMethod::ClosedContour & contour)
{
    resetExecuter();

    kovtunMethodExecuter = new KovtunMethod::Executer(contour);
    kovtunMethodPainter->setKovtunMethodExecuter(*kovtunMethodExecuter);
    kovtunMethodQmlInterface->setKovtunMethodExecuter(*kovtunMethodExecuter);

    closedContourSetterQmlInterface->onClosedContourIsSet();
    fileLogger.setKovtunMethodExecuter(*kovtunMethodExecuter);
    kovtunMethodExecuter->addListener(fileLogger);

    progressWatcher.setKovtunMethodExecuter(*kovtunMethodExecuter);
    kovtunMethodExecuter->addListener(progressWatcher);
}

void ApplicationWrapper::resetExecuter()
{
    if (kovtunMethodExecuter != nullptr)
    {
        kovtunMethodExecuter->removeListener(fileLogger);
        kovtunMethodExecuter->removeListener(progressWatcher);
        delete kovtunMethodExecuter;
    }
}
