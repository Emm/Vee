#include <QTest>
#include "blank_view_builder.h"
#include "blank_view.h"

class TestBlankViewBuilder : public QObject {

Q_OBJECT
private:
    BlankViewBuilder* mBuilder;
    View* mView;

public slots:
    void viewBuilt(View* view) {
        mView = view;
    }

private slots:

    void init() {
        mBuilder = new BlankViewBuilder();
        mView = NULL;
        connect(mBuilder, SIGNAL(viewBuilt(View*)), this, SLOT(viewBuilt(View*)));
    }

    void testBuild() {
        mBuilder->build(0L);
        QVERIFY(mView != NULL);
        // Test we really got a blank view
        BlankView* blankView = dynamic_cast<BlankView*>(mView);
        QVERIFY(blankView != NULL);
    }

    void testViewType() {
        QVERIFY(mBuilder->viewType() == "org.vee.BlankView");
    }

    void cleanup() {
        disconnect(mBuilder);
        delete mBuilder;
        delete mView;
    }
};

QTEST_MAIN(TestBlankViewBuilder);
