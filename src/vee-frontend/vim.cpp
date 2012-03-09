#include "vim.h"
#include <QDebug>

Vim::Vim(Mode mode, QObject* parent) :
    QObject(parent),
    mMode(mode) {
}

Vim::~Vim() {
}

void Vim::setMode(Mode mode) {
    mMode = mode;
}

Vim::Mode Vim::mode() const {
    return mMode;
}


bool Vim::parse(const QString& command) {
    bool result;
    qDebug() << "Parsing " << command;
    if (!command.startsWith(":")) {
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
            result = false;
        }
    }
    return result;
}
