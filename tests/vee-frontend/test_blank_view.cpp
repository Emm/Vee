#include <QTest>
#include "blank_view.h"

class TestBlankView : public QObject {

Q_OBJECT

private:
    BlankView* mView;
    int mResolved;

public slots:
    void blankViewResolved() {
        mResolved = 1;
    }

    void blankViewNotResolved() {
        mResolved = 0;
    }

private slots:

    void init() {
        mView = new BlankView();
        mResolved = -1;

        connect(mView, SIGNAL(urlResolved()), this, SLOT(blankViewResolved()));
        connect(mView, SIGNAL(urlNotResolved()), this, SLOT(blankViewNotResolved()));
    }

    void testWidget() {
        QWidget* widget = mView->widget();
        QVERIFY(widget != NULL);
    }

    void testResolveEmptyUrl() {
        mView->resolve("");
        QVERIFY(mResolved == 1);
    }

    void testResolveAboutBlankUrl() {
        mView->resolve("about:blank");
        QVERIFY(mResolved == 1);
    }

    void testResolveAnotherUrl() {
        mView->resolve("localhost");
        QVERIFY(mResolved == 0);
    }

    void testTitle() {
        QVERIFY(mView->title() == QString(""));
    }


    void testInterface() {
        QVERIFY(mView->interface() == "org.vee.BlankView");
    }

    void testUrl() {
        QVERIFY(mView->url() == QString("about:blank"));
    }

    void cleanup() {
        disconnect(mView);
        delete mView;
    }

};

QTEST_MAIN(TestBlankView);
