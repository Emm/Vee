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

QWidget* initApp(int argc, char* argv[]) {
    CommandLineParser parser(APP_NAME, APP_VERSION);
    int success = parser.parse(argc, argv);

    if (success == COMMANDLINE_PARSING_ERROR) {
        fprintf(stderr, parser.errorMessage().toUtf8().data());
        exit(1);
    }
    QString urlOrFile = parser.urlOrFile();
    ulong windowId = parser.windowId();
    QWidget* mainWidget;
    if (windowId != NULL_WINDOW_ID) {
        QString serviceId = QString::fromUtf8(SERVICE_ID_TEMPLATE).arg(windowId);
        QString objectPath = QString::fromUtf8(OBJECT_PATH);
        DBusManager dbusManager(serviceId, objectPath);

        WidgetBuilder embeddedWidgetBuilder(urlOrFile, windowId, & dbusManager);
        mainWidget = embeddedWidgetBuilder.build();
    }
    else {
        WidgetBuilder standaloneWidgetBuilder(urlOrFile);
        mainWidget = standaloneWidgetBuilder.build();
    }

    return mainWidget;
}

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);

    QWidget* mainWidget = initApp(app.argc(), app.argv());
    mainWidget->show();

    return app.exec();
}
