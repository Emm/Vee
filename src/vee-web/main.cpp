/**
 * Vee is a browser based on the integration of several different applications,
 * each running in their own process.
 *
 * Vee-web is the browser part of Vee.
 */

#include <QApplication>

#include "constants.h"
#include "command_line_parser.h"
#include "dbus_manager.h"
#include "vee_web_service_builder.h"

void initApp(int argc, char* argv[]) {
    CommandLineParser parser(APP_NAME, APP_VERSION);
    int success = parser.parse(argc, argv);

    if (success == COMMANDLINE_PARSING_ERROR) {
        fprintf(stderr, parser.errorMessage().toUtf8().data());
        exit(1);
    }
    QString urlOrFile = parser.urlOrFile();
    ulong windowId = parser.windowId();
    VeeWebService* service;
    if (windowId != NULL_WINDOW_ID) {
        QString serviceId = QString::fromUtf8(SERVICE_ID_TEMPLATE).arg(windowId);
        QString objectPath = QString::fromUtf8(OBJECT_PATH);
        DBusManager dbusManager(serviceId, objectPath);

        VeeWebServiceBuilder embeddedVeeWebServiceBuilder(urlOrFile, windowId, & dbusManager);
        service = embeddedVeeWebServiceBuilder.build();
    }
    else {
        VeeWebServiceBuilder standaloneVeeWebServiceBuilder(urlOrFile);
        service = standaloneVeeWebServiceBuilder.build();
        service->show();
    }
}

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);

    initApp(app.argc(), app.argv());

    return app.exec();
}
