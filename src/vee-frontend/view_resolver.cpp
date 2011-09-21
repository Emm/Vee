#include "view_resolver.h"
#include <QDebug>

ViewResolver::ViewResolver(QVector<ViewBuilder*>* viewBuilders, QObject* parent) :
        QObject(parent),
        mViewBuilders(viewBuilders),
        mCurrentView(NULL),
        mCurrentViewBuilderPos(-1),
        mIdentifier(-1) {
}

ViewResolver::~ViewResolver() {
    delete mCurrentView;
    for (int i = 0 ; i < mViewBuilders->size() ; i++) {
        ViewBuilder* viewBuilder = mViewBuilders->at(i);
        delete viewBuilder;
    }
    delete mViewBuilders;
}


void ViewResolver::resolve(const QString & url, VeeViewInterface* currentView) {
    mUrl = url;
    askViewToResolve(currentView);
}

void ViewResolver::askViewToResolve(VeeViewInterface* view) {
    if (view != NULL) {
        qDebug() << "askViewToResolve()";
        mCurrentView = view;
        connect(mCurrentView, SIGNAL(urlResolved()), this, SLOT(viewResolvedUrl()));
        connect(mCurrentView, SIGNAL(urlNotResolved()), this, SLOT(viewDidntResolveUrl()));
        mCurrentView->resolve(mUrl);
    }
    else
        tryWithNextBuilder();
}

void ViewResolver::viewResolvedUrl() {
    qDebug() << "ViewResolver::viewResolvedUrl()";
    emit urlResolved(mCurrentView);
    cleanup();
}

void ViewResolver::viewDidntResolveUrl() {
    qDebug() << "ViewResolver::viewDidntResolveUrl()";
    disconnectAll();
    tryWithNextBuilder();
}

void ViewResolver::setIdentifier(ulong identifier) {
    mIdentifier = identifier;
}

ulong ViewResolver::identifier() const {
    return mIdentifier;
}

void ViewResolver::tryWithNextBuilder() {
    mCurrentViewBuilderPos++;
    if (mCurrentViewBuilderPos < mViewBuilders->size()) {
        ViewBuilder* viewBuilder = mViewBuilders->at(mCurrentViewBuilderPos);
        connect(viewBuilder,
                SIGNAL(viewBuilt(VeeViewInterface*)),
                this,
                SLOT(askViewToResolve(VeeViewInterface*)));
        connect(viewBuilder,
                SIGNAL(error(ViewBuilder::BuilderError)),
                this,
                SLOT(viewBuilderError(ViewBuilder::BuilderError)));
        viewBuilder->build(mIdentifier);
        qDebug() << "After viewBuilder->build(" << mIdentifier << ")";
    }
    else {
        emit unresolvableUrl(mUrl);
        cleanup();
    }
}

void ViewResolver::cleanup() {
    disconnectAll();
    mCurrentView = NULL;
    mUrl = QString();
    mCurrentViewBuilderPos = -1;
}

void ViewResolver::disconnectAll() {
    if (mCurrentView != NULL)
        disconnect(mCurrentView, 0, this, 0);
    if (mCurrentViewBuilderPos > -1 && mCurrentViewBuilderPos < mViewBuilders->size())
        disconnect(mViewBuilders->at(mCurrentViewBuilderPos), 0, this, 0);
}

void ViewResolver::viewBuilderError(ViewBuilder::BuilderError error) {
    disconnectAll();
    // FIXME implement some error reporting here
    tryWithNextBuilder();
}
