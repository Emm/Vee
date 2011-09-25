#include "vee_web_view.h"

#include <QFile>
#include <QFileInfo>
#include <QVBoxLayout>
#include <QDebug>

#include "constants.h"

VeeWebView::VeeWebView(ulong windowId, QObject* parent) :
    QObject(parent),
    mWebView(new QWebView()),
    /*mEmbedWidget(NULL),*/
    mWindowId(windowId) {
    connect(mWebView, SIGNAL(loadFinished(bool)), this, SLOT(broadcastLoadFinished(bool)));
    connect(mWebView, SIGNAL(iconChanged()), this, SIGNAL(iconChanged()));
    connect(mWebView, SIGNAL(linkClicked(const QUrl &)), this, SIGNAL(linkClicked(const QUrl &)));
    connect(mWebView, SIGNAL(loadProgress(int)), this, SIGNAL(loadProgress(int)));
    connect(mWebView, SIGNAL(loadStarted()), this, SIGNAL(loadStarted()));
    connect(mWebView, SIGNAL(selectionChanged()), this, SIGNAL(selectionChanged()));
    connect(mWebView, SIGNAL(titleChanged(const QString &)), this, SIGNAL(titleChanged(const QString &)));
    connect(mWebView, SIGNAL(urlChanged(const QUrl &)), this, SIGNAL(urlChanged(const QUrl &)));
    if (shouldEmbed()) {
        /*mEmbedWidget = new QX11EmbedWidget();
        mEmbedWidget->setLayout(new QVBoxLayout());
        mEmbedWidget->layout()->addWidget(mWebView);
        layout()->addWidget(mEmbedWidget);*/
    }
}

VeeWebView::~VeeWebView() {
}

bool VeeWebView::shouldEmbed() {
    return mWindowId != NULL_WINDOW_ID;
}

void VeeWebView::embed() {
    if (shouldEmbed()) {
        QX11EmbedWidget* widget = new QX11EmbedWidget();
        widget->setLayout(new QVBoxLayout());
        widget->layout()->addWidget(mWebView);
        widget->embedInto(mWindowId);
        widget->show();
    }
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
    mWebView->load(url);
}

void VeeWebView::broadcastLoadFinished(bool ok) {
    qDebug() << "broadcastLoadFinished(" << ok << ")";
    if (ok)
        emit urlResolved();
    else
        emit urlNotResolved();
}

void VeeWebView::reload() {
    mWebView->reload();
}

void VeeWebView::stop() {
    mWebView->stop();
}

void VeeWebView::setHtml(const QString & html) {
    mWebView->setHtml(html);
}

QUrl VeeWebView::url() const {
    return mWebView->url();
}

QString VeeWebView::title() const {
    return mWebView->title();
}
