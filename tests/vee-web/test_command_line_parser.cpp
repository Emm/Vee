#include <QTest>
#include "command_line_parser.h"
#include "constants.h"

class TestCommandLineParser : public QObject {
Q_OBJECT

public:
    explicit TestCommandLineParser() {}

    virtual ~TestCommandLineParser() {}

private slots:
    void testArgumentsForEmbedding() {
        int argc = 4;
        char command[] = "test-vee-web";
        char option[] = "-w";
        char windowId[] = "5234";
        char urlOrFile[] = "http://someurl";
        char* argv[] = {command, option, windowId, urlOrFile};
        CommandLineParser parser(QString("test_app"), QString("1.0"));
        int res = parser.parse(argc, argv);

        QVERIFY(res == COMMANDLINE_PARSING_SUCCESSFUL);
        QCOMPARE(parser.urlOrFile(), QString(urlOrFile));
        QCOMPARE(parser.windowId(), 5234ul);
    }

    void testArgumentsForStandaloneApp() {
        int argc = 2;
        char command[] = "test-vee-web";
        char urlOrFile[] = "http://someurl";
        char* argv[] = {command, urlOrFile};
        CommandLineParser parser(QString("test_app"), QString("1.0"));
        int res = parser.parse(argc, argv);

        QVERIFY(res == COMMANDLINE_PARSING_SUCCESSFUL);
        QCOMPARE(parser.urlOrFile(), QString(urlOrFile));
        QCOMPARE(parser.windowId(), 0ul);
    }
};

QTEST_MAIN(TestCommandLineParser);
