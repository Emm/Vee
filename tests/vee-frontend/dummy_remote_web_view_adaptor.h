#ifndef DUMMY_REMOTE_WEB_VIEW_ADAPTOR
#define DUMMY_REMOTE_WEB_VIEW_ADAPTOR

#include <QDBusAbstractAdaptor>
#include "dummy_remote_web_view.h"

class DummyRemoteWebViewAdaptor : public QDBusAbstractAdaptor {

Q_OBJECT
Q_CLASSINFO("D-Bus Interface", "org.vee.WebView")

public:

    explicit DummyRemoteWebViewAdaptor(DummyRemoteWebView* parent);

    virtual ~DummyRemoteWebViewAdaptor();

    DummyRemoteWebView *parent() const;

    Q_PROPERTY(QString title READ title)
    QString title() const;

    Q_PROPERTY(QString url READ url)
    QString url() const;

public slots:
    void embed();

    void resolve(const QString & url);

    void reload();

    void setHtml(const QString & html);

    void stop();

signals:

    void error(int errorType, int errorCode);
    void iconChanged();
    void linkClicked(const QString &url);
    void loadProgress(int progress);
    void loadStarted();
    void selectionChanged();
    void titleChanged(const QString &title);
    void urlChanged(const QString &url);
    void urlNotResolved();
    void urlResolved();
};

#endif
