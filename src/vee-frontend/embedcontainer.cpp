#include "embedcontainer.h"
#include <QVBoxLayout>
#include "vee_web_view_interface.h"

/*EmbedContainer::EmbedContainer(EmbedCommand& embedCommand, QWidget* parent) : QWidget(parent) {
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
}*/

EmbedContainer::EmbedContainer(ViewResolver* viewResolver, QWidget* parent):
        QWidget(parent),
        mViewResolver(viewResolver),
        mView(NULL),
        mContainer(new QX11EmbedContainer(this))  {
    setLayout(new QVBoxLayout());
    layout()->addWidget(mContainer);
    mViewResolver->setParent(this);

    mViewResolver->setIdentifier(mContainer->winId());
    connect(mViewResolver, SIGNAL(urlResolved(VeeViewInterface*, QString)), this, SLOT(setView(VeeViewInterface*, QString)));
    connect(mViewResolver, SIGNAL(unresolvableUrl(QString &)), this, SLOT(setFailView(QString &)));
    connect(mContainer, SIGNAL(clientIsEmbedded()), this, SLOT(focusContainer()));
    connect(mContainer, SIGNAL(error(QX11EmbedContainer::Error)), this, SLOT(showEmbedError(QX11EmbedContainer::Error)));
}

EmbedContainer::~EmbedContainer() {
    delete mViewResolver;
}
/*void EmbedContainer::embed() {
    //mProcess->setStandardOutputFile(QString("/tmp/tab.1.log"));
    //mProcess->setStandardErrorFile(QString("/tmp/tab.2.log"));
    qDebug() << "Starting: " << *mExecutable << " " << *mArguments;
    mProcess->start(*mExecutable, *mArguments);
    ulong instanceId = mContainer->winId();
    QString serviceId = QString("org.vee.web.VeeWebView_%1").arg(instanceId);
    qDebug() << serviceId;
    mInterface = new OrgVeeWebVeeWebViewInterface(serviceId, "/VeeWebView", QDBusConnection::sessionBus(), this); 
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
}*/

void EmbedContainer::setUrl(const QString & url) {
    mViewResolver->resolve(url, mView);
}

void EmbedContainer::setView(VeeViewInterface* view, QString viewType) {
    if (view != mView) {
        if (mView) {
            disconnect();
            mContainer->discardClient();
            delete mView;
        }
        mView = view;
        mViewType = viewType;
        mView->setParent(mContainer);
        mView->embed();
        connect(mView, SIGNAL(titleChanged(const QString &)), this, SIGNAL(titleChanged(const QString &)));
        emit titleChanged(mView->title());
        if (mViewType == "org.vee.VeeWebView") {
            VeeWebViewInterface* webViewInt = qobject_cast<VeeWebViewInterface *>(mView);
            if (webViewInt) {
                connect(webViewInt, SIGNAL(urlChanged(const QString &)), this,
                        SIGNAL(urlChanged(const QString &)));
                emit urlChanged(webViewInt->url());
            }
        }
        //mContainer->setFocus(Qt::OtherFocusReason);
    }
}

void EmbedContainer::setFailView(QString & url) {
    // FIXME display an error message
}

void EmbedContainer::focusContainer() {
    qDebug() << "embedded";
    mContainer->setFocus(Qt::OtherFocusReason);
}

void EmbedContainer::showEmbedError(QX11EmbedContainer::Error error) {
    qDebug() << "error while embedding: " << error;
    // FIXME actually handle error
}

void EmbedContainer::disconnectView() {
    if (mView != NULL) {
        disconnect(mView, 0, this, 0);
    }
}
