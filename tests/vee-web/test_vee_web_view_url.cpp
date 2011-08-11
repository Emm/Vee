#include <QtTest/QtTest>
#include "vee_web_view.h"

/**
 * Tests View object with URLs
 */
class TestVeeWebViewUrl: public QObject {
Q_OBJECT

private:
    VeeWebView* mView;
    bool mSuccess;

public slots:
    void setSuccess(bool success);

private slots:
    void init();
    void testCorrectUrl();
    void testWrongUrl();
    void cleanup();
};

void TestVeeWebViewUrl::init() {
    mView = new VeeWebView();
    connect(mView, SIGNAL(loadFinished(bool)), this, SLOT(setSuccess(bool)));
}

void TestVeeWebViewUrl::testCorrectUrl() {
    mSuccess = false;
    mView->loadUrlOrPath(QString("about:blank"));
    QTest::qWait(10);
    QCOMPARE(mSuccess, true);
}

void TestVeeWebViewUrl::testWrongUrl() {
    mSuccess = true;
    mView->loadUrlOrPath(QString("abut:wrongurl"));
    QTest::qWait(10);
    QCOMPARE(mSuccess, false);
}

void TestVeeWebViewUrl::cleanup() {
    delete mView;
}

void TestVeeWebViewUrl::setSuccess(bool success) {
    mSuccess = success;
}

QTEST_MAIN(TestVeeWebViewUrl)
