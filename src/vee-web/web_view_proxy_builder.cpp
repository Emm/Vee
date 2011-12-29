#include "web_view_proxy_builder.h"

#include <QTextStream>
#include <QWebSettings>
#include <QDesktopServices>
#include <QDir>
#include <QDebug>

WebViewProxyBuilder::WebViewProxyBuilder(const QString & urlOrFile, const ulong windowId, const DBusManager* dbusManager) : mUrlOrFile(urlOrFile), mWindowId(windowId), mDBusManager(dbusManager)  {
}

WebViewProxyBuilder::~WebViewProxyBuilder() {
}

WebViewProxy* WebViewProxyBuilder::build() {
    WebViewProxy* view = buildView();
    return view;
}

WebViewProxy* WebViewProxyBuilder::buildView() {
    QString dataLocation = QDesktopServices::storageLocation(QDesktopServices::DataLocation);
    QDir dataDir(dataLocation);
    if (!dataDir.exists()) {
        if (!dataDir.mkpath(dataLocation))
            qDebug() << "Error: could not create the local storage directory " << dataLocation;
    }
    if (dataDir.exists())
        QWebSettings::setIconDatabasePath(dataLocation);

    WebViewProxy* view = new WebViewProxy(mWindowId);
    if (shouldEmbed())
        mDBusManager->registerWidget(*view);
    if (mUrlOrFile.compare("-") == 0) {
        const QString & html = readHtmlFromStdin();
        view->setHtml(html);
    }
    else if (!mUrlOrFile.isEmpty()) {
        view->resolve(mUrlOrFile);
    }
    return view;
}

/**
 Builds HTML from the contents of stdin
*/
const QString & WebViewProxyBuilder::readHtmlFromStdin() const {
    QString* html = new QString();
    QTextStream stdinStream(stdin);
    QString line;
    do {
        line = stdinStream.readLine();
        html->append(line);
    }
    while (!line.isNull());
    return *html;
}

bool WebViewProxyBuilder::shouldEmbed() const {
    return mWindowId != 0 && mDBusManager != NULL;
}
