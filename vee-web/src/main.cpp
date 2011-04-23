/**
 * Vik is a browser based on the integration of several different applications,
 * each running in their own process.
 *
 * Vik-browser is the browser part of Vik.
 */

#include <QApplication>
#include <QWebView>
#include <QTextStream>
#include <QStringList>
#include <QFile>
#include <QX11EmbedWidget>

#include <QDebug>

#include <cstdlib>
#include <tclap/CmdLine.h>

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
 * Builds a QUrl object from the positional parameter. Attempts to find a local
 * file first, if there is no such file, assumes the parameter is a URL.
 */
QUrl
makeUrl(const QString& urlStr) {
    QUrl url;
    if (QFile::exists(urlStr))
        url = QUrl::fromLocalFile(urlStr);
    else
        url = QUrl::fromUserInput(urlStr);
    return url;
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

int
main(int argc, char *argv[]) {
    QApplication app(argc, argv);

    QString html;
    QUrl url;
    ulong* windowId;
    std::string urlOrFile;

    QStringList args = QCoreApplication::arguments();

    parseArgv(app.argc(), app.argv(), windowId, urlOrFile);

    if (urlOrFile.compare("-") == 0 || urlOrFile.empty()) {
        html = makeHtml();
    }
    else {
        url = makeUrl(QString(urlOrFile.c_str()));
    }
    QWebView view;

    if (!url.isEmpty())
        view.load(url);
    else
        view.setHtml(html);

    QWidget* mainWidget;
    if (windowId == NULL) {
        qDebug() << "Going to be displayed standalone";
        mainWidget = &view;
    }
    else {
        QX11EmbedWidget* embedWidget = new QX11EmbedWidget();
        qDebug() << "Going to embed into " << windowId;
        embedWidget->embedInto(*windowId);
        view.setParent(embedWidget);

        mainWidget = embedWidget;
    }
    mainWidget->show();

    return app.exec();
}
