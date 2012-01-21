#include <QTest>
#include "embed_command.h"

class TestEmbedCommand : public QObject {

Q_OBJECT
private:
    EmbedCommand* mCommand;
    QString mExecutable;
    QString mArg1;
    QString mArg2;

private slots:

    void init() {
        mExecutable = QString("vee-frontend-test");
        mArg1 = QString("arg1");
        mArg2 = QString("arg2");
        mCommand = new EmbedCommand(mExecutable);
        mCommand->addArgument(mArg1);
        mCommand->addWinId();
        mCommand->addArgument(mArg2);
    }

    void testCommand() {
        const QString & executable = mCommand->executable();
        QVERIFY(executable == "vee-frontend-test");

        ulong winId = 42ul;

        QStringList arguments = mCommand->arguments(winId); 
        QVERIFY(arguments.length() == 3);
        QVERIFY(arguments.at(0) == mArg1);
        QVERIFY(arguments.at(1) == QString("42"));
        QVERIFY(arguments.at(2) == mArg2);
    }

    void cleanup() {
        delete mCommand;
    }
};

QTEST_MAIN(TestEmbedCommand);
