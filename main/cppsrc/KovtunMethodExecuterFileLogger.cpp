#include "KovtunMethodExecuterFileLogger.h"

#include <QFile>
#include <QTextStream>
#include <QDir>
#include <QDateTime>

#ifdef QT_DEBUG
#include <QDebug>
#endif

KovtunMethodExecuterFileLogger::KovtunMethodExecuterFileLogger() :
    KovtunMethodExecuterListener(),
    stepIndex(0),
    output()
{
}

void KovtunMethodExecuterFileLogger::onGravityCenterCalculated(const QPointF & gravityCenter, const double error, const KovtunQRectF & rectangle)
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

void KovtunMethodExecuterFileLogger::onColorGathered(const QColor & color, const KovtunQRectF & rectangle)
{
    output += "rect name: " + rectangle.getName() + ";\tcolor:\t\t(" + QString::number(color.red()) + ", " + QString::number(color.green()) + ", " + QString::number(color.blue()) + ");\n";
}

void KovtunMethodExecuterFileLogger::onStepFinished()
{
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
}

void KovtunMethodExecuterFileLogger::onReset()
{
    stepIndex = 0;
}
