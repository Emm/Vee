#include <QTest>
#include <QtGui>
#include <QProcess>
#include "view_tab.h"
#include "assert.h"
#include "error_view.h"
#include "dummy_view_builder.h"
#include "blank_view_builder.h"
#include "dummy_view.h"
#include "blank_view.h"

class TestViewTab : public QObject {

Q_OBJECT

private:

    ViewTab* mViewTab;
    QString mOpenInNewTabUrl;

public slots:

    void urlOpenedInNewTab(const QString & url) {
        mOpenInNewTabUrl = url;
    }

private slots:

    void init() {
        QVector<ViewBuilder*>* viewBuilders = new QVector<ViewBuilder*>();
        viewBuilders->append(new DummyViewBuilder());
        viewBuilders->append(new BlankViewBuilder());

        ViewResolver* viewResolver = new ViewResolver(viewBuilders);
        mViewTab = new ViewTab(new Vim(), viewResolver);
        mOpenInNewTabUrl = QString();

        connect(mViewTab, SIGNAL(openInNewTab(const QString &)), this, SLOT(urlOpenedInNewTab(const QString &)));
    }

    void testInitialView() {
        QVERIFY(mViewTab->view() == NULL);
    }

    void testnewUrl() {
        mViewTab->newUrl("dummy url");
        QVERIFY(mViewTab->view() != NULL);
        const DummyView* dummyView = dynamic_cast<const DummyView*>(mViewTab->view());
        QVERIFY(dummyView != NULL);
    }


    void testUnresolvableUrl() {
        mViewTab->newUrl("unresolvable url");
        QVERIFY(mViewTab->view() != NULL);
        const ErrorView* errorView = dynamic_cast<const ErrorView*>(mViewTab->view());
        QVERIFY(errorView != NULL);
    }

    void testDefaultFocus() {
        QVERIFY(mViewTab->focusWidget() == mViewTab->inputBar());
    }

    void testViewChangeWithVimShortcut() {
        mViewTab->show();
        mViewTab->newUrl("dummy url");
        QTest::qWait(500);
        QTest::keyClicks(mViewTab->widget(), ":");
        QTest::qWait(500);
        QTest::keyClicks(mViewTab->inputBar(), "o about:blank");
        QTest::keyClick(mViewTab->inputBar(), Qt::Key_Enter);
        QTest::qWait(500);
        const BlankView* blankView = dynamic_cast<const BlankView*>(mViewTab->view());
        QVERIFY(blankView != NULL);
    }

    void testViewChangeWithFocus() {
        mViewTab->show();
        QTest::mouseClick(mViewTab->inputBar(), Qt::LeftButton);
        QTest::qWait(500);
        QTest::keyClicks(mViewTab->inputBar(), "about:blank");
        QTest::keyClick(mViewTab->inputBar(), Qt::Key_Enter);
        QTest::qWait(500);
        const BlankView* blankView = dynamic_cast<const BlankView*>(mViewTab->view());
        QVERIFY(blankView != NULL);
    }

    void testViewChangeWithFocusAndVimCommand() {
        mViewTab->show();
        QTest::mouseClick(mViewTab->inputBar(), Qt::LeftButton);
        QTest::qWait(500);
        QTest::keyClicks(mViewTab->inputBar(), ":o about:blank");
        QTest::keyClick(mViewTab->inputBar(), Qt::Key_Enter);
        QTest::qWait(500);
        const BlankView* blankView = dynamic_cast<const BlankView*>(mViewTab->view());
        QVERIFY(blankView != NULL);
    }

    void testOpenViewInNewTab() {
        mViewTab->show();
        mViewTab->newUrl("dummy url");
        QTest::qWait(500);
        QTest::keyClicks(mViewTab->widget(), ":");
        QTest::qWait(500);
        QTest::keyClicks(mViewTab->inputBar(), "t about:blank");
        QTest::keyClick(mViewTab->inputBar(), Qt::Key_Enter);
        QTest::qWait(500);
        QVERIFY(mOpenInNewTabUrl == QString("about:blank"));
    }

    void testViewProcessError() {
        mViewTab->newUrl("dummy url");
        assert(mViewTab->view() != NULL);
        QTest::qWait(100);
        DummyView* dummyView = dynamic_cast<DummyView*>(mViewTab->view());
        assert(dummyView != NULL);
        dummyView->emitError(View::ProcessError, QProcess::Crashed);
        QTest::qWait(100);
        QVERIFY(mViewTab->view() != NULL);
        const ErrorView* errorView = dynamic_cast<const ErrorView*>(mViewTab->view());
        QVERIFY(errorView != NULL);
    }

    void testGoBack() {
        mViewTab->show();
        mViewTab->newUrl("dummy url");
        QTest::qWait(500);
        QTest::keyClicks(mViewTab->widget(), ":");
        QTest::qWait(500);
        QTest::keyClicks(mViewTab->inputBar(), "o about:blank");
        QTest::keyClick(mViewTab->inputBar(), Qt::Key_Enter);
        QTest::qWait(500);
        const BlankView* blankView = dynamic_cast<const BlankView*>(mViewTab->view());
        QVERIFY(blankView != NULL);

        QTest::qWait(500);

        QTest::keyClick(mViewTab->widget(), Qt::Key_O, Qt::ControlModifier);
        QTest::qWait(500);
        const DummyView* dummyView = dynamic_cast<const DummyView*>(mViewTab->view());
        QVERIFY(dummyView != NULL);
    }

    void testGoBackAndForward() {
        mViewTab->show();
        mViewTab->newUrl("dummy url");
        QTest::qWait(500);
        QTest::keyClicks(mViewTab->widget(), ":");
        QTest::qWait(500);
        QTest::keyClicks(mViewTab->inputBar(), "o about:blank");
        QTest::keyClick(mViewTab->inputBar(), Qt::Key_Enter);
        QTest::qWait(500);
        const BlankView* blankViewFromInputBar = dynamic_cast<const BlankView*>(mViewTab->view());
        QVERIFY(blankViewFromInputBar != NULL);

        QTest::qWait(500);

        QTest::keyClick(mViewTab->widget(), Qt::Key_O, Qt::ControlModifier);
        QTest::qWait(500);
        const DummyView* dummyView = dynamic_cast<const DummyView*>(mViewTab->view());
        QVERIFY(dummyView != NULL);

        QTest::keyClick(mViewTab->widget(), Qt::Key_I, Qt::ControlModifier);
        QTest::qWait(500);
        const BlankView* blankViewFromHistory = dynamic_cast<const BlankView*>(mViewTab->view());
        QVERIFY(blankViewFromHistory != NULL);
    }

    void cleanup() {
        delete mViewTab;
    }

};

QTEST_MAIN(TestViewTab);
