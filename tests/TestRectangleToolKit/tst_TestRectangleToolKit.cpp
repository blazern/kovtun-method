#include <QString>
#include <QtTest>

#include <QDebug>
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
    void allPointsOfTouchingWithOnePointRectangleAreOutside();
    void allPointsOfTouchingWithRectangleAreOutside();

    void doLinesIntersectWithoutLyingOnEachOtherTest1();
    void doLinesIntersectWithoutLyingOnEachOtherTest2();
    void doLinesIntersectWithoutLyingOnEachOtherTest3();

    void gravityCenterOfContourPieceLyingWithAngleInRectangleIsCorrect();
    void gravityCenterOfContourPieceLyingWithoutPointsInRectangleIsCorrect();
    void gravityCenterOfContourPieceLyingOnSideOfRectangleIsCorrect();
    void gravityCenterOfContourLyingInsideOfRectangleIsCorrect();

private:
    Contour contourForIntersectionsTesting;
    Contour contourForGravityCenterTesting;

};

TestRectangleToolKit::TestRectangleToolKit() :
    contourForIntersectionsTesting(),
    contourForGravityCenterTesting()
{
    contourForIntersectionsTesting.addPoint(0, 0);
    contourForIntersectionsTesting.addPoint(100, 0);
    contourForIntersectionsTesting.addPoint(100, 100);
    contourForIntersectionsTesting.addPoint(0, 100);

    contourForGravityCenterTesting.addPoint(0, 0);
    contourForGravityCenterTesting.addPoint(10, 0);
    contourForGravityCenterTesting.addPoint(0, 10);
}

void TestRectangleToolKit::innerRectangleIsInside()
{
    const QRectF innerRectangle(QPointF(10, 10), QPointF(90, 90));
    QVERIFY2(RectangleToolKit::isAnyPointOfRectangleInsideOfContour(contourForIntersectionsTesting, innerRectangle),
             "Почему-то ни одна точка внутреннего прямоугольника не лежит в контуре!");
}

void TestRectangleToolKit::outerRectangleIsOutside()
{
    const QRectF outerRectangle(QPointF(-20, -20), QPointF(-10, -10));
    QVERIFY2(!RectangleToolKit::isAnyPointOfRectangleInsideOfContour(contourForIntersectionsTesting, outerRectangle),
             "Почему-то какая-то точка внешнего прямоугольника лежит в контуре!");
}

void TestRectangleToolKit::somePointOfHalfInsideRectangleIsInside()
{
    const QRectF halfInsideRectangle(QPointF(-20, 20), QPointF(20, 0));
    QVERIFY2(RectangleToolKit::isAnyPointOfRectangleInsideOfContour(contourForIntersectionsTesting, halfInsideRectangle),
             "Почему-то ни одна точка от частично-лежащего-в-контуре прямоугольника не лежит в контуре!");
}

void TestRectangleToolKit::allPointsOfTouchingWithOnePointRectangleAreOutside()
{
    const QRectF touchingRectangle(QPointF(-100, -100), QPointF(0, 0));
    QVERIFY2(!RectangleToolKit::isAnyPointOfRectangleInsideOfContour(contourForIntersectionsTesting, touchingRectangle),
             "Почему-то какая-то точка касающегося контура прямоугольника лежит в контуре!");
}

void TestRectangleToolKit::allPointsOfTouchingWithRectangleAreOutside()
{
    const QRectF touchingRectangle(QPointF(-100, 100), QPointF(0, 0));
    QVERIFY2(!RectangleToolKit::isAnyPointOfRectangleInsideOfContour(contourForIntersectionsTesting, touchingRectangle),
             "Почему-то какая-то точка прилягающего прямоугольника лежит в контуре!");
}

void TestRectangleToolKit::doLinesIntersectWithoutLyingOnEachOtherTest1()
{
    const QLineF line1(0, 10, 10, 0);
    const QLineF line2(3, 6, 5, 6);

    QVERIFY(RectangleToolKit::doLinesIntersectWithoutLyingOnEachOther(line1, line2));
}

void TestRectangleToolKit::doLinesIntersectWithoutLyingOnEachOtherTest2()
{
    const QLineF line1(10, 0, 0, 10);
    const QLineF line2(3, 6, 5, 6);

    QVERIFY(RectangleToolKit::doLinesIntersectWithoutLyingOnEachOther(line1, line2));
}

void TestRectangleToolKit::doLinesIntersectWithoutLyingOnEachOtherTest3()
{
    const QLineF line1(0, 0, 10, 10);
    const QLineF line2(3, 3, 13, 13);

    QVERIFY(!RectangleToolKit::doLinesIntersectWithoutLyingOnEachOther(line1, line2));
}

void TestRectangleToolKit::gravityCenterOfContourPieceLyingWithAngleInRectangleIsCorrect()
{
    const QRectF rectangle(QPointF(-1, 12), QPointF(2, 9));
    const QPointF gravityCenter = RectangleToolKit::calculateGravityCenter(contourForGravityCenterTesting, rectangle);

    const QPointF expectedPoint(1 / 3, 28 / 3);

    const float xDiff = abs(gravityCenter.x() - expectedPoint.x());
    const float yDiff = abs(gravityCenter.y() - expectedPoint.y());

    QVERIFY2(xDiff < 0.00001 && yDiff < 0.00001,
             "Почему-то центр тяжести некорректен!");
}

void TestRectangleToolKit::gravityCenterOfContourPieceLyingWithoutPointsInRectangleIsCorrect()
{
    const QRectF rectangle(QPointF(3, 9), QPointF(5, 6));
    const QPointF gravityCenter = RectangleToolKit::calculateGravityCenter(contourForGravityCenterTesting, rectangle);

    const QPointF expectedPoint(7 / 2, 13 / 2);

    const float xDiff = abs(gravityCenter.x() - expectedPoint.x());
    const float yDiff = abs(gravityCenter.y() - expectedPoint.y());

    QVERIFY2(xDiff < 0.00001 && yDiff < 0.00001,
             "Почему-то центр тяжести некорректен!");
}

void TestRectangleToolKit::gravityCenterOfContourPieceLyingOnSideOfRectangleIsCorrect()
{
    const QRectF rectangle(QPointF(3, 3), QPointF(5, 0));
    const QPointF gravityCenter = RectangleToolKit::calculateGravityCenter(contourForGravityCenterTesting, rectangle);

    const QPointF expectedPoint(0, 0);

    const float xDiff = abs(gravityCenter.x() - expectedPoint.x());
    const float yDiff = abs(gravityCenter.y() - expectedPoint.y());

    QVERIFY2(xDiff < 0.00001 && yDiff < 0.00001,
             "Почему-то центр тяжести некорректен!");
}

void TestRectangleToolKit::gravityCenterOfContourLyingInsideOfRectangleIsCorrect()
{
    const QRectF rectangle(QPointF(-2, 12), QPointF(12, -2));
    const QPointF gravityCenter = RectangleToolKit::calculateGravityCenter(contourForGravityCenterTesting, rectangle);

    const QPointF expectedPoint(10 / 3, 10 / 3);

    const float xDiff = abs(gravityCenter.x() - expectedPoint.x());
    const float yDiff = abs(gravityCenter.y() - expectedPoint.y());

    QVERIFY2(xDiff < 0.00001 && yDiff < 0.00001,
             "Почему-то центр тяжести некорректен!");
}

QTEST_APPLESS_MAIN(TestRectangleToolKit)

#include "tst_TestRectangleToolKit.moc"
