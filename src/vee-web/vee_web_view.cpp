#include "vee_web_view.h"

#include <QFile>
#include <QFileInfo>
#include <QVBoxLayout>
#include <QDebug>

#include "constants.h"

VeeWebView::VeeWebView(ulong windowId, QWidget* parent) :
    QX11EmbedWidget(parent),
    mWebView(new QWebView()),
    /*mEmbedWidget(NULL),*/
    mWindowId(windowId) {
        setVisible(false);
    connect(mWebView, SIGNAL(loadFinished(bool)), this, SLOT(broadcastLoadFinished(bool)));
    connect(mWebView, SIGNAL(iconChanged()), this, SIGNAL(iconChanged()));
    connect(mWebView, SIGNAL(linkClicked(const QUrl &)), this, SIGNAL(linkClicked(const QUrl &)));
    connect(mWebView, SIGNAL(loadProgress(int)), this, SIGNAL(loadProgress(int)));
    connect(mWebView, SIGNAL(loadStarted()), this, SIGNAL(loadStarted()));
    connect(mWebView, SIGNAL(selectionChanged()), this, SIGNAL(selectionChanged()));
    connect(mWebView, SIGNAL(titleChanged(const QString &)), this, SIGNAL(titleChanged(const QString &)));
    connect(mWebView, SIGNAL(urlChanged(const QUrl &)), this, SIGNAL(urlChanged(const QUrl &)));
    setLayout(new QVBoxLayout());
    if (shouldEmbed()) {
        /*mEmbedWidget = new QX11EmbedWidget();
        mEmbedWidget->setLayout(new QVBoxLayout());
        mEmbedWidget->layout()->addWidget(mWebView);
        layout()->addWidget(mEmbedWidget);*/
        layout()->addWidget(mWebView);
    }
    else {
        layout()->addWidget(mWebView);
    }
}

VeeWebView::~VeeWebView() {
}

bool VeeWebView::shouldEmbed() {
    return mWindowId != NULL_WINDOW_ID;
}

void VeeWebView::embed() {
    if (shouldEmbed()) {
        embedInto(mWindowId);
        show();
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
