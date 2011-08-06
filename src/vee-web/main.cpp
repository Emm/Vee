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

#include <QDebug>

#include <cstdlib>
#include <tclap/CmdLine.h>

#include "view.h"
#include "view_adaptor_impl.h"

#define PROJECT_NAME "vee-browser"
#define PROJECT_VERSION "0.1"

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


/**
 * Parses the command-line arguments.
 */
void
parseArgv(int argc, char** argv, ulong* windowId, std::string& urlOrFile) {
    try {
        TCLAP::CmdLine cmd(PROJECT_NAME, ' ', PROJECT_VERSION);

        TCLAP::ValueArg<ulong> winIdArg("w", "window-id", "X server window ID, for embedding", false, 0, "integer");
        cmd.add(winIdArg);

        TCLAP::UnlabeledValueArg<std::string> urlArg("url", "URL or filename", true, "", "url");
        cmd.add(urlArg);
        cmd.parse(argc, argv);
        if (winIdArg.isSet()) {
            ulong wid = winIdArg.getValue();
            *windowId = wid;
        }
        urlOrFile.assign(urlArg.getValue());
    }
    catch(TCLAP::ArgException &e) {
        fprintf(stderr, "Error %s for %s\n", e.error().c_str(), e.argId().c_str());
        std::exit(1);
    }
}

void
exposeWebViewToDBus(View* view, ulong instanceId) {
    new ViewAdaptorImpl(view);
    QDBusConnection dbus = QDBusConnection::sessionBus();
    QString serviceId = QString("org.vee.web.View_%1").arg(instanceId);
    dbus.registerObject("/VeeWebView", view);
    dbus.registerService(serviceId);
}

int
main(int argc, char *argv[]) {
    QApplication app(argc, argv);

    QString html;
    QUrl url;
    ulong* windowId;
    std::string urlOrFile;

    QStringList args = QCoreApplication::arguments();

    parseArgv(app.argc(), app.argv(), windowId, urlOrFile);

    View view;
    exposeWebViewToDBus(&view, *windowId);
    if (urlOrFile.compare("-") == 0 || urlOrFile.empty()) {
        html = makeHtml();
        view.setHtml(html);
    }
    else {
        view.loadUrlOrPath(QString(urlOrFile.c_str()));
    }

    QWidget* mainWidget;
    if (windowId == NULL) {
        mainWidget = &view;
    }
    else {

        QX11EmbedWidget* embedWidget = new QX11EmbedWidget();
        embedWidget->setLayout(new QVBoxLayout());
        embedWidget->embedInto(*windowId);
        embedWidget->layout()->addWidget(&view);

        mainWidget = embedWidget;
    }
    mainWidget->show();

    return app.exec();
}
