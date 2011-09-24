#include "widget_builder.h"

#include <QTextStream>

WidgetBuilder::WidgetBuilder(const QString & urlOrFile, const ulong windowId, const DBusManager* dbusManager) : mUrlOrFile(urlOrFile), mWindowId(windowId), mDBusManager(dbusManager)  {
}

WidgetBuilder::~WidgetBuilder() {
}

QWidget* WidgetBuilder::build() {
    VeeWebView* view = buildView();
    return view;
}

VeeWebView* WidgetBuilder::buildView() {
    VeeWebView* view = new VeeWebView(mWindowId);
    if (shouldEmbed())
        mDBusManager->registerWidget(*view);
    if (mUrlOrFile.compare("-") == 0) {
        const QString & html = readHtmlFromStdin();
        view->setHtml(html);
    }
    else if (!mUrlOrFile.isEmpty()) {
        view->resolve(mUrlOrFile);
    }
    return view;
}

/**
 Builds HTML from the contents of stdin
*/
const QString & WidgetBuilder::readHtmlFromStdin() const {
    QString* html = new QString();
    QTextStream stdinStream(stdin);
    QString line;
    do {
        line = stdinStream.readLine();
        html->append(line);
    }
    while (!line.isNull());
    return *html;
}

bool WidgetBuilder::shouldEmbed() const {
    return mWindowId != 0 && mDBusManager != NULL;
}
