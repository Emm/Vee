#include <QDebug>

#include "mainwindow.h"

MainWindow::MainWindow() {
    QVBoxLayout* layout = new QVBoxLayout();

    urlBar = new QLineEdit();
    layout->addWidget(urlBar);

    mTabs = new EmbedTabs();
    layout->addWidget(mTabs);

    setLayout(layout);

    connect(urlBar, SIGNAL(returnPressed()), this, SLOT(addTab()));
}

void MainWindow::init(const QString& url) {
    urlBar->setText(url);
    addTab();
}

void MainWindow::addTab() {
    const QString & url = urlBar->text();
    QString executable("../../vee-web/src/src/vee-web");
    EmbedCommand cmd = EmbedCommand(executable);

    cmd << QString("-w");
    cmd << EmbedCommand::WIN_ID;
    cmd << QString(url);
    mTabs->embed(cmd);
}
