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


void ViewResolver::resolve(const QString & url, View* currentView) {
    mUrl = url;
    mTabView = currentView;
    tryWithNextBuilder();
}

void ViewResolver::askViewToResolve(View* view) {
    if (view != NULL) {
        qDebug() << "askViewToResolve()";
        mCurrentView = view;
        connect(mCurrentView, SIGNAL(urlResolved()), this, SLOT(viewResolvedUrl()));
        connect(mCurrentView, SIGNAL(urlNotResolved()), this, SLOT(viewDidntResolveUrl()));
        qDebug() << "askViewToResolve(): connected";
        mCurrentView->resolve(mUrl);
    }
    else
        tryWithNextBuilder();
}

void ViewResolver::viewResolvedUrl() {
    qDebug() << "ViewResolver::viewResolvedUrl()";
    ViewBuilder* builder = currentBuilder();
    QString viewType = builder != NULL ? builder->viewType() : QString();
    emit urlResolved(mCurrentView, viewType);
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
    ViewBuilder* builder = currentBuilder();
    if (builder != NULL) {
        if (mTabView && (builder->viewType() == mTabView->interface())) {
            qDebug() << "Using existing view";
            askViewToResolve(mTabView);
        }
        else {
            connect(builder,
                    SIGNAL(viewBuilt(View*)),
                    this,
                    SLOT(askViewToResolve(View*)));
            connect(builder,
                    SIGNAL(error(int, int)),
                    this,
                    SLOT(viewBuilderError(int, int)));
            builder->build(mIdentifier);
            qDebug() << "After builder->build(" << mIdentifier << ")";
        }
    }
    else {
        emit unresolvableUrl(mUrl);
        cleanup();
    }
}

void ViewResolver::cleanup() {
    disconnectAll();
    mCurrentView = NULL;
    mTabView = NULL;
    mUrl = QString();
    mCurrentViewBuilderPos = -1;
}

void ViewResolver::disconnectAll() {
    if (mCurrentView != NULL) {
        disconnect(mCurrentView, SIGNAL(urlResolved()), this, SLOT(viewResolvedUrl()));
        disconnect(mCurrentView, SIGNAL(urlNotResolved()), this, SLOT(viewDidntResolveUrl()));
    }
    ViewBuilder* builder = currentBuilder();
    if (builder != NULL)
        disconnect(builder, 0, this, 0);
}

void ViewResolver::viewBuilderError(int errorType, int errorCode) {
    disconnectAll();
    // FIXME implement some error reporting here
    tryWithNextBuilder();
}

ViewBuilder* ViewResolver::currentBuilder() {
    ViewBuilder* builder;
    if (mCurrentViewBuilderPos > -1 && mCurrentViewBuilderPos < mViewBuilders->size())
        builder = mViewBuilders->at(mCurrentViewBuilderPos);
    else
        builder = NULL;
    return builder;
}
