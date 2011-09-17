#include <QDebug>

#include "mainwindow.h"
#include <QFileInfo>

MainWindow::MainWindow(ViewResolverFactory & viewManagerFactory) : urlBar(new QLineEdit()),
    mTabs(new EmbedTabs(viewManagerFactory)) {
    urlBar->setParent(this);
    mTabs->setParent(this);

    QVBoxLayout* layout = new QVBoxLayout();

    layout->addWidget(urlBar);
    layout->addWidget(mTabs);

    setLayout(layout);

    connect(urlBar, SIGNAL(returnPressed()), this, SLOT(urlChanged()));
    connect(this, SIGNAL(showUrlInActiveTab(const QString &)), mTabs, SLOT(showUrlInActiveTab(const QString &)));
    setWindowTitle("vee");
    connect(mTabs, SIGNAL(titleChanged(const QString &)), this, SLOT(setTitle(const QString &)));
    connect(mTabs, SIGNAL(urlChanged(const QString &)), urlBar, SLOT(setText(const QString &)));
}

MainWindow::~MainWindow() {
}

void MainWindow::init(const QString& url) {
    urlBar->setText(url);
    emit showUrlInActiveTab(url);
}

void MainWindow::urlChanged() {
    const QString & url = urlBar->text();
    emit showUrlInActiveTab(url);
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
/*
void MainWindow::urlChanged() {
    const QString & url = urlBar->text();
    QString executable("src/vee-web/vee-web");
    assertExecutable(executable);
    EmbedCommand cmd = EmbedCommand(executable);

    cmd << QString("-w");
    cmd << EmbedCommand::WIN_ID;
    cmd << QString(url);
    mTabs->embed(cmd);
}

void MainWindow::showUrlInActiveTab() {
    emit(showUrlInActiveTab(
    const QString & url = urlBar->text();
    mTabs->showUrl(url);
}
*/
void MainWindow::setTitle(const QString & title) {
    QString finalTitle = title + " - vee";
    setWindowTitle(finalTitle);
}
