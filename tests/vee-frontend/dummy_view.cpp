#include "dummy_view.h"
#include <QWidget>

const QString DummyView::msInterface = "org.vee.DummyView";
const QString DummyView::msTitle = "DummyView";
const QString DummyView::msUrl = "dummy url";

DummyView::DummyView(QObject* parent):
    LocalView(parent) {
}

DummyView::~DummyView() {
}

QString DummyView::title() const {
    return msTitle;
}

QString DummyView::interface() const {
    return msInterface;
}

QString DummyView::url() const {
    return msUrl;
}

QWidget* DummyView::widget() {
    QWidget* blankWidget = new QWidget();
    blankWidget->setStyleSheet("background-color: #FFF");
    return blankWidget;
}

void DummyView::resolve(const QString & url) {
    if (url == msUrl || url == "")
        emit urlResolved();
    else
        emit urlNotResolved();
}
