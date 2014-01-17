#include <QString>
#include <QtTest>

#include "../../main/cppsrc/RectangleToolKit.h"
#include "../../main/cppsrc/Contour.h"

class TestRectangleToolKit_isAnyPointOfAnyLineOfContourInsideOfRectangleTest : public QObject
{
    Q_OBJECT

public:
    TestRectangleToolKit_isAnyPointOfAnyLineOfContourInsideOfRectangleTest();

private Q_SLOTS:
    void contourInsideOfRectangleIsReallyInside();

private:
    Contour contour;
};

TestRectangleToolKit_isAnyPointOfAnyLineOfContourInsideOfRectangleTest::TestRectangleToolKit_isAnyPointOfAnyLineOfContourInsideOfRectangleTest():
    contour()
{
    contour.addPoint(0, 0);
    contour.addPoint(100, 0);
    contour.addPoint(100, 100);
    contour.addPoint(0, 100);
}

void TestRectangleToolKit_isAnyPointOfAnyLineOfContourInsideOfRectangleTest::contourInsideOfRectangleIsReallyInside()
{
    const QRectF rectangle(QPointF(-10, 110), QPointF(110, -10));

    const bool contourIsInside = RectangleToolKit::isAnyPointOfAnyLineOfContourInsideOfRectangle(contour, rectangle);

    QVERIFY2(contourIsInside, "Почему-то ни одна точка контура не лежит в прямоугольнике, хотя он целиком должен лежить в нём!");
}

QTEST_APPLESS_MAIN(TestRectangleToolKit_isAnyPointOfAnyLineOfContourInsideOfRectangleTest)

#include "tst_TestRectangleToolKit_isAnyPointOfAnyLineOfContourInsideOfRectangleTest.moc"
