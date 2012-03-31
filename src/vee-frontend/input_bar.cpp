#include "input_bar.h"

InputBar::InputBar(QWidget* parent) : QLineEdit(parent) {
}

InputBar::InputBar(const QString & contents, QWidget* parent) : QLineEdit(contents, parent) {
}

InputBar::~InputBar() {
}

void InputBar::keyPressEvent(QKeyEvent* event) {
    if (event->key() == Qt::Key_Escape) {
        clearFocus();
        emit escapePressed();
    }
    else
        QLineEdit::keyPressEvent(event);
}
