#include <QApplication>
#include "view_resolver_factory.h"
#include "view_process_builder.h"
#include "main_window.h"

#include <QApplication>
#include <QProcess>
#include <QString>
#include <QStringList>
#include <QX11EmbedContainer>


int main(int argc, char* argv[]) {
    QApplication app(argc, argv);

    const ViewProcessBuilder processBuilder;
    ViewResolverFactory viewManagerFactory(processBuilder);
    MainWindow mainWindow(viewManagerFactory);

    mainWindow.init("reddit.com");

    mainWindow.show();

    return app.exec(); 
}
