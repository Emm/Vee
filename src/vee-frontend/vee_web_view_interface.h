#ifndef VEE_WEB_VIEW_INTERFACE_H
#define VEE_WEB_VIEW_INTERFACE_H

#include "remote_view.h"

class VeeWebViewInterface : public RemoteView {
    Q_OBJECT

public:
    explicit VeeWebViewInterface(QProcess* process, const QString &service, const QString &path, const QString & interfaceName, const QDBusConnection &connection, QObject *parent = 0);
    virtual ~VeeWebViewInterface();

public slots:
    void reload();
    void setHtml(const QString &html);
    void stop();

signals:
    void iconChanged();
    void loadFinished(bool in0);
    void loadProgress(int progress);
    void loadStarted();
    void selectionChanged();
};

#endif
