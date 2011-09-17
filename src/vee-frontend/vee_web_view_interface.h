#ifndef VEE_WEB_VIEW_INTERFACE_H
#define VEE_WEB_VIEW_INTERFACE_H

#include "vee_view_interface.h"

class VeeWebViewInterface : public VeeViewInterface {
    Q_OBJECT

public:
    explicit VeeWebViewInterface(QProcess* process, const QString &service, const QString &path, const QString & interfaceName, const QDBusConnection &connection, QObject *parent = 0);
    virtual ~VeeWebViewInterface();

    Q_PROPERTY(QString url READ url)
    QString url() const;

public slots:
    QDBusPendingReply<> reload();
    QDBusPendingReply<> setHtml(const QString &html);
    QDBusPendingReply<> stop();

signals:
    void iconChanged();
    void loadFinished(bool in0);
    void loadProgress(int progress);
    void loadStarted();
    void selectionChanged();
    void urlChanged(QString url);
};

#endif
