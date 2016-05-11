#include <QString>
#include <QtTest>

class GraphTest : public QObject
{
    Q_OBJECT

public:
    GraphTest();

private Q_SLOTS:
    void testCase1();
};

GraphTest::GraphTest()
{
}

void GraphTest::testCase1()
{
    QVERIFY2(true, "Failure");
}

QTEST_APPLESS_MAIN(GraphTest)

#include "tst_graphtest.moc"
