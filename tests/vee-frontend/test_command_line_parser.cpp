#include <QTest>
#include "command_line_parser.h"
#include "constants.h"

class TestCommandLineParser : public QObject {
Q_OBJECT

public:
    explicit TestCommandLineParser() {
    }


    virtual ~TestCommandLineParser() {
    }

private:
    QString* mAppName;
    QString* mAppVersion;
    CommandLineParser* mParser;

private slots:

    void initTestCase() {
        mAppName = new QString("test_app");
        mAppVersion = new QString("1.0");
        mParser = new CommandLineParser(*mAppName, *mAppVersion);
    }

    void cleanupTestCase() {
        delete mParser;
        delete mAppName;
        delete mAppVersion;
    }

    void testArguments() {
        int argc = 2;
        char command[] = "test-vee-frontend";
        char urlOrFile[] = "http://someurl";
        char* argv[] = {command, urlOrFile};
        int res = mParser->parse(argc, argv);

        QVERIFY(res == COMMANDLINE_PARSING_SUCCESS);
        QCOMPARE(mParser->urlOrFile(), QString(urlOrFile));

        mParser->reset();
    }
};

QTEST_MAIN(TestCommandLineParser);
