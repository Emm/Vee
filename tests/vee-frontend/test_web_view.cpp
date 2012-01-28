#include <QTest>
#include <QTemporaryFile>
#include <QDBusConnection>
#include "web_view.h"
#include "dummy_process.h"
#include "dummy_remote_web_view.h"
#include "dummy_remote_web_view_adaptor.h"

#define TEST_SERVICE_ID "org.vee.TestWebView"
#define TEST_SERVICE_PATH "/TestWebView"
#define WEB_VIEW_INTERFACE "org.vee.WebView"

class TestWebView : public QObject {

Q_OBJECT

private:

    WebView* mView;
    ViewCommand* mViewCommand;
    DummyRemoteWebView* mRemoteView;
    DummyProcess* mProcess;
    QString* mNewUrl;
    QString* mNewTitle;
    int mUrlResolved;
    View::ErrorType mErrorType;
    int mErrorCode;
    QTemporaryFile* mDummyExe;

public slots:

    void urlResolved() {
        mUrlResolved = 1;
    }

    void urlNotResolved() {
        mUrlResolved = 0;
    }

    void urlChanged(const QString & url) {
        mNewUrl = new QString(url);
    }

    void titleChanged(const QString & title) {
        mNewTitle = new QString(title);
    }

    void error(View::ErrorType errorType, int errorCode) {
        mErrorType = errorType;
        mErrorCode = errorCode;
    }

private slots:

    void init() {

        mDummyExe = new QTemporaryFile();
        // Generates the temporary executable file
        if (mDummyExe->open()) {
            mDummyExe->close();
        }
        mDummyExe->setPermissions(QFile::ExeOwner);

        EmbedCommand* command = new EmbedCommand(mDummyExe->fileName());
        command->addArgument("-w");
        command->addWinId();

        mViewCommand = new ViewCommand;
        mViewCommand->embedCommand = command;
        mViewCommand->interfaceName = QString(WEB_VIEW_INTERFACE);
        mViewCommand->serviceIdPattern = QString(TEST_SERVICE_ID);
        mViewCommand->objectPath = QString(TEST_SERVICE_PATH);
        mNewUrl = NULL;
        mUrlResolved = -1;
        mErrorType = View::ProcessError;
        mErrorCode = -1;

        mProcess = new DummyProcess();
        mView = new WebView(*mViewCommand, mProcess, this);
        mView->init(0ul);
        mRemoteView = new DummyRemoteWebView();
        new DummyRemoteWebViewAdaptor(mRemoteView);
        QDBusConnection dbus = QDBusConnection::sessionBus();
        dbus.registerService(TEST_SERVICE_ID);
        dbus.registerObject(TEST_SERVICE_PATH, mRemoteView);
        QTest::qWait(100);
    }

    void testProcessStarted() {
        QVERIFY(mProcess->executable() == mViewCommand->embedCommand->executable());
        QVERIFY(mProcess->arguments() == mViewCommand->embedCommand->arguments(0ul));
    }

    void testInterface() {
        QVERIFY(mView->interface() == QString(WEB_VIEW_INTERFACE)); 
    }

    void testTitle() {
        QVERIFY(mView->title() == QString("myTitle")); 
    }

    void testUrl() {
        QVERIFY(mView->url() == QString("myUrl")); 
    }

    void testEmbed() {
        mView->embed();
        QTest::qWait(100);
        QVERIFY(mRemoteView->embedded() == true);
    }

    void testReload() {
        mView->reload();
        QTest::qWait(100);
        QVERIFY(mRemoteView->reloaded() == true);
    }

    void testResolve() {
        QString* urlToResolve = new QString("url to resolve");
        mView->resolve(*urlToResolve);
        QTest::qWait(100);
        QVERIFY(*mRemoteView->resolvedUrl() == *urlToResolve);
        delete urlToResolve;
    }

    void testSetHtml() {
        QString* html = new QString("<html/>");
        mView->setHtml(*html);
        QTest::qWait(100);
        QVERIFY(*mRemoteView->html() == *html);
        delete html;
    }

    void testStop() {
        mView->stop();
        QTest::qWait(100);
        QVERIFY(mRemoteView->stopped() == true);
    }

    void testUrlResolved() {
        connect(mView, SIGNAL(urlResolved()), this, SLOT(urlResolved()));
        mRemoteView->emitUrlResolved();
        QTest::qWait(100);
        QVERIFY(mUrlResolved == 1);
    }

    void testUrlNotResolved() {
        connect(mView, SIGNAL(urlNotResolved()), this, SLOT(urlNotResolved()));
        mRemoteView->emitUrlNotResolved();
        QTest::qWait(100);
        QVERIFY(mUrlResolved == 0);
    }

    void testUrlChanged() {
        connect(mView, SIGNAL(urlChanged(const QString &)), this, SLOT(urlChanged(const QString &)));
        QString* newUrl = new QString("new url");
        mRemoteView->emitUrlChanged(*newUrl);
        QTest::qWait(100);
        QDBusConnection dbus = QDBusConnection::sessionBus();
        QVERIFY(mNewUrl != NULL);
        QVERIFY(*mNewUrl == *newUrl);
        delete newUrl;
    }

    void testTitleChanged() {
        connect(mView, SIGNAL(titleChanged(const QString &)), this, SLOT(titleChanged(const QString &)));
        QString* newTitle = new QString("new title");
        mRemoteView->emitTitleChanged(*newTitle);
        QTest::qWait(100);
        QDBusConnection dbus = QDBusConnection::sessionBus();
        QVERIFY(mNewTitle != NULL);
        QVERIFY(*mNewTitle == *newTitle);
        delete newTitle;
    }

    void testError() {
        connect(mView, SIGNAL(error(View::ErrorType, int)), this, SLOT(error(View::ErrorType, int)));
        int errorType = 256;
        int errorCode = 42;
        mRemoteView->emitError(errorType, errorCode);
        QTest::qWait(100);
        QDBusConnection dbus = QDBusConnection::sessionBus();
        QVERIFY(mErrorType == View::UnknownError);
        QVERIFY(mErrorCode == errorCode);
    }

    void testProcessError() {
        connect(mView, SIGNAL(error(View::ErrorType, int)), this, SLOT(error(View::ErrorType, int)));
        // Ensure error reported by the process are not propagated
        mProcess->emitError(QProcess::Crashed);
        QTest::qWait(100);
        QVERIFY(mErrorCode == -1);
    }

    void testProcessExit() {
        connect(mView, SIGNAL(error(View::ErrorType, int)), this, SLOT(error(View::ErrorType, int)));
        mProcess->exit();
        QTest::qWait(100);
        QVERIFY(mErrorType == View::ProcessError);
        QVERIFY(mErrorCode == QProcess::Crashed);
    }

    void testProcessCrash() {
        connect(mView, SIGNAL(error(View::ErrorType, int)), this, SLOT(error(View::ErrorType, int)));
        mProcess->crash();
        QTest::qWait(100);
        QVERIFY(mErrorType == View::ProcessError);
        QVERIFY(mErrorCode == QProcess::Crashed);
    }


    void cleanup() {
        disconnect(mView);
        disconnect(mRemoteView);
        QDBusConnection dbus = QDBusConnection::sessionBus();
        dbus.unregisterObject(TEST_SERVICE_PATH);
        dbus.unregisterService(TEST_SERVICE_ID);
        delete mViewCommand->embedCommand;
        delete mViewCommand;
        delete mRemoteView;
        delete mView;
        delete mDummyExe;
        mProcess = NULL;
        QTest::qWait(100);
    }
};

QTEST_MAIN(TestWebView)
