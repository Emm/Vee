#include <QDebug>

#include "mainwindow.h"
#include <QFileInfo>

MainWindow::MainWindow(ViewResolverFactory & viewManagerFactory) : mTabs(new EmbedTabs(viewManagerFactory)) {
    mTabs->setParent(this);

    QVBoxLayout* layout = new QVBoxLayout();

    layout->addWidget(mTabs);

    setLayout(layout);

    connect(this, SIGNAL(showUrlInActiveTab(const QString &)), mTabs, SLOT(showUrlInActiveTab(const QString &)));
    setWindowTitle("vee");
    connect(mTabs, SIGNAL(titleChanged(const QString &)), this, SLOT(setTitle(const QString &)));
}

MainWindow::~MainWindow() {
}

void MainWindow::init(const QString& url) {
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

void MainWindow::setTitle(const QString & title) {
    QString finalTitle = title + " - vee";
    setWindowTitle(finalTitle);
}
