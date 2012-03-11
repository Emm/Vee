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
    mScrollDownAction(mWebView),
    mScrollUpAction(mWebView),
    mScrollLeftAction(mWebView),
    mScrollRightAction(mWebView),
    mScrollToTopAction(mWebView),
    mScrollToBottomAction(mWebView),
    mWindowId(windowId),
    mScrollIncrement(scrollIncrement) {
    mWebView->addAction(& mScrollDownAction);
    mWebView->addAction(& mScrollUpAction);
    mWebView->addAction(& mScrollLeftAction);
    mWebView->addAction(& mScrollRightAction);
    mWebView->addAction(& mScrollToTopAction);
    mWebView->addAction(& mScrollToBottomAction);

    mScrollDownAction.setShortcut(QKeySequence(Qt::Key_J));
    mScrollUpAction.setShortcut(QKeySequence(Qt::Key_K));
    mScrollLeftAction.setShortcut(QKeySequence(Qt::Key_L));
    mScrollRightAction.setShortcut(QKeySequence(Qt::Key_H));
    mScrollToTopAction.setShortcut(QKeySequence(Qt::Key_G, Qt::Key_G));
    mScrollToBottomAction.setShortcut(QKeySequence(Qt::SHIFT + Qt::Key_G));

    connect(& mScrollDownAction, SIGNAL(triggered()), this, SLOT(scrollDown()));
    connect(& mScrollUpAction, SIGNAL(triggered()), this, SLOT(scrollUp()));
    connect(& mScrollLeftAction, SIGNAL(triggered()), this, SLOT(scrollLeft()));
    connect(& mScrollRightAction, SIGNAL(triggered()), this, SLOT(scrollRight()));
    connect(& mScrollToTopAction, SIGNAL(triggered()), this, SLOT(scrollToTop()));
    connect(& mScrollToBottomAction, SIGNAL(triggered()), this, SLOT(scrollToBottom()));

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

void WebViewProxy::scrollToTop() {
    QWebFrame* frame = getWebFrame();
    if (frame == NULL) return;
    frame->setScrollPosition(QPoint(0, 0));
}

void WebViewProxy::scrollToBottom() {
    QWebFrame* frame = getWebFrame();
    if (frame == NULL) return;
    QSize contentsSize = frame->contentsSize();
    int height = contentsSize.rheight();
    frame->setScrollPosition(QPoint(0, height));
}

void WebViewProxy::scroll(int dx, int dy) {
    QWebFrame* frame = getWebFrame();
    if (frame == NULL) return;
    frame->scroll(dx, dy);
}

QWebFrame* WebViewProxy::getWebFrame() {
    QWebPage* webPage = mWebView->page();
    QWebFrame* frame;
    if (webPage != NULL) {
        frame = webPage->currentFrame();
    }
    else {
        frame = NULL;
    }
    return frame;
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
