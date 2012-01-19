#include <QTest>
#include <QtGui>
#include "view_tab.h"
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

    void testInitialMode() {
        QVERIFY(mViewTab->vim()->mode() == Vim::NormalMode);
    }

    void testChangeToCommandMode() {
        mViewTab->show();
        mViewTab->setUrl("about:blank");
        QTest::qWait(100);
        QTest::keyClicks(mViewTab->widget(), ":");
        QVERIFY(mViewTab->vim()->mode() == Vim::CommandMode);
    }

    void testChangeBackToNormalMode() {
        mViewTab->show();
        mViewTab->setUrl("dummy url");
        QTest::qWait(100);
        QTest::keyClicks(mViewTab->widget(), ":");
        QTest::qWait(100);
        QTest::keyClicks(mViewTab->inputBar(), "o about:blank");
        QTest::keyClick(mViewTab->inputBar(), Qt::Key_Enter);
        QVERIFY(mViewTab->vim()->mode() == Vim::NormalMode);
    }

    void testDefaultFocus() {
        QVERIFY(mViewTab->focusWidget() == mViewTab->inputBar());
    }

    void testViewChange() {
        mViewTab->show();
        mViewTab->setUrl("dummy url");
        QTest::qWait(100);
        QTest::keyClicks(mViewTab->widget(), ":");
        QTest::qWait(100);
        QTest::keyClicks(mViewTab->inputBar(), "o about:blank");
        QTest::keyClick(mViewTab->inputBar(), Qt::Key_Enter);
        QTest::qWait(100);
        const BlankView* dummyView = dynamic_cast<const BlankView*>(mViewTab->view());
        QVERIFY(dummyView != NULL);
    }

    void testOpenViewInNewTab() {
        mViewTab->show();
        mViewTab->setUrl("dummy url");
        QTest::qWait(100);
        QTest::keyClicks(mViewTab->widget(), ":");
        QTest::qWait(100);
        QTest::keyClicks(mViewTab->inputBar(), "t about:blank");
        QTest::keyClick(mViewTab->inputBar(), Qt::Key_Enter);
        QTest::qWait(100);
        QVERIFY(mOpenInNewTabUrl == QString("about:blank"));
    }

    void cleanup() {
        delete mViewTab;
    }

};

QTEST_MAIN(TestViewTab);
