#include "dummy_remote_web_view_adaptor.h"

DummyRemoteWebViewAdaptor::DummyRemoteWebViewAdaptor(DummyRemoteWebView* parent) : QDBusAbstractAdaptor(parent) {
    setAutoRelaySignals(true);
}

DummyRemoteWebViewAdaptor::~DummyRemoteWebViewAdaptor() {}

DummyRemoteWebView *DummyRemoteWebViewAdaptor::parent() const {
    return static_cast<DummyRemoteWebView *>(QObject::parent());
}

QString DummyRemoteWebViewAdaptor::title() const {
    return qvariant_cast< QString >(parent()->property("title"));
}

QString DummyRemoteWebViewAdaptor::url() const {
    return qvariant_cast< QString >(parent()->property("url"));
}

void DummyRemoteWebViewAdaptor::embed() {
    parent()->embed();
}

void DummyRemoteWebViewAdaptor::resolve(const QString & url) {
    parent()->resolve(url);
}

void DummyRemoteWebViewAdaptor::reload() {
    parent()->reload();
}

void DummyRemoteWebViewAdaptor::setHtml(const QString & html) {
    parent()->setHtml(html);
}

void DummyRemoteWebViewAdaptor::stop() {
    parent()->stop();
}
