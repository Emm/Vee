#include "embedcontainer.h"

EmbedContainer::EmbedContainer(EmbedCommand& embedContainer, QWidget* parent) : QWidget(parent) {
    setLayout(new QVBoxLayout());
    mContainer = new QX11EmbedContainer();
    mContainer->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    mLabel = new QLabel(QString("Loading"));
    layout()->addWidget(mLabel);
    layout()->addWidget(mContainer);

    mExecutable = &(embedContainer.executable());
    mArguments = embedContainer.arguments(mContainer->winId());
    qDebug() << "WinID: " << mContainer->winId();
    mProcess = new QProcess(mContainer);

    connect(mContainer, SIGNAL(clientIsEmbedded()), this, SLOT(clientIsEmbedded()));
    connect(mContainer, SIGNAL(error(QX11EmbedContainer::Error)), this, SLOT(error(QX11EmbedContainer::Error)));
    connect(mContainer, SIGNAL(clientClosed()), this, SLOT(clientClosed()));
}

void EmbedContainer::embed() {
    mProcess->setStandardOutputFile(QString("/tmp/tab.1.log"));
    mProcess->setStandardErrorFile(QString("/tmp/tab.2.log"));
    qDebug() << "Starting: " << *mExecutable << " " << *mArguments;
    mProcess->start(*mExecutable, *mArguments);
}

void EmbedContainer::clientClosed() {
    qDebug() << "Client closed";
}

void EmbedContainer::clientIsEmbedded() {
    qDebug() << "Embedded";
    layout()->removeWidget(mLabel);
}

void EmbedContainer::error(QX11EmbedContainer::Error error) {
    qDebug() << "Error";
    mLabel->setText("Error while embedding, got code: " + error);
}
