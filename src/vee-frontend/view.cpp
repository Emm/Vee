#include "view.h"

View::View(QObject* parent) : QObject(parent) {
}

View::~View() {
}

void View::init(const ulong identifier) {
    emit initialized();
}
