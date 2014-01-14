#include <QString>
#include <QtTest>

#include "../../main/cppsrc/RectangleToolKit.h"
#include "../../main/cppsrc/Contour.h"

class TestRectangleToolKit : public QObject
{
    Q_OBJECT

public:
    TestRectangleToolKit();

private Q_SLOTS:
    void innerRectangleIsInside();
    void outerRectangleIsOutside();
    void somePointOfHalfInsideRectangleIsInside();
    void allPointsOfTouchingRectangleAreOutside();

private:
    Contour contour;

};

TestRectangleToolKit::TestRectangleToolKit() :
    contour()
{
    contour.addPoint(0, 0);
    contour.addPoint(100, 0);
    contour.addPoint(100, 100);
    contour.addPoint(0, 100);
}

void TestRectangleToolKit::innerRectangleIsInside()
{
    const QRectF innerRectangle(QPointF(10, 10), QPointF(90, 90));
    QVERIFY2(RectangleToolKit::isAnyPointOfRectangleInsideOfContour(contour, innerRectangle),
             "Почему-то ни одна точка внутреннего прямоугольника не лежит в контуре!");
}

void TestRectangleToolKit::outerRectangleIsOutside()
{
    const QRectF outerRectangle(QPointF(-20, -20), QPointF(-10, -10));
    QVERIFY2(!RectangleToolKit::isAnyPointOfRectangleInsideOfContour(contour, outerRectangle),
             "Почему-то какая-то точка внешнего прямоугольника лежит в контуре!");
}

void TestRectangleToolKit::somePointOfHalfInsideRectangleIsInside()
{
    const QRectF halfInsideRectangle(QPointF(-20, 20), QPointF(20, 0));
    QVERIFY2(RectangleToolKit::isAnyPointOfRectangleInsideOfContour(contour, halfInsideRectangle),
             "Почему-то ни одна точка от частично-лежащего-в-контуре прямоугольника не лежит в контуре!");
}

void TestRectangleToolKit::allPointsOfTouchingRectangleAreOutside()
{
    const QRectF touchingRectangle(QPointF(-100, -100), QPointF(0, 0));
    QVERIFY2(RectangleToolKit::isAnyPointOfRectangleInsideOfContour(contour, touchingRectangle),
             "Почему-то какая-то точка прилягающего прямоугольника лежит в контуре!");
}

QTEST_APPLESS_MAIN(TestRectangleToolKit)

#include "tst_TestRectangleToolKit.moc"
