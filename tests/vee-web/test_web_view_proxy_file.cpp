#include <QtTest/QtTest>
#include <QFileInfo>
#include <QTemporaryFile>
#include <QTextStream>

#include "web_view_proxy.h"

/**
 * Test WebViewProxy objects with local files.
 */
class TestWebViewProxyFile : public QObject {
Q_OBJECT

private:
    WebViewProxy* mProxy;
    QTemporaryFile* mHtmlFile;
    bool mSuccess;

public slots:
    void succeeded() {
        mSuccess = true;
    };

    void failed() {
        mSuccess = false;
    };

private slots:
    void init() {
        // Relative filename (otherwise we can't test relative path), with html
        // extension (otherwise QWebView won't load the file)
        mHtmlFile = new QTemporaryFile("test_web_view_proxy_XXXXXX.HTML");
        if (mHtmlFile->open()) {
            QTextStream out(mHtmlFile);
            out << "<html><body>test</body></html>";
        }
        mHtmlFile->close();
        mProxy = new WebViewProxy();
        connect(mProxy, SIGNAL(urlResolved()), this, SLOT(succeeded()));
        connect(mProxy, SIGNAL(urlNotResolved()), this, SLOT(failed()));
    };

    void cleanup() {
        delete mProxy;
        delete mHtmlFile;
    };

    void testAbsolutePath() {
        QFileInfo fileInfo(*mHtmlFile);
        QString absPath = fileInfo.absoluteFilePath();
        mSuccess = false;
        mProxy->resolve(absPath);
        QTest::qWait(1000);
        QCOMPARE(mSuccess, true);
    };

    void testRelativePath() {
        QFileInfo fileInfo(*mHtmlFile);
        QString relPath = fileInfo.filePath();
        mSuccess = false;
        mProxy->resolve(relPath);
        QTest::qWait(1000);
        QCOMPARE(mSuccess, true);
    };
};

QTEST_MAIN(TestWebViewProxyFile)
