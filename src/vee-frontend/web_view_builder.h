#ifndef WEB_VIEW_BUILDER_H
#define WEB_VIEW_BUILDER_H

#include "remote_view_builder.h"
#include "process_builder.h"

class WebViewBuilder : public RemoteViewBuilder {

Q_OBJECT

public:

    explicit WebViewBuilder(const ViewCommand & viewCommand,
            const ProcessBuilder & processBuilder, QObject* parent=0);
    virtual ~WebViewBuilder();

protected:
    virtual View* buildView();
};
#endif
