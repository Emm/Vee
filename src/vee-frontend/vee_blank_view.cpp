#include "vee_blank_view.h"
#include <QVBoxLayout>
#include <QLineEdit>
#include <QDebug>

const QString VeeBlankView::msInterface = "org.vee.BlankView";
const QString VeeBlankView::msTitle = "";
const QString VeeBlankView::msUrl = "about:blank";

VeeBlankView::VeeBlankView(QObject* parent):
    LocalView(parent) {
}

VeeBlankView::~VeeBlankView() {
}

QString VeeBlankView::title() const {
    return msTitle;
}

QString VeeBlankView::interface() const {
    return msInterface;
}

QString VeeBlankView::url() const {
    return msTitle;
}

QWidget* VeeBlankView::widget() {
    QWidget* blankWidget = new QWidget();
    blankWidget->setStyleSheet("background-color: #FFF");
    return blankWidget;
}

void VeeBlankView::resolve(const QString & url) {
    if (url == msUrl || url == "")
        emit urlResolved();
    else
        emit urlNotResolved();
}
