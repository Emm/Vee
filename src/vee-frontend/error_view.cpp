#include "error_view.h"
#include <QLabel>

const QString ErrorView::msInterface = "org.vee.ErrorView";
const QString ErrorView::msTitle = "Error";
const QString ErrorView::msUrl = "";

ErrorView::ErrorView(const QString errorMessage, QObject* parent):
    LocalView(parent),
    mErrorMessage(errorMessage) {
    mIcon = QIcon::fromTheme("dialog-error", QIcon(":/icons/dialog-error"));
}

ErrorView::~ErrorView() {
}

QString ErrorView::title() const {
    return msTitle;
}

QString ErrorView::interface() const {
    return msInterface;
}

QString ErrorView::url() const {
    return msUrl;
}

QWidget* ErrorView::widget() {
    QLabel* errorWidget = new QLabel(mErrorMessage);
    errorWidget->setAlignment(Qt::AlignTop);
    return errorWidget;
}

void ErrorView::resolve(const QString & url) {
    emit urlResolved();
}
