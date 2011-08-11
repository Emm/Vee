/**
 * Vee is a browser based on the integration of several different applications,
 * each running in their own process.
 *
 * Vee-web is the browser part of Vee.
 */

#include <QApplication>
#include <QTextStream>
#include <QStringList>
#include <QFile>
#include <QX11EmbedWidget>
#include <QVBoxLayout>
#include <QDBusConnection>

#include <cstdlib>

#include "constants.h"
#include "commandline.h"
#include "vee_web_view.h"
#include "vee_web_view_adaptor_impl.h"

/**
 Builds HTML from the contents of stdin
*/
QString
makeHtml() {
    QString html;
    QTextStream stdinStream(stdin);
    QString line;
    do {
        line = stdinStream.readLine();
        html.append(line);
    }
    while (!line.isNull());
    return html;
}


void
exposeVeeWebViewToDBus(VeeWebView* view, ulong instanceId) {
    new VeeWebViewAdaptorImpl(view);
    QDBusConnection dbus = QDBusConnection::sessionBus();
    QString serviceId = QString("org.vee.web.VeeWebView_%1").arg(instanceId);
    dbus.registerObject("/VeeWebView", view);
    dbus.registerService(serviceId);
}

int
main(int argc, char *argv[]) {
    QApplication app(argc, argv);
    QString html;
    QUrl url;

    QStringList args = QCoreApplication::arguments();

    CommandLineParser parser(APP_NAME, APP_VERSION);
    int success = parser.parse(app.argc(), app.argv());

    if (success == COMMANDLINE_PARSING_ERROR) {
        fprintf(stderr, parser.errorMessage().toUtf8().data());
        exit(1);
    }
    const QString & urlOrFile = parser.urlOrFile();
    ulong windowId = parser.windowId();
    VeeWebView view;
    if (urlOrFile.compare("-") == 0 || urlOrFile.isEmpty()) {
        html = makeHtml();
        view.setHtml(html);
    }
    else {
        exposeVeeWebViewToDBus(&view, windowId);
        view.loadUrlOrPath(urlOrFile);
    }

    QWidget* mainWidget;
    if (windowId == 0) {
        mainWidget = &view;
    }
    else {

        QX11EmbedWidget* embedWidget = new QX11EmbedWidget();
        embedWidget->setLayout(new QVBoxLayout());
        embedWidget->embedInto(windowId);
        embedWidget->layout()->addWidget(&view);

        mainWidget = embedWidget;
    }
    mainWidget->show();

    return app.exec();
}
