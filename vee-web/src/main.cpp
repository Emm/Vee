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
#include <QDebug>

#include <cstdlib>

#include <argtable2.h>

/**
 Builds HTML from the contents of stdin
*/
QString makeHtml() {
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
QUrl makeUrl(const QString& urlStr) {
    QUrl url;
    if (QFile::exists(urlStr))
        url = QUrl::fromLocalFile(urlStr);
    else
        url = QUrl::fromUserInput(urlStr);
    return url;
}

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    QString html;
    QUrl url;
    QStringList args = QCoreApplication::arguments();

    if (args.size() > 1) {
        if (args.at(1) == "-") {
            qDebug() << "Reading from stdin";
            html = makeHtml();
        }
        else {
            qDebug() << "Will load from a URL";
            url = makeUrl(args.at(1));
            qDebug() << url;
        }
    }
    else {
        QTextStream stderrStream(stderr);
        stderrStream << "Expected at least one parameter" << endl;
        std::exit(2);
    }
    
    QWebView view;

    if (!url.isEmpty())
        view.load(url);
    else
        view.setHtml(html);

    view.show();

    return app.exec();
}
