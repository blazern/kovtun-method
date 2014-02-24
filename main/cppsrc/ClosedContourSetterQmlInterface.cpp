#include "ClosedContourSetterQmlInterface.h"

namespace KovtunMethod
{

ClosedContourSetterQmlInterface::ClosedContourSetterQmlInterface(QObject * parent) :
    QObject(parent)
{
}

void ClosedContourSetterQmlInterface::setByFile(const QUrl & url)
{
    ClosedContour contour;
    try
    {
        contour = ClosedContourParser::parse(url);
        emit contourPointed(contour);
    }
    catch (const std::exception & exception)
    {
        emit errorOccured(exception.what());
    }
}

void ClosedContourSetterQmlInterface::setByJson(const QString & jsonString)
{
    ClosedContour contour;
    try
    {
        contour = ClosedContourParser::parse(jsonString);
        emit contourPointed(contour);
    }
    catch (const std::exception & exception)
    {
        emit errorOccured(exception.what());
    }
}

void ClosedContourSetterQmlInterface::onClosedContourIsSet()
{
    emit closedContourIsSet();
}

}
