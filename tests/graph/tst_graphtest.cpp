#include <QString>
#include <QtTest>
#include <QDebug>

#include "graph.h"
#include "shortestpath.h"
#include "evnavrequest.h"
#include "chargerprovider.h"

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
    void testLoadCharger();
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

#include <QTime>

void GraphTest::testCase4()
{
    QTime time(0, 0, 0);
    time = time.addSecs(1000);
    QVERIFY(QString{"0:16"}.compare(time.toString("h:m")) == 0);
}

void GraphTest::testLoadCharger()
{
    ChargerProvider provider;
    // TODO: do not hardcode the path to the file
    QString path(TOPSRCDIR "/res/result_circuit_electrique_20161019165359.json");
    provider.loadJson(path);
    QVERIFY2(provider.size() > 0, "failed to load the charger list");
    ChargerProvider dcfc = provider.filter(ChargerProvider::fastChargerFilter);
    QVERIFY2(dcfc.size() > 0, "failed to filter chargers");
}

QTEST_APPLESS_MAIN(GraphTest)

#include "tst_graphtest.moc"
