#include <QTest>
#include "view_tab.h"
#include "error_view.h"
#include "dummy_view_builder.h"
#include "dummy_view.h"

class TestViewTab : public QObject {

Q_OBJECT

private:

    ViewTab* mViewTab;

private slots:

    void init() {
        QVector<ViewBuilder*>* viewBuilders = new QVector<ViewBuilder*>();
        viewBuilders->append(new DummyViewBuilder());

        ViewResolver* viewResolver = new ViewResolver(viewBuilders);
        mViewTab = new ViewTab(new Vim(), viewResolver);
    }

    void testInitialView() {
        QVERIFY(mViewTab->view() == NULL);
    }

    void testSetUrl() {
        mViewTab->setUrl("dummy url");
        QVERIFY(mViewTab->view() != NULL);
        const DummyView* dummyView = dynamic_cast<const DummyView*>(mViewTab->view());
        QVERIFY(dummyView != NULL);
    }


    void testUnresolvableUrl() {
        mViewTab->setUrl("unresolvable url");
        QVERIFY(mViewTab->view() != NULL);
        const ErrorView* errorView = dynamic_cast<const ErrorView*>(mViewTab->view());
        QVERIFY(errorView != NULL);
    }

    void cleanup() {
        delete mViewTab;
    }
 
};

QTEST_MAIN(TestViewTab);
