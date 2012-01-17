#include "blank_view.h"

const QString BlankView::msInterface = "org.vee.BlankView";
const QString BlankView::msTitle = "";
const QString BlankView::msUrl = "about:blank";

BlankView::BlankView(QObject* parent):
    LocalView(parent) {
    mIcon = QIcon::fromTheme("go-home", QIcon(":/icons/go-home"));
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
    return msUrl;
}

QWidget* BlankView::widget() {
    QWidget* blankWidget = new QWidget();
    return blankWidget;
}

void BlankView::resolve(const QString & url) {
    if (url == msUrl || url == "")
        emit urlResolved();
    else
        emit urlNotResolved();
}
