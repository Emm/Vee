#include "view_builder.h"

ViewBuilder::ViewBuilder(QObject* parent) :
    QObject(parent),
    mView(NULL) {
}

ViewBuilder::~ViewBuilder() {
    cleanup();
}

void ViewBuilder::build(const ulong identifier) {
    // In case the user changed the current view while the builder was attempting to build a view
    cleanup();
    mView = buildView();
    connect(mView, SIGNAL(initialized()), this, SLOT(viewInitialized()));
    connect(mView, SIGNAL(error(int, int)), this, SIGNAL(error(int, int)));
    mView->init(identifier);
}

void ViewBuilder::viewInitialized() {
    disconnect(mView);
    emit viewBuilt(mView);
    mView = NULL;
}

void ViewBuilder::viewGotAnError(int errorType, int errorCode) {
    cleanup();
    emit error(errorType, errorCode);
}

void ViewBuilder::cleanup() {
    if (mView != NULL) {
        disconnect(mView);
        delete mView;
    }
}

