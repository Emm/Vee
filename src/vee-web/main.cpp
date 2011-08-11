/**
 * Vee is a browser based on the integration of several different applications,
 * each running in their own process.
 *
 * Vee-web is the browser part of Vee.
 */

#include <QApplication>

#include "constants.h"
#include "commandline.h"
#include "dbus_manager.h"
#include "widget_builder.h"

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);

    CommandLineParser parser(APP_NAME, APP_VERSION);
    int success = parser.parse(app.argc(), app.argv());

    if (success == COMMANDLINE_PARSING_ERROR) {
        fprintf(stderr, parser.errorMessage().toUtf8().data());
        exit(1);
    }
    const QString & urlOrFile = parser.urlOrFile();
    const ulong windowId = parser.windowId();

    DBusManager dbusManager(SERVICE_ID_TEMPLATE, OBJECT_PATH, windowId);

    WidgetBuilder widgetBuilder;

    widgetBuilder.setUrlOrFile(urlOrFile);
    widgetBuilder.setWindowId(windowId);
    widgetBuilder.setDBusManager(&dbusManager);

    QWidget* mainWidget = widgetBuilder.build();

    mainWidget->show();

    return app.exec();
}
