#include <QTest>
#include "vim.h"

class TestVim : public QObject {

Q_OBJECT

private:

    Vim* mVim;
    QString* mOpenUrl;
    QString* mOpenInNewTabUrl;
    bool mCloseTab;

public slots:

    void setOpenUrl(QString url) {
        mOpenUrl = new QString(url);
    }

    void setOpenInNewTabUrl(QString url) {
        mOpenInNewTabUrl = new QString(url);
    }

    void setCloseTab() {
        mCloseTab = true;
    }

private slots:

    void init() {
        mVim = new Vim();
        mOpenUrl = NULL;
        mOpenInNewTabUrl= NULL;
        mCloseTab = false;
        connect(mVim, SIGNAL(openCommand(QString)), this, SLOT(setOpenUrl(QString)));
        connect(mVim, SIGNAL(openInNewTabCommand(QString)), this, SLOT(setOpenInNewTabUrl(QString)));
        connect(mVim, SIGNAL(closeTabCommand()), this, SLOT(setCloseTab()));
    }


    void testDefaultMode() {
        QVERIFY(mVim->mode() == Vim::NormalMode);
    }

    void testSetMode() {
        mVim->setMode(Vim::CommandMode);
        QVERIFY(mVim->mode() == Vim::CommandMode);
    }

    void testParseOpenCommand_data() {
        QTest::addColumn<QString>("command");
        QTest::addColumn<QString>("result");
        QTest::newRow("long form") << "open about:blank" << "about:blank";
        QTest::newRow("long form with spaces") << "open   about:blank   " << "about:blank";
        QTest::newRow("short form") << "o about:blank" << "about:blank";
        QTest::newRow("short form with spaces") << "o   about:blank  " << "about:blank";
    }

    void testParseOpenCommand() {
        QFETCH(QString, command);
        QFETCH(QString, result);

        QVERIFY(mVim->parse(command));
        QVERIFY(mOpenUrl == QString(result));
        QVERIFY(mOpenInNewTabUrl == NULL);
    }

    void testParseOpenInNewTabCommand_data() {
        QTest::addColumn<QString>("command");
        QTest::addColumn<QString>("result");
        QTest::newRow("long form") << "tab about:blank" << "about:blank";
        QTest::newRow("long form with spaces") << "tab   about:blank   " << "about:blank";
        QTest::newRow("short form") << "t about:blank" << "about:blank";
        QTest::newRow("short form with spaces") << "t   about:blank  " << "about:blank";
    }

    void testParseOpenInNewTabCommand() {
        QFETCH(QString, command);
        QFETCH(QString, result);

        QVERIFY(mVim->parse(command));
        QVERIFY(mOpenInNewTabUrl == QString(result));
        QVERIFY(mOpenUrl == NULL);
    }

    void testParseCloseTabCommand() {
        QFETCH(QString, command);

        QVERIFY(mVim->parse(command));
        QVERIFY(mCloseTab == true);
    }

    void testParseCloseTabCommand_data() {
        QTest::addColumn<QString>("command");
        QTest::newRow("long form") << "quit";
        QTest::newRow("short form") << "q";
    }

    void cleanup() {
        delete mVim;
        delete mOpenUrl;
        delete mOpenInNewTabUrl;
    }
};

QTEST_MAIN(TestVim);
