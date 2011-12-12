#include <QTest>
#include <QDBusConnection>
#include <QDBusAbstractAdaptor>
#include "web_view.h"
#include "process.h"

#define TEST_SERVICE_ID "org.vee.TestWebView"
#define TEST_SERVICE_PATH "/TestWebView"
#define WEB_VIEW_INTERFACE "org.vee.WebView"

class TestProcess : public Process {

Q_OBJECT

private:

    const QString* mExecutable;
    const QStringList* mArguments;

public:

    explicit TestProcess() : Process() {}

    virtual ~TestProcess() {}

    virtual void start(const QString & executable, const QStringList & arguments) {
        mExecutable = &executable;
        mArguments = &arguments;
    }

    const QString* executable() {
        return mExecutable;
    }

    const QStringList* arguments() {
        return mArguments;
    }

public slots:

    virtual void terminate() {};

    virtual QProcess::ProcessState state() const {
        return QProcess::NotRunning;
    }
};

class TestRemoteWebView: public QObject {
Q_OBJECT

private:
    bool mEmbedded;
    bool mReloaded;
    bool mStopped;
    QString * mResolvedUrl;
    QString * mHtml;

public:
    explicit TestRemoteWebView(QObject* parent = 0) :
        QObject(parent),
        mEmbedded(false),
        mReloaded(false),
        mStopped(false),
        mResolvedUrl(NULL),
        mHtml(NULL) {
    };

    virtual ~TestRemoteWebView() {
        delete mResolvedUrl;
    };

    Q_PROPERTY(QString title READ title)
    QString title() const {
        return QString("myTitle");
    }

    Q_PROPERTY(QString url READ url)
    QString url() const {
        return QString("myUrl");
    }

    void emitUrlResolved() {
        emit urlResolved();
    }

    void emitUrlNotResolved() {
        emit urlNotResolved();
    }

    void emitUrlChanged(const QString & url) {
        emit urlChanged(url);
    }

    void emitTitleChanged(const QString & title) {
        emit titleChanged(title);
    }

    void emitError(int errorType, int errorCode) {
        emit error(errorType, errorCode);
    }

    bool embedded() const {
        return mEmbedded;
    }

    bool reloaded() const {
        return mReloaded;
    }

    bool stopped() const {
        return mStopped;
    }

    QString * resolvedUrl() const {
        return mResolvedUrl;
    }

    QString * html() const {
        return mHtml;
    }

public slots:

    void embed() {
        mEmbedded = true;
    }

    void resolve(const QString & url) {
        mResolvedUrl = new QString(url);
    }

    void reload() {
        mReloaded = true;
    }

    void setHtml(const QString & html) {
        mHtml = new QString(html);
    }

    void stop() {
        mStopped = true;
    }

signals:

    void error(int errorType, int errorCode);
    void iconChanged();
    void linkClicked(const QString &url);
    void loadProgress(int progress);
    void loadStarted();
    void selectionChanged();
    void titleChanged(const QString &title);
    void urlChanged(const QString &url);
    void urlNotResolved();
    void urlResolved();
};

class TestRemoteWebViewAdaptor : public QDBusAbstractAdaptor {

Q_OBJECT
Q_CLASSINFO("D-Bus Interface", "org.vee.WebView")

public:

    explicit TestRemoteWebViewAdaptor(TestRemoteWebView* parent) : QDBusAbstractAdaptor(parent) {
        setAutoRelaySignals(true);
    }

    virtual ~TestRemoteWebViewAdaptor() {}

    inline TestRemoteWebView *parent() const {
        return static_cast<TestRemoteWebView *>(QObject::parent());
    }

    Q_PROPERTY(QString title READ title)
    QString title() const {
        return qvariant_cast< QString >(parent()->property("title"));
    }

    Q_PROPERTY(QString url READ url)
    QString url() const {
        return qvariant_cast< QString >(parent()->property("url"));
    }

public slots:
    void embed() {
        parent()->embed();
    }

    void resolve(const QString & url) {
        parent()->resolve(url);
    }

    void reload() {
        parent()->reload();
    }

    void setHtml(const QString & html) {
        parent()->setHtml(html);
    }

    void stop() {
        parent()->stop();
    }

signals:

    void error(int errorType, int errorCode);
    void iconChanged();
    void linkClicked(const QString &url);
    void loadProgress(int progress);
    void loadStarted();
    void selectionChanged();
    void titleChanged(const QString &title);
    void urlChanged(const QString &url);
    void urlNotResolved();
    void urlResolved();
};

class TestWebView : public QObject {

Q_OBJECT

private:

    WebView* mView;
    ViewCommand* mViewCommand;
    TestRemoteWebView* mRemoteView;
    TestProcess* mProcess;
    QString* mNewUrl;
    QString* mNewTitle;
    int mUrlResolved;
    View::ErrorType mErrorType;
    int mErrorCode;

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

        EmbedCommand* command = new EmbedCommand(QString("dummy_embed_command"));
        command->addArgument("-w");
        command->addWinId();

        mViewCommand = new ViewCommand;
        mViewCommand->embedCommand = command;
        mViewCommand->interfaceName += WEB_VIEW_INTERFACE;
        mViewCommand->serviceIdPattern += TEST_SERVICE_ID;
        mViewCommand->objectPath += TEST_SERVICE_PATH;
        mNewUrl = NULL;
        mUrlResolved = -1;
        mErrorType = View::ProcessError;
        mErrorCode = -1;

        mProcess = new TestProcess();
        mView = new WebView(*mViewCommand, mProcess, this);
        mView->init(0ul);
        mRemoteView = new TestRemoteWebView();
        new TestRemoteWebViewAdaptor(mRemoteView);
        QDBusConnection dbus = QDBusConnection::sessionBus();
        dbus.registerService(TEST_SERVICE_ID);
        dbus.registerObject(TEST_SERVICE_PATH, mRemoteView);
        QTest::qWait(100);
    }

    void testProcessStarted() {
        QVERIFY(mProcess->executable() == mViewCommand->embedCommand->executable());
        QVERIFY(*(mProcess->arguments()) == *(mViewCommand->embedCommand->arguments(0ul)));
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
        mProcess = NULL;
        QTest::qWait(100);
    }
};

QTEST_MAIN(TestWebView)
