#include "embedtabs.h"

EmbedTabs::EmbedTabs(QWidget* parent)
    : QTabWidget(parent) {
}

void EmbedTabs::embed(EmbedCommand& embedCommand) {
    EmbedContainer* container = new EmbedContainer(embedCommand);
    int newTabPosition = addTab(container, QString("Loading..."));
    setCurrentIndex(newTabPosition);
    container->embed();
    container->show();
}

