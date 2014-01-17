#include <QString>
#include <QtTest>
#include <QCoreApplication>

#include "../../main/cppsrc/RectangleToolKit.h"
#include "../../main/cppsrc/Contour.h"

class TestRectangleToolKit_doLinesIntersectWithoutLyingOnEachOtherTest : public QObject
{
    Q_OBJECT

public:
    TestRectangleToolKit_doLinesIntersectWithoutLyingOnEachOtherTest();

private Q_SLOTS:
    void doLinesIntersectWithoutLyingOnEachOtherTest1();
    void doLinesIntersectWithoutLyingOnEachOtherTest2();
    void doLinesIntersectWithoutLyingOnEachOtherTest3();
};

TestRectangleToolKit_doLinesIntersectWithoutLyingOnEachOtherTest::TestRectangleToolKit_doLinesIntersectWithoutLyingOnEachOtherTest()
{
}

void TestRectangleToolKit_doLinesIntersectWithoutLyingOnEachOtherTest::doLinesIntersectWithoutLyingOnEachOtherTest1()
{
    const QLineF line1(0, 10, 10, 0);
    const QLineF line2(3, 6, 5, 6);

    QVERIFY(RectangleToolKit::doLinesIntersectWithoutLyingOnEachOther(line1, line2));
}

void TestRectangleToolKit_doLinesIntersectWithoutLyingOnEachOtherTest::doLinesIntersectWithoutLyingOnEachOtherTest2()
{
    const QLineF line1(10, 0, 0, 10);
    const QLineF line2(3, 6, 5, 6);

    QVERIFY(RectangleToolKit::doLinesIntersectWithoutLyingOnEachOther(line1, line2));
}

void TestRectangleToolKit_doLinesIntersectWithoutLyingOnEachOtherTest::doLinesIntersectWithoutLyingOnEachOtherTest3()
{
    const QLineF line1(0, 0, 10, 10);
    const QLineF line2(3, 3, 13, 13);

    QVERIFY(!RectangleToolKit::doLinesIntersectWithoutLyingOnEachOther(line1, line2));
}


QTEST_MAIN(TestRectangleToolKit_doLinesIntersectWithoutLyingOnEachOtherTest)

#include "tst_TestRectangleToolKit_doLinesIntersectWithoutLyingOnEachOtherTest.moc"
