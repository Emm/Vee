#include "vee_blank_view.h"
#include <QVBoxLayout>
#include <QLineEdit>
#include <QDebug>

const QString VeeBlankView::msInterface = "org.vee.BlankView";
const QString VeeBlankView::msTitle = "";
const QString VeeBlankView::msUrl = "about:blank";

VeeBlankView::VeeBlankView(QObject* parent):
    VeeLocalView(parent),
    mWidget(new QWidget()) {
    mWidget->setLayout(new QVBoxLayout());
    mWidget->layout()->addWidget(new QLineEdit("mooh"));
}

VeeBlankView::~VeeBlankView() {
    qDebug() << "bye";
    delete mWidget;
}

QWidget* VeeBlankView::widget() const {
    return mWidget;
}

QString VeeBlankView::title() const {
    qDebug() << "title";
    return msTitle;
}

QString VeeBlankView::interface() const {
    qDebug() << "interface";
    return msInterface;
}

QString VeeBlankView::url() const {
    qDebug() << "url";
    return msTitle;
}

QWidget* VeeBlankView::widget() {
    return mWidget;
}

void VeeBlankView::resolve(const QString & url) {
    if (url == msUrl || url == "")
        emit urlResolved();
    else
        emit urlNotResolved();
}
