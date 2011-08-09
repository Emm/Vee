#include <QFileInfo>
#include <QTemporaryFile>
#include <QTextStream>
#include <QtTest/QtTest>

#include "view.h"

/**
 * Test View objects with local files.
 */
class TestViewFile : public QObject {
Q_OBJECT
private:
    View* mView;
    QTemporaryFile* mHtmlFile;
    bool mSuccess;

public slots:
    void setSuccess(bool success);
private slots:
    void init();
    void cleanup();
    void testAbsolutePath();
    void testRelativePath();
};

void TestViewFile::init() {
    // Relative filename (otherwise we can't test relative path), with html
    // extension (otherwise QWebView won't load the file)
    mHtmlFile = new QTemporaryFile("test_view_file_XXXXXX.HTML");
    if (mHtmlFile->open()) {
        QTextStream out(mHtmlFile);
        out << "<html><body>test</body></html>";
    }
    mHtmlFile->close();
    mView = new View();
    connect(mView, SIGNAL(loadFinished(bool)), this, SLOT(setSuccess(bool)));
}

void TestViewFile::testAbsolutePath() {
    QFileInfo fileInfo(*mHtmlFile);
    QString absPath = fileInfo.absoluteFilePath(); 
    mSuccess = false;
    mView->loadUrlOrPath(absPath);
    QTest::qWait(1000);
    QCOMPARE(mSuccess, true);
}

void TestViewFile::testRelativePath() {
    QFileInfo fileInfo(*mHtmlFile);
    QVERIFY(fileInfo.isRelative() == true);
    QString relPath = fileInfo.filePath(); 
    mSuccess = false;
    mView->loadUrlOrPath(relPath);
    QTest::qWait(1000);
    QCOMPARE(mSuccess, true);
}

void TestViewFile::cleanup() {
    delete mView;
    delete mHtmlFile;
}

void TestViewFile::setSuccess(bool success) {
    mSuccess = success;
}

QTEST_MAIN(TestViewFile)
