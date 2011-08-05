#include "view.h"

#include <QFile>
#include <QDebug>
View::View(QWidget* parent) : QWebView(parent) {
}

void View::loadUrlOrPath(const QString &value) {
    QUrl url;

    if (QFile::exists(value))
        url = QUrl::fromLocalFile(value);
    else
        url = QUrl::fromUserInput(value);
    // FIXME loading from a local url doesn't work anymore
    load(url);
}
