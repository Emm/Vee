#include "web_view_builder.h"
#include "web_view.h"

WebViewBuilder::WebViewBuilder(const ViewCommand & viewCommand,
        const ProcessBuilder & processBuilder, QObject* parent) :
    RemoteViewBuilder(viewCommand, processBuilder, parent) {
}

WebViewBuilder::~WebViewBuilder() {
}

View* WebViewBuilder::buildView() {
    Process* process = mProcessBuilder.build();
    WebView* view = new WebView(mViewCommand, process, this);
    return view;
}
