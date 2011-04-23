#include "embedtabs.h"

EmbedTabs::EmbedTabs(QWidget* parent)
    : QTabWidget(parent) {
}

void EmbedTabs::embed(EmbedCommand& embedCommand) {
/*    QX11EmbedContainer container;
    connect(&container, SIGNAL(clientIsEmbedded()), this, &());

    QProcess process(&container);
    process.start(executable, arguments);
    
    //QObject::connect(&container, SIGNAL(clientClosed()), &app,
    //SLOT(quit()));
    addTab(&container, QString("Loading..."));*/
    EmbedContainer* container = new EmbedContainer(embedCommand);
    //addTab(container, QString("Loading"));
    addTab(container, QString("Loading"));
    qDebug() << "Added tab";

    container->embed();
    container->show();
}
