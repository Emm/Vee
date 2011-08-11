#include <QDebug>

#include "mainwindow.h"
#include <QFileInfo>

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

void assertExecutable(const QString & executable) {
    QFileInfo fileInfo(executable);
    if (!fileInfo.isFile()) {
        fprintf(stderr, "The binary %s doesn't exist or is not a file", executable.toUtf8().data());
        exit(1);
    }
    if (!fileInfo.isExecutable()) {
        fprintf(stderr, "The binary %s is not executable", executable.toUtf8().data());
        exit(1);
    }
}

void MainWindow::addTab() {
    const QString & url = urlBar->text();
    QString executable("src/vee-web/vee-web");
    assertExecutable(executable);
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
