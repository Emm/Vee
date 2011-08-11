#include <QApplication>
#include "mainwindow.h"
#include "embedcommand.h"

#include <QApplication>
#include <QProcess>
#include <QString>
#include <QStringList>
#include <QX11EmbedContainer>


int main(int argc, char* argv[]) {
    QApplication app(argc, argv);

    MainWindow mainWindow;

    mainWindow.init("about:blank");

    mainWindow.show();

    return app.exec(); 
}
