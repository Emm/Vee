#include "vim.h"
#include <QDebug>

Vim::Vim(QObject* parent) :
    QObject(parent) {
}

Vim::~Vim() {
}

bool Vim::parse(QString command) {
    bool result;
    qDebug() << "Parsing " << command;
    if (!command.startsWith(":")) {
        emit prefixMissing(command);
        result = false;
    }
    else {
        const QString & realCommand = QString(command).remove(0, 1).trimmed();
        if (realCommand.startsWith("open ") || realCommand.startsWith("o ")) {
            emit openCommand(realCommand.section(' ', 1).trimmed());
            result = true;
        }
        else if (realCommand.startsWith("tab ") || realCommand.startsWith("t ")) {
            emit openInNewTabCommand(realCommand.section(' ', 1).trimmed());
            result = true;
        }
        else if (realCommand == "quit" || realCommand == "q") {
            emit closeTabCommand();
            result = true;
        }
        else {
            emit parsingFailed(command);
            result = false;
        }
    }
    return result;
}
