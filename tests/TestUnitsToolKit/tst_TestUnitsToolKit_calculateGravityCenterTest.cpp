#include <QString>
#include <QtTest>
#include <QCoreApplication>

#include "../../main/cppsrc/ClosedContour.h"
#include "../../main/cppsrc/UnitsToolKit.h"

using namespace KovtunMethod;

class TestUnitsToolKit_calculateGravityCenterTest : public QObject
{
    Q_OBJECT

public:
    TestUnitsToolKit_calculateGravityCenterTest();

private Q_SLOTS:
    void gravityCenterIsInCenterOfRectangleEqualToContour();
    void gravityCenterIsInCenterOfRectangleLittleBiggerThanContour();
    void rectangleTakesHalfOfContour();
    void rectangleTakesFourthOfContour();

private:
    ClosedContour contour;
};

TestUnitsToolKit_calculateGravityCenterTest::TestUnitsToolKit_calculateGravityCenterTest() :
    contour()
{
    contour.addPoint(0, 0);
    contour.addPoint(10, 0);
    contour.addPoint(10, 10);
    contour.addPoint(0, 10);
}

void TestUnitsToolKit_calculateGravityCenterTest::gravityCenterIsInCenterOfRectangleEqualToContour()
{
    const QRectF rectangle(0, 0, 10, 10);

    const QPointF expectedGravityCenter(5, 5);

    const double maxDiff = 0.0001;

    const QPointF gravityCenter = UnitsToolKit::calculateGravityCenter(contour, rectangle, 20).first;

    const bool valueIsGood = abs(expectedGravityCenter.x() - gravityCenter.x()) < maxDiff
                            && abs(expectedGravityCenter.y() - gravityCenter.y()) < maxDiff;

    QVERIFY2(valueIsGood, "Почему-то центр гравитации квадрата не находится в его центре!");
}

void TestUnitsToolKit_calculateGravityCenterTest::gravityCenterIsInCenterOfRectangleLittleBiggerThanContour()
{
    const QRectF rectangle(-1, -1, 12, 12);

    const QPointF expectedGravityCenter(5, 5);

    const double maxDiff = 0.0001;

    const QPointF gravityCenter = UnitsToolKit::calculateGravityCenter(contour, rectangle, 20).first;

    const bool valueIsGood = abs(expectedGravityCenter.x() - gravityCenter.x()) < maxDiff
                            && abs(expectedGravityCenter.y() - gravityCenter.y()) < maxDiff;

    QVERIFY2(valueIsGood, "Почему-то центр гравитации квадрата не находится в его центре!");
}

void TestUnitsToolKit_calculateGravityCenterTest::rectangleTakesHalfOfContour()
{
    const QRectF rectangle(-5, 0, 10, 10);

    const QPointF expectedGravityCenter(2.5, 5);

    const double maxDiff = 0.0001;

    const QPointF gravityCenter = UnitsToolKit::calculateGravityCenter(contour, rectangle, 20).first;

    const bool valueIsGood = abs(expectedGravityCenter.x() - gravityCenter.x()) < maxDiff
                            && abs(expectedGravityCenter.y() - gravityCenter.y()) < maxDiff;

    QVERIFY2(valueIsGood, "Почему-то центр гравитации квадрата не находится в его центре!");
}

void TestUnitsToolKit_calculateGravityCenterTest::rectangleTakesFourthOfContour()
{
    const QRectF rectangle(-5, -5, 10, 10);

    const QPointF expectedGravityCenter(2.5, 2.5);

    const double maxDiff = 0.0001;

    const QPointF gravityCenter = UnitsToolKit::calculateGravityCenter(contour, rectangle, 20).first;

    const bool valueIsGood = abs(expectedGravityCenter.x() - gravityCenter.x()) < maxDiff
                            && abs(expectedGravityCenter.y() - gravityCenter.y()) < maxDiff;

    QVERIFY2(valueIsGood, "Почему-то центр гравитации квадрата не находится в его центре!");
}

QTEST_MAIN(TestUnitsToolKit_calculateGravityCenterTest)

#include "tst_TestUnitsToolKit_calculateGravityCenterTest.moc"
