#ifndef CONTOURSETTERQMLINTERFACE_H
#define CONTOURSETTERQMLINTERFACE_H

#include <QObject>
#include <QUrl>
#include <QString>
#include "ClosedContour.h"
#include "ClosedContourParser.h"

namespace KovtunMethod
{

class ClosedContourSetterQmlInterface : public QObject
{
    Q_OBJECT
public:
    explicit ClosedContourSetterQmlInterface(QObject * parent = 0);

    Q_INVOKABLE void setByFile(const QUrl & url);
    Q_INVOKABLE void setByJson(const QString & jsonString);

signals:
    void contourPointed(const ClosedContour & contour);
    void errorOccured(const QString & errorDescription);
    void closedContourIsSet();

public slots:
    void onClosedContourIsSet();
};

}

#endif // CONTOURSETTERQMLINTERFACE_H
