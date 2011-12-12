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
    if (command.startsWith("open ") || command.startsWith("o ")) {
        emit openCommand(command.section(' ', 1));
        result = true;
    }
    else {
        result = false;
    }
    return result;
}