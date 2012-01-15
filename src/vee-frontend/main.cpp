#include <QApplication>

#include <QFile>
#include <QString>
#include <QTextStream>
#include <QDebug>
#include "common_constants.h"
#include "constants.h"
#include "view_resolver_factory.h"
#include "view_process_builder.h"
#include "command_line_parser.h"
#include "main_window.h"

void loadStyleSheet() {
    /* Let's use QFile and point to a resource... */
    QFile data("://stylesheets/vee.css");
    QString style;
    /* ...to open the file */
    if(data.open(QFile::ReadOnly)) {
        /* QTextStream... */
        QTextStream styleIn(&data);
        /* ...read file to a string. */
        style = styleIn.readAll();
        data.close();
        /* We'll use qApp macro to get the QApplication pointer
         * and set the style sheet application wide. */
        qApp->setStyleSheet(style);
    }
}

int main(int argc, char* argv[]) {
    QApplication app(argc, argv);
    loadStyleSheet();
    const ViewProcessBuilder processBuilder;
    ViewResolverFactory viewManagerFactory(processBuilder);

    CommandLineParser parser(APP_NAME, APP_VERSION);
    int success = parser.parse(argc, argv);

    if (success == COMMANDLINE_PARSING_ERROR) {
        fprintf(stderr, parser.errorMessage().toUtf8().data());
        exit(1);
    }
    QString urlOrFile = parser.urlOrFile();
    MainWindow mainWindow(viewManagerFactory);
    mainWindow.init(urlOrFile);
    mainWindow.show();

    return app.exec(); 
}
