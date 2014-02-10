#include "ExecuterFileLogger.h"

#include <QFile>
#include <QTextStream>
#include <QDir>
#include <QDateTime>

#ifdef QT_DEBUG
#include <QDebug>
#endif

namespace KovtunMethod
{

ExecuterFileLogger::ExecuterFileLogger(const Executer & executer) :
    ExecuterListener(),
    executer(executer),
    stepIndex(1),
    output(),
    filledRectanglesColors(),
    filledRectanglesInfo()
{
}

void ExecuterFileLogger::onGravityCenterCalculated(const QPointF & gravityCenter, const double error, const MyQRectF & rectangle)
{
    output +=
            "rect name: "
            + rectangle.getName()
            + ";\tgravity center:\t("
            + QString::number(gravityCenter.x())
            + ", "
            + QString::number(gravityCenter.y())
            + ");\terror: "
            + QString::number(error)
            + "\n";
}

void ExecuterFileLogger::onColorGathered(const QColor & color, const MyQRectF & rectangle)
{
    output +=
            "rect name: "
            + rectangle.getName()
            + ";\tcolor:\t\t"
            + toString(color)
            + ";\n";
}

const QString ExecuterFileLogger::toString(const QColor & color) const
{
    return QString("("
            + QString::number(color.red())
            + ", "
            + QString::number(color.green())
            + ", "
            + QString::number(color.blue())
            + ")");
}

void ExecuterFileLogger::onStepFinished()
{
    gatherAdditionalColorInformation();

    const QString folderName(QDir::currentPath() + QDir::separator() + "Logs");
    const QDir folder(folderName);

    if (!folder.exists())
    {
        folder.mkdir(folderName);
    }

    QString filename(folderName + QDir::separator());
    filename += "log" + QString::number(stepIndex++) + ".txt";

    QFile file(filename);

    if (file.open(QIODevice::WriteOnly))
    {
        QTextStream stream(&file);
        stream << QDateTime::currentDateTime().toString() << "\n\n";
        stream << output.toLatin1();
        file.close();
    }
#ifdef QT_DEBUG
    else
    {
        qDebug() << "По какой-то причине файл лога не был создан!";
    }
#endif

    output.clear();
    filledRectanglesColors.clear();
    filledRectanglesInfo.clear();
}

void ExecuterFileLogger::gatherAdditionalColorInformation()
{
    fillAdditionalColorInformationFields();

    QString additionalColorInformation = getAdditionalColorsInformationString();

    additionalColorInformation += "\n";
    output.push_front(additionalColorInformation);
}

void ExecuterFileLogger::fillAdditionalColorInformationFields()
{
    for (int index = 0; index < executer.getFilledRectanglesCount(); index++)
    {
        const MyQRectF & currentFilledRectangle = executer.getFilledRectangle(index);
        const QColor & currentColor = currentFilledRectangle.getColor();

        if (filledRectanglesColors.contains(currentColor))
        {
            const int indexOfCurrentColor = filledRectanglesColors.indexOf(currentColor);

            FilledRectanglesInfo & currentColorInfo = filledRectanglesInfo[indexOfCurrentColor];
            currentColorInfo.rectanglesCount++;
            currentColorInfo.rectanglesArea += currentFilledRectangle.getArea();
        }
        else
        {
            filledRectanglesColors.push_back(currentColor);
            filledRectanglesInfo.push_back(FilledRectanglesInfo(1, currentFilledRectangle.getArea()));
        }
    }
}

const QString ExecuterFileLogger::getAdditionalColorsInformationString() const
{
    QString additionalColorInformation;

    for (int index = 0; index < filledRectanglesColors.size(); index++)
    {
        const QColor & currentColor = filledRectanglesColors[index];
        const FilledRectanglesInfo & currentColorInfo = filledRectanglesInfo[index];

        additionalColorInformation +=
                "color:"
                + toString(currentColor)
                + ",\t"
                + "rectangles count: "
                + QString::number(currentColorInfo.rectanglesCount)
                + ",\t"
                + "rectangles area: "
                + QString::number(currentColorInfo.rectanglesArea)
                + "\n";
    }

    return additionalColorInformation;
}

void ExecuterFileLogger::onReset()
{
    stepIndex = 1;
}

}
