#include <QString>
#include <QtTest>
#include <QTime>
#include <QDebug>

#include <QFile>

#include "graph.h"
#include "shortestpath.h"
#include "evnavrequest.h"
#include "elevationtile.h"

class GraphTest : public QObject
{
    Q_OBJECT

public:
    GraphTest();

private Q_SLOTS:
    void testCase1();
    void testCase2();
    void testCase3();
    void testCase4();
    void testCase5();
};

GraphTest::GraphTest()
{
}

void GraphTest::testCase1()
{
    Graph g;
    g.addEdge(Edge{1, 2, 1.0});
    g.addEdge(Edge{1, 3, 2.0});
    g.addEdge(Edge{2, 3, 3.0});
    g.addEdge(Edge{2, 1, 4.0});

    QList<Edge> lstAll = g.edges();
    QList<Edge> lstV1 = g.adj(1);
    QVERIFY2(lstAll.size() == 4, "total edges");
    QVERIFY2(lstV1.size() == 2, "edges of vertex 1");
    QVERIFY2(g.E() == 4, "total edges");
    QVERIFY2(g.V() == 3, "total vertices");
}

static Edge g1[] = {
    { 4, 5, 0.35 },
    { 5, 4, 0.35 },
    { 4, 7, 0.37 },
    { 5, 7, 0.28 },
    { 7, 5, 0.28 },
    { 5, 1, 0.32 },
    { 0, 4, 0.38 },
    { 0, 2, 0.26 },
    { 7, 3, 0.39 },
    { 1, 3, 0.29 },
    { 2, 7, 0.34 },
    { 6, 2, 0.40 },
    { 3, 6, 0.52 },
    { 6, 0, 0.58 },
    { 6, 4, 0.93 },
};
int g1n = sizeof(g1) / sizeof(Edge);

void GraphTest::testCase2()
{
    Graph g;
    for (int i = 0; i < g1n; ++i) {
        g.addEdge(g1[i]);
    }

    ShortestPath sp(g, 0);

    QVERIFY2(sp.hasPathTo(1), "path to v1 exists");
    QVERIFY2(std::fabs(sp.distTo(1) - 1.05) < 0.0001, "wrong distance to v1");
    QList<Edge> path = sp.pathTo(1);
    QVERIFY2(path.size() == 3, "path has 3 edges");

}

void GraphTest::testCase3()
{
    QUrl url("/route/v1/evnav/13.388860,52.517037;13.397634,52.529407?battery=18&SOC_act=0.8");
    EvnavRequest req;
    int ret = req.parseUrl(url);
    QVERIFY2(ret == 0, "return code indicates error");
    QVERIFY2(fabs(req.m_battery - 18.0) < 0.001, "wrong battery parameter");
    QVERIFY2(fabs(req.m_SOC_act - 0.8) < 0.001, "wrong SOC_act parameter");
}

void GraphTest::testCase4()
{
    QTime time(0, 0, 0);
    time = time.addSecs(1000);
    QVERIFY(QString{"0:16"}.compare(time.toString("h:m")) == 0);
}

void GraphTest::testCase5()
{
    //lat=45.5062487&lon=-73.5985226 alt=170
    ElevationTile tile;
    QString data = QFINDTESTDATA("../../res/L18/N45W073.hgt");
    qDebug() << data;
    tile.load(data);
    double alt = tile.query(45.5062487, -73.5985226);
    QVERIFY2(fabs(alt - 170) < 0.001, "wrong altitude");
}

QTEST_APPLESS_MAIN(GraphTest)

#include "tst_graphtest.moc"
