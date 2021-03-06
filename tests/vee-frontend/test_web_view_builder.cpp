#include <QTest>
#include <QTemporaryFile>
#include "web_view_builder.h"
#include "web_view.h"
#include "dummy_process_builder.h"
#include "dummy_remote_web_view_adaptor.h"
#include "dummy_remote_web_view.h"

#define TEST_SERVICE_ID "org.vee.WebView_0"
#define TEST_SERVICE_PATH "/TestWebView"
#define WEB_VIEW_INTERFACE "org.vee.WebView"

class TestWebViewBuilder : public QObject {

Q_OBJECT

private:

    WebViewBuilder* mBuilder;
    View* mView;
    ViewCommand* mViewCommand;
    ProcessBuilder* mProcessBuilder;
    DummyRemoteWebView* mRemoteView;
    QTemporaryFile* mDummyExe;

public slots:

    void viewBuilt(View* view) {
        mView = view;
    }

private slots:

    void init() {

        mView = NULL;
        mDummyExe = new QTemporaryFile();
        // Generates the temporary executable file
        if (mDummyExe->open()) {
            mDummyExe->close();
        }
        mDummyExe->setPermissions(QFile::ExeOwner);

        EmbedCommand* command = new EmbedCommand(mDummyExe->fileName());
        command->addArgument("-w");
        command->addWinId();

        mViewCommand = new ViewCommand(*command, "org.vee.WebView", "org.vee.WebView_%1", "/WebView");

        mProcessBuilder = new DummyProcessBuilder();

        // We need to handle the dbus stuff in the test since the process is a
        // dummy
        mRemoteView = new DummyRemoteWebView();
        new DummyRemoteWebViewAdaptor(mRemoteView);

        mBuilder = new WebViewBuilder(*mViewCommand, *mProcessBuilder, this);
        connect(mBuilder, SIGNAL(viewBuilt(View*)), this, SLOT(viewBuilt(View*)));
    }

    void testBuild() {
        mBuilder->build(0L);

        QDBusConnection dbus = QDBusConnection::sessionBus();
        dbus.registerService(TEST_SERVICE_ID);
        dbus.registerObject(TEST_SERVICE_PATH, mRemoteView);

        QTest::qWait(100);
        QVERIFY(mView != NULL);
        // Test we really got a web view
        WebView* webView = dynamic_cast<WebView*>(mView);
        QVERIFY(webView != NULL);
    }

    void testViewType() {
        QVERIFY(mBuilder->viewType() == "org.vee.WebView");
    }

    void cleanup() {
        QDBusConnection dbus = QDBusConnection::sessionBus();
        dbus.unregisterObject(TEST_SERVICE_PATH);
        dbus.unregisterService(TEST_SERVICE_ID);
        delete mViewCommand;
        delete mView;
        delete mBuilder;
        delete mProcessBuilder;
        delete mRemoteView;
        delete mDummyExe;
        QTest::qWait(100);
    }
};

QTEST_MAIN(TestWebViewBuilder);
