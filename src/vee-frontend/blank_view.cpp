#include "blank_view.h"
#include <QVBoxLayout>
#include <QLineEdit>
#include <QDebug>

const QString BlankView::msInterface = "org.vee.BlankView";
const QString BlankView::msTitle = "";
const QString BlankView::msUrl = "about:blank";

BlankView::BlankView(QObject* parent):
    LocalView(parent) {
}

BlankView::~BlankView() {
}

QString BlankView::title() const {
    return msTitle;
}

QString BlankView::interface() const {
    return msInterface;
}

QString BlankView::url() const {
    return msTitle;
}

QWidget* BlankView::widget() {
    QWidget* blankWidget = new QWidget();
    blankWidget->setStyleSheet("background-color: #FFF");
    return blankWidget;
}

void BlankView::resolve(const QString & url) {
    if (url == msUrl || url == "")
        emit urlResolved();
    else
        emit urlNotResolved();
}
