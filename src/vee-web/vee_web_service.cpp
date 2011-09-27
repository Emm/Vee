#include "vee_web_service.h"

#include <QFile>
#include <QFileInfo>
#include <QVBoxLayout>

#include "constants.h"

VeeWebService::VeeWebService(ulong windowId, QObject* parent) :
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
}

VeeWebService::~VeeWebService() {
}

bool VeeWebService::shouldEmbed() {
    return mWindowId != NULL_WINDOW_ID;
}

void VeeWebService::embed() {
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

void VeeWebService::resolve(const QString &value) {
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

void VeeWebService::broadcastLoadFinished(bool ok) {
    if (ok)
        emit urlResolved();
    else
        emit urlNotResolved();
}

void VeeWebService::reload() {
    mWebView->reload();
}

void VeeWebService::stop() {
    mWebView->stop();
}

void VeeWebService::setHtml(const QString & html) {
    mWebView->setHtml(html);
}

QUrl VeeWebService::url() const {
    return mWebView->url();
}

QString VeeWebService::title() const {
    return mWebView->title();
}

void VeeWebService::show() {
    mWebView->show();
}
