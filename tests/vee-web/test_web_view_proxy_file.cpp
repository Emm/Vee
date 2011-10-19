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
    void setSuccess(bool success) {
        mSuccess = success;
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
        connect(mProxy, SIGNAL(loadFinished(bool)), this, SLOT(setSuccess(bool)));
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
        QVERIFY(fileInfo.isRelative() == true);
        QString relPath = fileInfo.filePath();
        mSuccess = false;
        mProxy->resolve(relPath);
        QTest::qWait(1000);
        QCOMPARE(mSuccess, true);
    };
};

QTEST_MAIN(TestWebViewProxyFile)
