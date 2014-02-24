#ifndef CLOSEDCONTOURFILEPARSER_H
#define CLOSEDCONTOURFILEPARSER_H

#include <QUrl>
#include <QJsonObject>
#include "ClosedContour.h"

namespace KovtunMethod
{

class ClosedContourParser
{
public:
    ClosedContourParser() = delete;
    ~ClosedContourParser() = delete;
    ClosedContourParser(const ClosedContourParser &) = delete;
    ClosedContourParser & operator=(const ClosedContourParser &) = delete;

    static ClosedContour parse(const QUrl & url); //throws std::invalid_argument
    static ClosedContour parse(const QString & json); //throws std::invalid_argument

private:
    static QJsonObject parseJsonFile(const QUrl & url);
    static ClosedContour parseJsonObject(const QJsonObject & closedContourAsJson);
};

}

#endif // CLOSEDCONTOURFILEPARSER_H
