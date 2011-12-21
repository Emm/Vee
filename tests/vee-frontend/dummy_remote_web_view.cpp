#include "dummy_remote_web_view.h"

DummyRemoteWebView::DummyRemoteWebView(QObject* parent) :
    QObject(parent),
    mEmbedded(false),
    mReloaded(false),
    mStopped(false),
    mResolvedUrl(NULL),
    mHtml(NULL) {
};

DummyRemoteWebView::~DummyRemoteWebView() {
    delete mResolvedUrl;
};

QString DummyRemoteWebView::title() const {
    return QString("myTitle");
}

QString DummyRemoteWebView::url() const {
    return QString("myUrl");
}

void DummyRemoteWebView::emitUrlResolved() {
    emit urlResolved();
}

void DummyRemoteWebView::emitUrlNotResolved() {
    emit urlNotResolved();
}

void DummyRemoteWebView::emitUrlChanged(const QString & url) {
    emit urlChanged(url);
}

void DummyRemoteWebView::emitTitleChanged(const QString & title) {
    emit titleChanged(title);
}

void DummyRemoteWebView::emitError(int errorType, int errorCode) {
    emit error(errorType, errorCode);
}

bool DummyRemoteWebView::embedded() const {
    return mEmbedded;
}

bool DummyRemoteWebView::reloaded() const {
    return mReloaded;
}

bool DummyRemoteWebView::stopped() const {
    return mStopped;
}

QString * DummyRemoteWebView::resolvedUrl() const {
    return mResolvedUrl;
}

QString * DummyRemoteWebView::html() const {
    return mHtml;
}

void DummyRemoteWebView::embed() {
    mEmbedded = true;
}

void DummyRemoteWebView::resolve(const QString & url) {
    mResolvedUrl = new QString(url);
}

void DummyRemoteWebView::reload() {
    mReloaded = true;
}

void DummyRemoteWebView::setHtml(const QString & html) {
    mHtml = new QString(html);
}

void DummyRemoteWebView::stop() {
    mStopped = true;
}
