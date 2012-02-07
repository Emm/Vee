#include <QDebug>

#include "main_window.h"
#include <QFileInfo>

MainWindow::MainWindow(ViewResolverFactory & viewManagerFactory) :
    QWidget(),
    mTabs(new ViewTabs(viewManagerFactory)) {
    mTabs->setParent(this);
	mTabs->setTabsClosable(true);

    QVBoxLayout* layout = new QVBoxLayout();

    layout->addWidget(mTabs);
    layout->setMargin(0);

    setLayout(layout);

    connect(this, SIGNAL(showUrlInActiveTab(const QString &)), mTabs, SLOT(showUrlInActiveTab(const QString &)));
    setWindowTitle("vee");
    connect(mTabs, SIGNAL(titleChanged(const QString &)), this, SLOT(setTitle(const QString &)));
    connect(mTabs, SIGNAL(lastTabClosed()), this, SLOT(close()));
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
