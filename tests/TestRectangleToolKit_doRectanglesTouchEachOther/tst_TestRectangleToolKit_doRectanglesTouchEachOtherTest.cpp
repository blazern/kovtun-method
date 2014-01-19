#include <QString>
#include <QtTest>

#include "../../main/cppsrc/RectangleToolKit.h"

class TestRectangleToolKit_doRectanglesTouchEachOtherTest : public QObject
{
    Q_OBJECT

public:
    TestRectangleToolKit_doRectanglesTouchEachOtherTest();

private Q_SLOTS:
    void sameSizedWithSameYTouchedRectanglesTouchEachOther();
    void sameSizedWithDifferentYTouchedRectanglesTouchEachOther();
    void rectanglesTouchedByAnglesTouchEachOther();
    void intersectingRectanglesDoNotTouchEachOther();
    void innerRectangleAndOuterDoNotTouchEatchOther();

    void notTouchingRectanglesTouchEachOther();

    void rectanglesTouchedByOtherSidesTouchEachOther();
    void rectanglesIntersectedByOtherSIdesDoNotTouchEachOther();
};

TestRectangleToolKit_doRectanglesTouchEachOtherTest::TestRectangleToolKit_doRectanglesTouchEachOtherTest()
{
}

void TestRectangleToolKit_doRectanglesTouchEachOtherTest::sameSizedWithSameYTouchedRectanglesTouchEachOther()
{
    const QRectF leftRectangle(-10, 10, 10, 10);
    const QRectF rightRectangle(0, 10, 10, 10);

    const bool touching =
            RectangleToolKit::doRectanglesTouchEachOther(leftRectangle, rightRectangle)
            && RectangleToolKit::doRectanglesTouchEachOther(rightRectangle, leftRectangle);

    QVERIFY2(touching, "Соседние прилягающие прямоугольники почему-то не касаются друг-друга!");
}

void TestRectangleToolKit_doRectanglesTouchEachOtherTest::sameSizedWithDifferentYTouchedRectanglesTouchEachOther()
{
    const QRectF leftRectangle(-10, 5, 10, 10);
    const QRectF rightRectangle(0, 10, 10, 10);

    const bool touching =
            RectangleToolKit::doRectanglesTouchEachOther(leftRectangle, rightRectangle)
            && RectangleToolKit::doRectanglesTouchEachOther(rightRectangle, leftRectangle);

    QVERIFY2(touching, "Соседние прилягающие прямоугольники почему-то не касаются друг-друга!");
}

void TestRectangleToolKit_doRectanglesTouchEachOtherTest::rectanglesTouchedByAnglesTouchEachOther()
{
    const QRectF leftRectangle(QPointF(0, 0), QPointF(10, 10));
    const QRectF rightRectangle(QPointF(10, 10), QPointF(20, 20));

    const bool touching =
            RectangleToolKit::doRectanglesTouchEachOther(leftRectangle, rightRectangle)
            && RectangleToolKit::doRectanglesTouchEachOther(rightRectangle, leftRectangle);

    QVERIFY2(touching, "Соседние прямоугольники с одной одинаковой точкой почему-то не касаются друг-друга!");
}

void TestRectangleToolKit_doRectanglesTouchEachOtherTest::intersectingRectanglesDoNotTouchEachOther()
{
    const QRectF leftRectangle(-5, 10, 10, 10);
    const QRectF rightRectangle(0, 10, 10, 10);

    const bool touching =
            RectangleToolKit::doRectanglesTouchEachOther(leftRectangle, rightRectangle)
            && RectangleToolKit::doRectanglesTouchEachOther(rightRectangle, leftRectangle);

    QVERIFY2(!touching, "Пересекающиеся прямоугольники почему-то касаются друг-друга!");
}

void TestRectangleToolKit_doRectanglesTouchEachOtherTest::innerRectangleAndOuterDoNotTouchEatchOther()
{
    const QRectF innerRectangle(0, 0, 10, 10);
    const QRectF outerRectangle(-5, -5, 20, 20);

    const bool touching =
            RectangleToolKit::doRectanglesTouchEachOther(innerRectangle, outerRectangle)
            && RectangleToolKit::doRectanglesTouchEachOther(outerRectangle, innerRectangle);

    QVERIFY2(!touching, "Прямоугольник, лежащий внутри другого, почему-то его касается!");
}

void TestRectangleToolKit_doRectanglesTouchEachOtherTest::notTouchingRectanglesTouchEachOther()
{
    const QRectF leftRectangle(-20, 10, 10, 10);
    const QRectF rightRectangle(0, 10, 10, 10);

    const bool touching =
            RectangleToolKit::doRectanglesTouchEachOther(leftRectangle, rightRectangle)
            && RectangleToolKit::doRectanglesTouchEachOther(rightRectangle, leftRectangle);

    QVERIFY2(!touching, "Соседние не прилягающие прямоугольники почему-то касаются друг-друга!");
}

void TestRectangleToolKit_doRectanglesTouchEachOtherTest::rectanglesTouchedByOtherSidesTouchEachOther()
{
    const QRectF topTouchingRectangle(0, 0, 10, 10);
    const QRectF bottomTouchingRectangle(0, 10, 10, 10);

    const bool touching =
            RectangleToolKit::doRectanglesTouchEachOther(topTouchingRectangle, bottomTouchingRectangle)
            && RectangleToolKit::doRectanglesTouchEachOther(bottomTouchingRectangle, topTouchingRectangle);

    QVERIFY2(touching, "Соседние прилягающие прямоугольники почему-то не касаются друг-друга!");
}

void TestRectangleToolKit_doRectanglesTouchEachOtherTest::rectanglesIntersectedByOtherSIdesDoNotTouchEachOther()
{
    const QRectF topRectangle(0, 0, 10, 10);
    const QRectF bottomRectangle(0, 5, 10, 10);

    const bool touching =
            RectangleToolKit::doRectanglesTouchEachOther(topRectangle, bottomRectangle)
            && RectangleToolKit::doRectanglesTouchEachOther(bottomRectangle, topRectangle);

    QVERIFY2(!touching, "Пересекающиеся прямоугольники почему-то касаются друг-друга!");
}

//QTEST_APPLESS_MAIN(TestRectangleToolKit_doRectanglesTouchEachOtherTest)
QTEST_MAIN(TestRectangleToolKit_doRectanglesTouchEachOtherTest)

#include "tst_TestRectangleToolKit_doRectanglesTouchEachOtherTest.moc"
