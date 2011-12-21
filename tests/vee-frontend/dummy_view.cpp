#include "dummy_view.h"
#include <QWidget>

DummyView::DummyView(QObject* parent):
    LocalView(parent) {
}

DummyView::~DummyView() {
}

QString DummyView::title() const {
    return "DummyView";
}

QString DummyView::interface() const {
    return "org.vee.DummyView";
}

QString DummyView::url() const {
    return "dummy url";
}

QWidget* DummyView::widget() {
    QWidget* blankWidget = new QWidget();
    blankWidget->setStyleSheet("background-color: #FFF");
    return blankWidget;
}

void DummyView::resolve(const QString & url) {
    if (url == "dummy url" || url == "")
        emit urlResolved();
    else
        emit urlNotResolved();
}
