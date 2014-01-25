#include <QString>
#include <QtTest>

#include "../../main/cppsrc/Executer.h"
#include "../../main/cppsrc/ClosedContour.h"

using namespace KovtunMethod;

class TestKovtunMethodExecuterTest : public QObject
{
    Q_OBJECT

public:
    TestKovtunMethodExecuterTest();

private Q_SLOTS:
    void initTestCase();
    void cleanupTestCase();
    void hasCorrectFirstActiveRectangle();

private:
    QPointF topLeftPoint;
    QPointF bottomRightPoint;
    ClosedContour contour;
    Executer * kovtunMethodExecuter;
};

TestKovtunMethodExecuterTest::TestKovtunMethodExecuterTest() :
    topLeftPoint(-10, 50),
    bottomRightPoint(50, -10),
    contour(),
    kovtunMethodExecuter(nullptr)
{
    contour.addPoint(QPointF(topLeftPoint));
    contour.addPoint(QPointF(0, 0));
    contour.addPoint(QPointF(1, 1));
    contour.addPoint(QPointF(2, 2));
    contour.addPoint(QPointF(bottomRightPoint.x(), topLeftPoint.y()));
    contour.addPoint(QPointF(bottomRightPoint));
    contour.addPoint(QPointF(topLeftPoint.x(), bottomRightPoint.y()));
}

void TestKovtunMethodExecuterTest::initTestCase()
{
    kovtunMethodExecuter = new Executer(contour);
}

void TestKovtunMethodExecuterTest::cleanupTestCase()
{
    delete kovtunMethodExecuter;
}

void TestKovtunMethodExecuterTest::hasCorrectFirstActiveRectangle()
{
    QVERIFY2(kovtunMethodExecuter->getActiveRectanglesCount() == 0,
             "По какой-то причине kovtunMethodExecuter о первого шага содержит active rectangles!");

    kovtunMethodExecuter->performNextStep();

    QVERIFY2(kovtunMethodExecuter->getActiveRectanglesCount() == 1,
             "По какой-то причине число active rectangles в kovtunMethodExecuter после первого шага не равно 1!");
}

QTEST_APPLESS_MAIN(TestKovtunMethodExecuterTest)

#include "tst_TestKovtunMethodExecuterTest.moc"
