#include <QtTest/QtTest>
#include <QFileInfo>
#include <QTemporaryFile>
#include <QTextStream>

#include "vee_web_view.h"

/**
 * Test VeeWebView objects with local files.
 */
class TestVeeWebViewFile : public QObject {
Q_OBJECT

private:
    VeeWebView* mView;
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
        mHtmlFile = new QTemporaryFile("test_vee_web_view_file_XXXXXX.HTML");
        if (mHtmlFile->open()) {
            QTextStream out(mHtmlFile);
            out << "<html><body>test</body></html>";
        }
        mHtmlFile->close();
        mView = new VeeWebView();
        connect(mView, SIGNAL(loadFinished(bool)), this, SLOT(setSuccess(bool)));
    };

    void cleanup() {
        delete mView;
        delete mHtmlFile;
    };

    void testAbsolutePath() {
        QFileInfo fileInfo(*mHtmlFile);
        QString absPath = fileInfo.absoluteFilePath();
        mSuccess = false;
        mView->loadUrlOrPath(absPath);
        QTest::qWait(1000);
        QCOMPARE(mSuccess, true);
    };

    void testRelativePath() {
        QFileInfo fileInfo(*mHtmlFile);
        QVERIFY(fileInfo.isRelative() == true);
        QString relPath = fileInfo.filePath();
        mSuccess = false;
        mView->loadUrlOrPath(relPath);
        QTest::qWait(1000);
        QCOMPARE(mSuccess, true);
    };
};

QTEST_MAIN(TestVeeWebViewFile)
