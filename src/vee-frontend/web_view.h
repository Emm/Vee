#ifndef WEB_VIEW_H
#define WEB_VIEW_H

#include "remote_view.h"

class WebView : public RemoteView {
    Q_OBJECT

public:
    explicit WebView(QProcess* process, const QString &service, const QString &path, const QString & interfaceName, const QDBusConnection &connection, QObject *parent = 0);
    virtual ~WebView();

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
