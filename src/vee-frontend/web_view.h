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
    virtual void updateIcon();
};

#endif
