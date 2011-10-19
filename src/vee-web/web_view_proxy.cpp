#include "web_view_proxy.h"

#include <QFile>
#include <QFileInfo>
#include <QVBoxLayout>

#include "constants.h"

WebViewProxy::WebViewProxy(ulong windowId, QObject* parent) :
    QObject(parent),
    mWebView(new QWebView()),
    mEmbedWidget(NULL),
    mWindowId(windowId) {
    connect(mWebView, SIGNAL(loadFinished(bool)), this, SLOT(broadcastLoadFinished(bool)));
    connect(mWebView, SIGNAL(iconChanged()), this, SIGNAL(iconChanged()));
    connect(mWebView, SIGNAL(linkClicked(const QUrl &)), this, SIGNAL(linkClicked(const QUrl &)));
    connect(mWebView, SIGNAL(loadProgress(int)), this, SIGNAL(loadProgress(int)));
    connect(mWebView, SIGNAL(loadStarted()), this, SIGNAL(loadStarted()));
    connect(mWebView, SIGNAL(selectionChanged()), this, SIGNAL(selectionChanged()));
    connect(mWebView, SIGNAL(titleChanged(const QString &)), this, SIGNAL(titleChanged(const QString &)));
    connect(mWebView, SIGNAL(urlChanged(const QUrl &)), this, SIGNAL(urlChanged(const QUrl &)));
    connect(mWebView, SIGNAL(urlChanged(const QUrl &)), this, SIGNAL(urlResolved()));
}

WebViewProxy::~WebViewProxy() {
}

bool WebViewProxy::shouldEmbed() {
    return mWindowId != NULL_WINDOW_ID;
}

void WebViewProxy::embed() {
    if (shouldEmbed()) {
        // Only create the embed widget now, otherwise we're going to run into
        // odd focus issues if the widget is created earlier
        mEmbedWidget = new QX11EmbedWidget();
        mEmbedWidget->setLayout(new QVBoxLayout());
        mEmbedWidget->layout()->addWidget(mWebView);
        mEmbedWidget->embedInto(mWindowId);
        mEmbedWidget->show();
    }
}

void WebViewProxy::resolve(const QString &value) {
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
    mWebView->load(url);
}

void WebViewProxy::broadcastLoadFinished(bool ok) {
    if (!ok)
        emit urlNotResolved();
}

void WebViewProxy::reload() {
    mWebView->reload();
}

void WebViewProxy::stop() {
    mWebView->stop();
}

void WebViewProxy::setHtml(const QString & html) {
    mWebView->setHtml(html);
}

QUrl WebViewProxy::url() const {
    return mWebView->url();
}

QString WebViewProxy::title() const {
    return mWebView->title();
}

void WebViewProxy::show() {
    mWebView->show();
}