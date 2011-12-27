#include <QTest>
#include "view_resolver.h"
#include "blank_view_builder.h"
#include "blank_view.h"
#include "dummy_view_builder.h"

class TestViewResolver : public QObject {

Q_OBJECT

private:

    ViewResolver* mViewResolver;
    View* mView;
    View* mCurrentView;
    QString* mUnresolvableUrl;

public slots:

    void setView(View* view) {
        mView = view;
    }

    void setUnresolvableUrl(QString url) {
        mUnresolvableUrl = new QString(url);
    }

private slots:

    void init() {
        mView = NULL;
        mUnresolvableUrl = NULL;

        mCurrentView = new BlankView();
        QVector<ViewBuilder*>* viewBuilders = new QVector<ViewBuilder*>();
        DummyViewBuilder* dummyBuilder = new DummyViewBuilder(); 
        viewBuilders->append(dummyBuilder);
        BlankViewBuilder* blankBuilder = new BlankViewBuilder();
        viewBuilders->append(blankBuilder);
        mViewResolver = new ViewResolver(viewBuilders);
        connect(mViewResolver, SIGNAL(urlResolved(View*)), this, SLOT(setView(View*)));
        connect(mViewResolver, SIGNAL(unresolvableUrl(QString)), this, SLOT(setUnresolvableUrl(QString)));
    }

    void testIdentifier() {
        mViewResolver->setIdentifier(1L);
        QVERIFY(mViewResolver->identifier() == 1L);
    }

    void testResolutionNoCurrentView() {
        QString url("about:blank");
        mViewResolver->resolve(url);

        QVERIFY(mView != NULL);
        BlankView* blankView = dynamic_cast<BlankView*>(mView);
        QVERIFY(blankView != NULL);
    }

    void testResolutionWithCurrentView() {
        QString url("about:blank");
        mViewResolver->resolve(url, mCurrentView);
        QVERIFY(mView == mCurrentView);
        BlankView* blankView = dynamic_cast<BlankView*>(mView);
        QVERIFY(blankView != NULL);
    }

    void testFailedResolution() {
        QString url("dont resolve me");
        mViewResolver->resolve(url, mCurrentView);
        QVERIFY(mView == NULL);
        QVERIFY(mUnresolvableUrl != NULL);
        QVERIFY(*mUnresolvableUrl == url);
    }

    void cleanup() {
        delete mViewResolver;
        if (mView != mCurrentView)
            delete mCurrentView;
        delete mView;
        delete mUnresolvableUrl;
    }
};

QTEST_MAIN(TestViewResolver);
