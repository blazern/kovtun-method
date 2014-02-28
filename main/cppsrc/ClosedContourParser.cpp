#include "ClosedContourParser.h"

#include <QFile>
#include <QJsonParseError>
#include <QJsonDocument>
#include <QJsonValue>
#include <QJsonArray>
#include <stdexcept>
#include <string>

#ifdef QT_DEBUG
#include <QDebug>
#endif

namespace KovtunMethod
{

ClosedContour ClosedContourParser::parse(const QUrl & url)
{
    const QJsonObject closedContourAsJson = parseJsonFile(url);

    return parseJsonObject(closedContourAsJson);
}

QJsonObject ClosedContourParser::parseJsonFile(const QUrl & url)
{
    const QString path = url.toLocalFile();
    QFile file(path);

    if (file.exists())
    {
        QJsonParseError jsonParseError;

        file.open(QIODevice::ReadOnly | QIODevice::Text);

        QJsonDocument document = QJsonDocument::fromJson(file.readAll(), &jsonParseError);

        file.close();

        if (jsonParseError.error == QJsonParseError::NoError)
        {
            return document.object();
        }
#ifdef QT_DEBUG
        else
        {
            throw std::invalid_argument(QString(
                                        "Парсинг файла с URL: "
                                        + url.toString()
                                        + " закончился ошибкой: "
                                        + jsonParseError.errorString()
                                        + " в позиции: "
                                        + QString::number(jsonParseError.offset)).toStdString());
        }
#endif
    }
#ifdef QT_DEBUG
    else
    {
        throw std::invalid_argument("Парсинг файла с URL: "
                                    + url.toString().toStdString()
                                    + " невозможен, т.к. файл не существует");
    }
#endif
}

ClosedContour ClosedContourParser::parse(const QString & json)
{
    const QJsonDocument jsonDocument = QJsonDocument::fromJson(json.toUtf8());

    return parseJsonObject(jsonDocument.object());
}


/// Пример JSON-объекта:
/// {
///     points: [
///         { "x": 20.123, "y":31.28 },
///         { "x": 32.1, "y":64.23 }
///     ]
/// }
ClosedContour ClosedContourParser::parseJsonObject(const QJsonObject & closedContourAsJson)
{
    if (closedContourAsJson.contains("points"))
    {
        const QJsonValue pointsValue = closedContourAsJson.value("points");

        if (pointsValue.isArray())
        {
            ClosedContour closedContour;
            const QJsonArray pointsArray = pointsValue.toArray();

            for (const auto & pointValue : pointsArray)
            {
                if (pointValue.isObject())
                {
                    const QJsonObject pointObject = pointValue.toObject();

                    if (pointObject.contains("x") && pointObject.contains("y"))
                    {
                        const QJsonValue xValue = pointObject.value("x");
                        const QJsonValue yValue = pointObject.value("y");

                        if (xValue.isDouble() && yValue.isDouble())
                        {
                            const double x = xValue.toDouble();
                            const double y = yValue.toDouble();
                            closedContour.addPoint(x, y);
                        }
                        else
                        {
                            throw std::invalid_argument("'x' и/или 'y' не является числом");
                        }
                    }
                    else
                    {
                        throw std::invalid_argument("Объект, рассматриваемый как точка, на содержит 'x' и/или 'y'");
                    }
                }
                else
                {
                    throw std::invalid_argument("Некоторый элемент массива не является объектом");
                }
            }
            return closedContour;
        }
        else
        {
            throw std::invalid_argument("Объект 'points' не является массивом");
        }
    }
    else
    {
        throw std::invalid_argument("JSON-объект не содержит элемента 'points'");
    }
}

}
