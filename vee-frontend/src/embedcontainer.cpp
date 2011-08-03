#include "embedcontainer.h"

EmbedContainer::EmbedContainer(EmbedCommand& embedCommand, QWidget* parent) : QWidget(parent) {
    QVBoxLayout* layout = new QVBoxLayout();
    mContainer = new QX11EmbedContainer();
    //mContainer->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    //mLabel = new QLabel(QString("Loading"));
    //layout->addWidget(mLabel);
    layout->addWidget(mContainer);
    setLayout(layout);
    setStyleSheet("background-color: #FFF");
    setStyleSheet("padding: 0; margin: 0");

    mExecutable = &(embedCommand).executable();
    mArguments = embedCommand.arguments(mContainer->winId());
    qDebug() << "WinID: " << mContainer->winId();
    mProcess = new QProcess(mContainer);

    connect(mContainer, SIGNAL(clientIsEmbedded()), this, SLOT(clientIsEmbedded()));
    connect(mContainer, SIGNAL(error(QX11EmbedContainer::Error)), this, SLOT(error(QX11EmbedContainer::Error)));
    connect(mContainer, SIGNAL(clientClosed()), this, SIGNAL(clientClosed()));
}

void EmbedContainer::embed() {
    //mProcess->setStandardOutputFile(QString("/tmp/tab.1.log"));
    //mProcess->setStandardErrorFile(QString("/tmp/tab.2.log"));
    qDebug() << "Starting: " << *mExecutable << " " << *mArguments;
    mProcess->start(*mExecutable, *mArguments);
    ulong instanceId = mContainer->winId();
    QString serviceId = QString("com.trolltech.Qt.QWebView_%1").arg(instanceId);
    qDebug() << serviceId;
    mInterface = new ComTrolltechQtQWebViewInterface(serviceId, "/VeeWebView", QDBusConnection::sessionBus(), this); 
    connect(mInterface, SIGNAL(urlChanged(const QString &)), this, SIGNAL(urlChanged(const QString &)));
    connect(mInterface, SIGNAL(titleChanged(const QString &)), this, SIGNAL(titleChanged(const QString &)));
    qDebug() << "Connected";
}

void EmbedContainer::clientIsEmbedded() {
    mContainer->setFocus(Qt::OtherFocusReason);
    //layout()->removeWidget(mLabel);
}

void EmbedContainer::error(QX11EmbedContainer::Error error) {
    qDebug() << "Error";
    mLabel->setText("Error while embedding, got code: " + error);
}

void EmbedContainer::setUrl(const QString & url) {
    if (!mInterface)
        return;
    //mInterface->setHtml("<html><body><h1>mooh</body></html>");
    mInterface->setUrl(url);
}
