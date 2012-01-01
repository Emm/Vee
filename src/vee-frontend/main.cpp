#include <QApplication>

#include "common_constants.h"
#include "constants.h"
#include "view_resolver_factory.h"
#include "view_process_builder.h"
#include "command_line_parser.h"
#include "main_window.h"

int main(int argc, char* argv[]) {
    QApplication app(argc, argv);

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
