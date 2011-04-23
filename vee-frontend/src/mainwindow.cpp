#include <QDebug>

#include "mainwindow.h"
#include "embedcommand.h"

MainWindow::MainWindow() {
    mTabs = new EmbedTabs();
    QVBoxLayout* layout = new QVBoxLayout();
    layout->addWidget(mTabs);

    setLayout(layout);
}

void MainWindow::embed(EmbedCommand& embedCommand) {
    mTabs->embed(embedCommand);
}
