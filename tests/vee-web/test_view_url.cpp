#include <QtTest/QtTest>
#include "view.h"

/**
 * Tests View object with URLs
 */
class TestViewUrl: public QObject {
Q_OBJECT

private:
    View* mView;
    bool mSuccess;

public slots:
    void setSuccess(bool success);

private slots:
    void init();
    void testCorrectUrl();
    void testWrongUrl();
    void cleanup();
};

void TestViewUrl::init() {
    mView = new View();
    connect(mView, SIGNAL(loadFinished(bool)), this, SLOT(setSuccess(bool)));
}

void TestViewUrl::testCorrectUrl() {
    mSuccess = false;
    mView->loadUrlOrPath(QString("about:blank"));
    QTest::qWait(10);
    QCOMPARE(mSuccess, true);
}

void TestViewUrl::testWrongUrl() {
    mSuccess = true;
    mView->loadUrlOrPath(QString("abut:wrongurl"));
    QTest::qWait(10);
    QCOMPARE(mSuccess, false);
}

void TestViewUrl::cleanup() {
    delete mView;
}

void TestViewUrl::setSuccess(bool success) {
    mSuccess = success;
}

QTEST_MAIN(TestViewUrl)
