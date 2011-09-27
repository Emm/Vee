#include <QtTest/QtTest>
#include "vee_web_service.h"

/**
 * Tests View object with URLs
 */
class TestVeeWebServiceUrl: public QObject {
Q_OBJECT

private:
    VeeWebService* mView;
    bool mSuccess;

public slots:
    void setSuccess(bool success) {
        mSuccess = success;
    };

private slots:
    void init() {
        mView = new VeeWebService();
        connect(mView, SIGNAL(loadFinished(bool)), this, SLOT(setSuccess(bool)));
    };

    void testCorrectUrl() {
        mSuccess = false;
        mView->resolve(QString("about:blank"));
        QTest::qWait(10);
        QCOMPARE(mSuccess, true);
    };
 
    void testWrongUrl() {
        mSuccess = true;
        mView->resolve(QString("abut:wrongurl"));
        QTest::qWait(10);
        QCOMPARE(mSuccess, false);
    };

    void cleanup() {
        delete mView;
    };
};

QTEST_MAIN(TestVeeWebServiceUrl)
