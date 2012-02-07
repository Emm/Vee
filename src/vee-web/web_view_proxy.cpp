#include "web_view_proxy.h"

#include <QFile>
#include <QFileInfo>
#include <QVBoxLayout>
#include <QWebFrame>
#include <QBuffer>

#include "constants.h"

WebViewProxy::WebViewProxy(ulong windowId, int scrollIncrement, QObject* parent) :
    QObject(parent),
    mWebView(new QWebView()),
    mEmbedWidget(NULL),
    mScrollDownAction(new QAction(mWebView)),
    mScrollUpAction(new QAction(mWebView)),
    mScrollLeftAction(new QAction(mWebView)),
    mScrollRightAction(new QAction(mWebView)),
    mWindowId(windowId),
    mScrollIncrement(scrollIncrement) {
    mWebView->addAction(mScrollDownAction);
    mWebView->addAction(mScrollUpAction);
    mWebView->addAction(mScrollLeftAction);
    mWebView->addAction(mScrollRightAction);

    mScrollDownAction->setShortcut(QKeySequence(Qt::Key_J));
    mScrollUpAction->setShortcut(QKeySequence(Qt::Key_K));
    mScrollLeftAction->setShortcut(QKeySequence(Qt::Key_L));
    mScrollRightAction->setShortcut(QKeySequence(Qt::Key_H));

    connect(mScrollDownAction, SIGNAL(triggered()), this, SLOT(scrollDown()));
    connect(mScrollUpAction, SIGNAL(triggered()), this, SLOT(scrollUp()));
    connect(mScrollLeftAction, SIGNAL(triggered()), this, SLOT(scrollLeft()));
    connect(mScrollRightAction, SIGNAL(triggered()), this, SLOT(scrollRight()));

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

void WebViewProxy::scrollDown() {
    scroll(0, mScrollIncrement);
}

void WebViewProxy::scrollUp() {
    scroll(0, -1 * mScrollIncrement);
}

void WebViewProxy::scrollLeft() {
    scroll(mScrollIncrement, 0);
}

void WebViewProxy::scrollRight() {
    scroll(-1 * mScrollIncrement, 0);
}

void WebViewProxy::scroll(int dx, int dy) {
    QWebPage* webPage = mWebView->page();
    if (webPage == NULL) return;
    QWebFrame* frame = webPage->currentFrame();
    if (frame == NULL) return;
    frame->scroll(dx, dy);
}

void WebViewProxy::embed() {
    if (shouldEmbed()) {
        // Only create the embed widget now, otherwise we're going to run into
        // odd focus issues if the widget is created earlier
        mEmbedWidget = new QX11EmbedWidget();
        mEmbedWidget->setLayout(new QVBoxLayout());
        mEmbedWidget->layout()->setMargin(0);
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
    emit loadFinished(ok);
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

QByteArray WebViewProxy::iconImage() const {
    QPixmap iconPixmap = mWebView->icon().pixmap(16,16);
    QByteArray bytes;
    QBuffer buffer(&bytes);
    buffer.open(QIODevice::WriteOnly);
    iconPixmap.save(&buffer, "ICO");
    return bytes;
}

void WebViewProxy::show() {
    mWebView->show();
}
