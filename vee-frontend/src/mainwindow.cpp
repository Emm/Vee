#include <QDebug>

#include "mainwindow.h"

MainWindow::MainWindow() {
    QVBoxLayout* layout = new QVBoxLayout();

    urlBar = new QLineEdit();
    layout->addWidget(urlBar);

    mTabs = new EmbedTabs();
    layout->addWidget(mTabs);

    setLayout(layout);

    connect(urlBar, SIGNAL(returnPressed()), this, SLOT(setUrl()));
    setWindowTitle("vee");
    connect(mTabs, SIGNAL(titleChanged(const QString &)), this, SLOT(setTitle(const QString &)));
    connect(mTabs, SIGNAL(urlChanged(const QString &)), urlBar, SLOT(setText(const QString &)));
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

void MainWindow::setUrl() {
    const QString & url = urlBar->text();
    mTabs->setUrl(url);
}

void MainWindow::setTitle(const QString & title) {
    QString finalTitle = title + " - vee";
    setWindowTitle(finalTitle);
}
