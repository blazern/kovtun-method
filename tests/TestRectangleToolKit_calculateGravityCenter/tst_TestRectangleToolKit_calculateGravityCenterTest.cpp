#include <QString>
#include <QtTest>
#include <QCoreApplication>

#include "../../main/cppsrc/Contour.h"
#include "../../main/cppsrc/RectangleToolKit.h"

class TestRectangleToolKit_calculateGravityCenterTest : public QObject
{
    Q_OBJECT

public:
    TestRectangleToolKit_calculateGravityCenterTest();

private Q_SLOTS:
    void gravityCenterIsInCenterOfRectangleEqualToContour();
    void gravityCenterIsInCenterOfRectangleLittleBiggerThanContour();
    void rectangleTakesHalfOfContour();
    void rectangleTakesFourthOfContour();

private:
    Contour contour;
};

TestRectangleToolKit_calculateGravityCenterTest::TestRectangleToolKit_calculateGravityCenterTest() :
    contour()
{
    contour.addPoint(0, 0);
    contour.addPoint(10, 0);
    contour.addPoint(10, 10);
    contour.addPoint(0, 10);
}

void TestRectangleToolKit_calculateGravityCenterTest::gravityCenterIsInCenterOfRectangleEqualToContour()
{
    const QRectF rectangle(0, 0, 10, 10);

    const QPointF expectedGravityCenter(5, 5);

    const double maxDiff = 0.0001;

    const QPointF gravityCenter = RectangleToolKit::calculateGravityCenter(contour, rectangle, 200);

//    qDebug() << gravityCenter;

    const bool valueIsGood = abs(expectedGravityCenter.x() - gravityCenter.x()) < maxDiff
                            && abs(expectedGravityCenter.y() - gravityCenter.y()) < maxDiff;

    QVERIFY2(valueIsGood, "Почему-то центр гравитации квадрата не находится в его центре!");
}

void TestRectangleToolKit_calculateGravityCenterTest::gravityCenterIsInCenterOfRectangleLittleBiggerThanContour()
{
    const QRectF rectangle(-1, -1, 12, 12);

    const QPointF expectedGravityCenter(5, 5);

    const double maxDiff = 0.0001;

    const QPointF gravityCenter = RectangleToolKit::calculateGravityCenter(contour, rectangle, 200);

//    qDebug() << gravityCenter;

    const bool valueIsGood = abs(expectedGravityCenter.x() - gravityCenter.x()) < maxDiff
                            && abs(expectedGravityCenter.y() - gravityCenter.y()) < maxDiff;

    QVERIFY2(valueIsGood, "Почему-то центр гравитации квадрата не находится в его центре!");
}

void TestRectangleToolKit_calculateGravityCenterTest::rectangleTakesHalfOfContour()
{
    const QRectF rectangle(-5, 0, 10, 10);

    const QPointF expectedGravityCenter(2.5, 5);

    const double maxDiff = 0.0001;

    const QPointF gravityCenter = RectangleToolKit::calculateGravityCenter(contour, rectangle, 200);

//    qDebug() << gravityCenter;

    const bool valueIsGood = abs(expectedGravityCenter.x() - gravityCenter.x()) < maxDiff
                            && abs(expectedGravityCenter.y() - gravityCenter.y()) < maxDiff;

    QVERIFY2(valueIsGood, "Почему-то центр гравитации квадрата не находится в его центре!");
}

void TestRectangleToolKit_calculateGravityCenterTest::rectangleTakesFourthOfContour()
{
    const QRectF rectangle(-5, -5, 10, 10);

    const QPointF expectedGravityCenter(2.5, 2.5);

    const double maxDiff = 0.0001;

    const QPointF gravityCenter = RectangleToolKit::calculateGravityCenter(contour, rectangle, 200);

//    qDebug() << gravityCenter;

    const bool valueIsGood = abs(expectedGravityCenter.x() - gravityCenter.x()) < maxDiff
                            && abs(expectedGravityCenter.y() - gravityCenter.y()) < maxDiff;

    QVERIFY2(valueIsGood, "Почему-то центр гравитации квадрата не находится в его центре!");
}

QTEST_MAIN(TestRectangleToolKit_calculateGravityCenterTest)

#include "tst_TestRectangleToolKit_calculateGravityCenterTest.moc"
