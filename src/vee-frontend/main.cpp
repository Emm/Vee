#include <QApplication>
#include "view_resolver_factory.h"
#include "mainwindow.h"

#include <QApplication>
#include <QProcess>
#include <QString>
#include <QStringList>
#include <QX11EmbedContainer>


int main(int argc, char* argv[]) {
    QApplication app(argc, argv);

    ViewResolverFactory viewManagerFactory;
    MainWindow mainWindow(viewManagerFactory);

    mainWindow.init("reddit.com");

    mainWindow.show();

    return app.exec(); 
}
