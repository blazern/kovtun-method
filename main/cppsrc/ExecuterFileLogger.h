#ifndef KOVTUNMETHODEXECUTERFILELOGGER_H
#define KOVTUNMETHODEXECUTERFILELOGGER_H

#include <QHash>
#include <QColor>
#include "ExecuterListener.h"
#include "Executer.h"

namespace KovtunMethod
{

class ExecuterFileLogger : public ExecuterListener
{
    struct FilledRectanglesInfo
    {
        FilledRectanglesInfo() : rectanglesCount(0), rectanglesArea(0) {}
        FilledRectanglesInfo(const int rectanglesCount, const double rectanglesArea) :
            rectanglesCount(rectanglesCount), rectanglesArea(rectanglesArea) {}

        int rectanglesCount;
        double rectanglesArea;
    };

public:
    ExecuterFileLogger(const ExecuterFileLogger &) = delete;
    ExecuterFileLogger & operator=(const ExecuterFileLogger &) = delete;
    explicit ExecuterFileLogger(const Executer & executer);

protected:
    virtual void onActiveRectangleProcessed() final override {}

    virtual void onStepStarted() final override {}

    virtual void onGravityCenterCalculated(const QPointF & gravityCenter, const double error, const MyQRectF & rectangle) final override;

    virtual void onColorGathered(const QColor & color, const MyQRectF & rectangle) final override;

    virtual void onStepFinished() final override;

    virtual void onReset() final override;

private:
    const Executer & executer;
    int stepIndex;
    QString output;
    QVector<QColor> filledRectanglesColors;
    QVector<FilledRectanglesInfo> filledRectanglesInfo;

    const QString toString(const QColor & color) const;
    void gatherAdditionalColorInformation();
    void fillAdditionalColorInformationFields();
    const QString getAdditionalColorsInformationString() const;
};

}

#endif // KOVTUNMETHODEXECUTERFILELOGGER_H
