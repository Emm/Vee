#include "embedcontainer.h"
#include <QVBoxLayout>
#include "vee_view_remote_interface.h"
#include "vee_local_view.h"

EmbedContainer::EmbedContainer(ViewResolver* viewResolver, QWidget* parent):
        QWidget(parent),
        mViewResolver(viewResolver),
        mView(NULL),
        mInputBar(new QLineEdit(this)),
        mContainer(new QX11EmbedContainer(this)),
        mWidget(NULL),
        mChangeUrlAction(new QAction(this)) {
    setLayout(new QVBoxLayout());

    layout()->addWidget(mInputBar);
    layout()->addWidget(mContainer);
    addAction(mChangeUrlAction);

    mViewResolver->setParent(this);
    mViewResolver->setIdentifier(mContainer->winId());

    connect(mInputBar, SIGNAL(returnPressed()), mChangeUrlAction, SLOT(trigger()));
    connect(mChangeUrlAction, SIGNAL(triggered()), this, SLOT(resolveUrl()));
    connect(mViewResolver, SIGNAL(urlResolved(VeeViewInterface*, QString)), this, SLOT(setView(VeeViewInterface*, QString)));
    connect(mViewResolver, SIGNAL(unresolvableUrl(QString &)), this, SLOT(setFailView(QString &)));
    connect(mContainer, SIGNAL(clientIsEmbedded()), this, SLOT(focusContainer()));
    connect(mContainer, SIGNAL(error(QX11EmbedContainer::Error)), this, SLOT(showEmbedError(QX11EmbedContainer::Error)));
}

EmbedContainer::~EmbedContainer() {
    delete mViewResolver;
}

void EmbedContainer::setUrl(const QString & url) {
    mInputBar->setText(url);
    mChangeUrlAction->trigger();
}

void EmbedContainer::setView(VeeViewInterface* view, QString viewType) {
    if (view != mView) {
        if (mView) {
            disconnectView();
            if (mWidget != NULL) {
                layout()->removeWidget(mWidget);
                delete mWidget;
                qDebug() << "Removed old widget";
            }
            delete mView;
        }
        mView = view;
        mViewType = viewType;
        mInputBar->setText(view->url());
        qDebug() << "New view type: " << mView->interface();
        VeeViewRemoteInterface* remoteInt = qobject_cast<VeeViewRemoteInterface *>(mView);
        if (remoteInt) {
            mView->setParent(mContainer);
            mContainer->show();
            remoteInt->embed();
        }
        else {
            VeeLocalView* localView = qobject_cast<VeeLocalView *>(mView);
            localView->setParent(this);
            mWidget = localView->widget();
            mWidget->setParent(this);
            mContainer->hide();
            layout()->addWidget(mWidget);
        }
        connect(mView, SIGNAL(titleChanged(const QString &)), this, SIGNAL(titleChanged(const QString &)));
        connect(mView, SIGNAL(urlChanged(const QString &)), mInputBar, SLOT(setText(const QString &)));
        emit titleChanged(mView->title());
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
        disconnect(mView, 0, mInputBar, 0);
    }
}

void EmbedContainer::resolveUrl() {
    qDebug() << "resolveUrl";
    const QString & url = mInputBar->text();
    mViewResolver->resolve(url, mView);
}
