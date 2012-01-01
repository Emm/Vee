#include "web_view.h"
#include <QDBusPendingCall>

WebView::WebView(const ViewCommand & viewCommmand, Process* process, QObject* parent) :
    RemoteView(viewCommmand, process, parent) {
}

WebView::~WebView() {
}

void WebView::reload() {
    mRealInterface->asyncCall(QLatin1String("reload"));
}

void WebView::setHtml(const QString &html) {
    QList<QVariant> argumentList;
    argumentList << qVariantFromValue(html);
    mRealInterface->asyncCallWithArgumentList(QLatin1String("setHtml"), argumentList);
}

void WebView::updateIcon() {
    QByteArray bytes = qvariant_cast< QByteArray >(mRealInterface->property("iconImage"));
    QPixmap pixmap;
    pixmap.loadFromData(bytes, "ICO");
    mIcon = QIcon(pixmap);
    emit(iconChanged());
}

void WebView::remoteLoadFinished(bool ok) {
    // If the icon is null, force the update to a 'blank' icon
    if (RemoteView::icon().isNull()) {
        emit iconChanged();
    }
    RemoteView::remoteLoadFinished(ok);
}

QIcon WebView::icon() const {
    QIcon realIcon = RemoteView::icon();
    QIcon icon = realIcon.isNull() ? QIcon::fromTheme("text-html") : realIcon;
    return icon;
}

void WebView::stop() {
    mRealInterface->asyncCall(QLatin1String("stop"));
}
