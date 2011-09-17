#include "vee_web_view.h"

#include <QFile>
#include <QFileInfo>
#include <QDebug>

VeeWebView::VeeWebView(QWidget* parent) : QWebView(parent) {
}

void VeeWebView::resolve(const QString &value) {
    QUrl url;
    QFileInfo fileInfo(value);
    QString fileName = fileInfo.fileName();
    // Check for extension in order not to anger QWebView
    if (fileInfo.isFile() && (fileName.endsWith(".html", Qt::CaseInsensitive)
                || fileName.endsWith(".htm", Qt::CaseInsensitive))) {
        QString absPath = fileInfo.absoluteFilePath();
        url = QUrl::fromLocalFile(absPath);
    }
    else
        url = QUrl::fromUserInput(value);
    load(url);
}
