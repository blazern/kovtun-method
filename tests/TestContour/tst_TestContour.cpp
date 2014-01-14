#include <QString>
#include <QtTest>
#include "../../main/cppsrc/Contour.h"

class TestContour : public QObject
{
    Q_OBJECT

public:
    TestContour();

private Q_SLOTS:
    void containsInside();
    void doesntContainInside();
    void doesntContainInsidePointFromHole();

private:
    Contour contour;
};

TestContour::TestContour() :
    contour()
{
    contour.addPoint(0, 0);
    contour.addPoint(100, 0);
    contour.addPoint(100, 100);
    contour.addPoint(0, 100);
    contour.addPoint(0, 90);
    contour.addPoint(10, 90);
    contour.addPoint(10, 10);
    contour.addPoint(0, 10);
}

void TestContour::containsInside()
{
    QVERIFY2(contour.containsInside(QPointF(50, 50)),
             "По какой-то причине контур не содержит точку, которую должен содержать!");
}

void TestContour::doesntContainInside()
{
    QVERIFY2(!contour.containsInside(QPointF(-10, -10)),
             "По какой-то причине контур содержит точку, которую должен не содержать!");
}

void TestContour::doesntContainInsidePointFromHole()
{
    QVERIFY2(!contour.containsInside(QPointF(0, 50)),
             "По какой-то причине контур содержит точку, которую должен не содержать!");
}

QTEST_APPLESS_MAIN(TestContour)

#include "tst_TestContour.moc"
