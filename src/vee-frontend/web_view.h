#ifndef WEB_VIEW_H
#define WEB_VIEW_H

#include "remote_view.h"

class WebView : public RemoteView {
    Q_OBJECT

public:
    explicit WebView(const ViewCommand & viewCommmand, Process* process, QObject* parent=0);
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
