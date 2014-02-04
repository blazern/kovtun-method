#include <QString>
#include <QtTest>
#include <QCoreApplication>

#include "../../main/cppsrc/RectangleToolKit.h"
#include "../../main/cppsrc/ClosedContour.h"

using namespace KovtunMethod;

class TestRectanglesToolKit_isAnyPointOfRectangleInsideOfContourTest : public QObject
{
    Q_OBJECT

public:
    TestRectanglesToolKit_isAnyPointOfRectangleInsideOfContourTest();

private Q_SLOTS:
    void bottomLeftUnitWith50DimensionInOutside();

private:
    ClosedContour contour;
    const QRectF rectangle;
};

TestRectanglesToolKit_isAnyPointOfRectangleInsideOfContourTest::TestRectanglesToolKit_isAnyPointOfRectangleInsideOfContourTest() :
    contour(),
    rectangle(0, 0, 215, 110)
{
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
}

void TestRectanglesToolKit_isAnyPointOfRectangleInsideOfContourTest::bottomLeftUnitWith50DimensionInOutside()
{
    const qreal unitWidth = rectangle.width() / 50;
    const qreal unitHeight = rectangle.height() / 50;

    const qreal x = rectangle.left() + 0 * unitWidth;
    const qreal y = rectangle.top() + 49 * unitHeight;

    const QRectF unit(x, y, unitWidth, unitHeight);

    const bool unitInsideOfContour = RectangleToolKit::isAnyPointOfRectangleInsideOfContour(contour, unit);

    QVERIFY2(!unitInsideOfContour, "По какой-то причине юнит находится в контуре!");
}

QTEST_MAIN(TestRectanglesToolKit_isAnyPointOfRectangleInsideOfContourTest)

#include "tst_TestRectanglesToolKit_isAnyPointOfRectangleInsideOfContourTest.moc"
