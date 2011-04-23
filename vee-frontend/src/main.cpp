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

    QString executable("../../vee-web/src/src/vee-web");
    EmbedCommand cmd = EmbedCommand(executable);

    cmd << QString("-w");
    cmd << EmbedCommand::WIN_ID;
    cmd << QString("reddit.com");


    mainWindow.show();
    mainWindow.embed(cmd);

    return app.exec(); 
}
