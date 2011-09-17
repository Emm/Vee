#include "widget_builder.h"

#include <QDBusConnection>
#include <QX11EmbedWidget>
#include <QVBoxLayout>
#include <QTextStream>

WidgetBuilder::WidgetBuilder(const QString & urlOrFile, const ulong windowId, const DBusManager* dbusManager) : mUrlOrFile(urlOrFile), mWindowId(windowId), mDBusManager(dbusManager)  {
}

WidgetBuilder::~WidgetBuilder() {
}

QWidget* WidgetBuilder::build() {
    VeeWebView* view = buildView();
    QWidget* widget = buildWidget(*view);
    return widget;
}

VeeWebView* WidgetBuilder::buildView() {
    VeeWebView* view = new VeeWebView();
    if (shouldEmbed())
        mDBusManager->registerWidget(*view);
    if (mUrlOrFile.compare("-") == 0 || mUrlOrFile.isEmpty()) {
        const QString & html = readHtmlFromStdin();
        view->setHtml(html);
    }
    else {
        view->loadUrlOrPath(mUrlOrFile);
    }
    return view;
}

QWidget* WidgetBuilder::buildWidget(VeeWebView & view) const {
    QWidget* widget;
    if (shouldEmbed()) {
        QX11EmbedWidget* embedWidget = new QX11EmbedWidget();
        embedWidget->setLayout(new QVBoxLayout());
        embedWidget->embedInto(mWindowId);
        embedWidget->layout()->addWidget(&view);

        widget = embedWidget;
    }
    else {
        widget = & view;
    }
    return widget;
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
