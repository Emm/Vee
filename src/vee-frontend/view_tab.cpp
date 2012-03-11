#include "view_tab.h"
#include <QVBoxLayout>
#include "error_view.h"

const QString ViewTab::VIM_COMMAND_PREFIX = ":";

ViewTab::ViewTab(Vim* vim, ViewResolver* viewResolver, QWidget* parent):
        QWidget(parent),
        mVim(vim),
        mViewResolver(viewResolver),
        mView(NULL),
        mInputBar(this),
        mContainer(this),
        mWidget(NULL),
        mChangeUrlAction(this),
        mNewVimCommandAction(this) {
    setLayout(new QVBoxLayout());

    mVim->setParent(this);
    mViewResolver->setParent(this);

    layout()->addWidget(& mInputBar);
    layout()->addWidget(& mContainer);
    addAction(& mChangeUrlAction);
    mContainer.addAction(& mNewVimCommandAction);

    mNewVimCommandAction.setShortcut(QKeySequence(Qt::Key_Colon));

    mViewResolver->setParent(this);
    mViewResolver->setIdentifier(mContainer.winId());

    connect(& mInputBar, SIGNAL(returnPressed()), this, SLOT(triggerVimParsing()));
    connect(& mChangeUrlAction, SIGNAL(triggered()), this, SLOT(resolveUrl()));
    connect(& mNewVimCommandAction, SIGNAL(triggered()), this, SLOT(newVimCommand()));
    connect(mViewResolver, SIGNAL(urlResolved(View*)), this, SLOT(setView(View*)));
    connect(mViewResolver, SIGNAL(unresolvableUrl(QString)), this, SLOT(setFailView(QString)));
    connect(& mContainer, SIGNAL(clientIsEmbedded()), this, SLOT(focusContainer()));
    connect(& mContainer, SIGNAL(error(QX11EmbedContainer::Error)), this, SLOT(showEmbedError(QX11EmbedContainer::Error)));

    connect(mVim, SIGNAL(openCommand(QString)), this, SLOT(setUrl(const QString &)));
    connect(mVim, SIGNAL(openInNewTabCommand(QString)), this, SIGNAL(openInNewTab(const QString &)));
    connect(mVim, SIGNAL(openInNewTabCommand(QString)), this, SLOT(blurInputBar()));
    connect(mVim, SIGNAL(closeTabCommand()), this, SIGNAL(closeTab()));
    connect(mVim, SIGNAL(prefixMissing(QString)), this, SLOT(setUrl(const QString &)));
    mInputBar.setFocus(Qt::OtherFocusReason);
}

ViewTab::~ViewTab() {
}

QIcon ViewTab::icon() const {
    return mIcon;
}

void ViewTab::setIcon(QIcon icon) {
    mIcon = icon;
    emit iconChanged(icon);
}

void ViewTab::setUrl(const QString & url) {
    mInputBar.setText(url);
    mChangeUrlAction.trigger();
}

void ViewTab::setView(View* view) {
    if (view == mView)
        return;

    discardOldView();

    bool wasDisplayUpdated = dispatchUpdateDisplay(view);
    if (wasDisplayUpdated) {
        mView = view;
        mView->setParent(this);
        updateInputBar();
        connect(mView, SIGNAL(titleChanged(const QString &)), this, SIGNAL(titleChanged(const QString &)));
        emit titleChanged(mView->title());
        setIcon(mView->icon());
        connect(mView, SIGNAL(iconChanged()), this, SLOT(viewIconWasChanged()));
        connect(mView, SIGNAL(error(View::ErrorType, int)), this, SLOT(viewGotAnError(View::ErrorType, int)));
    }
    else {
        qDebug() << "Unknown view";
    }
}

void ViewTab::discardOldView() {
    if (mView == NULL)
        return;

    mView->disconnect();
    if (mWidget != NULL) {
        layout()->removeWidget(mWidget);
        delete mWidget;
        mWidget = NULL;
        qDebug() << "Removed old widget";
    }
    delete mView;
    mView = NULL;
}

void ViewTab::updateDisplay(RemoteView* view) {
    mContainer.show();
    view->embed();
}

void ViewTab::updateDisplay(LocalView* view) {
    mWidget = view->widget();
    mWidget->setParent(this);
    mWidget->setObjectName("view");
    mContainer.hide();
    layout()->addWidget(mWidget);
    mWidget->addAction(& mNewVimCommandAction);
    mWidget->setFocus(Qt::OtherFocusReason);
}

bool ViewTab::dispatchUpdateDisplay(View* view) {
    // For want of double dispatch in C++, use dynamic casts to retrieve the
    // correct type of view
    bool viewTypeFound = true;
    LocalView* localView = qobject_cast<LocalView*>(view);
    if (localView != NULL) {
        updateDisplay(localView);
    }
    else {
        RemoteView* remoteView = qobject_cast<RemoteView*>(view);
        if (remoteView != NULL) {
            updateDisplay(remoteView);
        }
        else {
            viewTypeFound = false;
        }
    }
    return viewTypeFound;
}


void ViewTab::updateInputBar() {
    mInputBar.setText(mView->url());
    connect(mView, SIGNAL(urlChanged(const QString &)), & mInputBar, SLOT(setText(const QString &)));
}

void ViewTab::viewIconWasChanged() {
    if (!mView->icon().isNull()) {
        setIcon(mView->icon());
    }
}

View * ViewTab::view() const {
    return mView;
}

QString ViewTab::viewType() const {
    QString viewType;
    if (mView != NULL) {
        viewType = mView->interface();
    }
    return viewType;
}

void ViewTab::setFailView(QString url) {
    QString msg = QString("The URL '%1' is not valid and cannot be loaded").arg(url);
    ErrorView* errorView = new ErrorView(msg);
    setView(errorView);
}

void ViewTab::focusContainer() {
    qDebug() << "embedded";
    mContainer.setFocus(Qt::OtherFocusReason);
}

void ViewTab::showEmbedError(QX11EmbedContainer::Error error) {
    qDebug() << "error while embedding: " << error;
    QString msg = QString("Unable to embed the tab view");
    ErrorView* errorView = new ErrorView(msg);
    setView(errorView);
}

void ViewTab::resolveUrl() {
    const QString & url = mInputBar.text();
    qDebug() << "resolveUrl " << url;
    mViewResolver->resolve(url, mView);
}

void ViewTab::newVimCommand() {
    mInputBar.setFocus(Qt::ShortcutFocusReason);
    mInputBar.setText(ViewTab::VIM_COMMAND_PREFIX);
    disconnect(& mInputBar, SIGNAL(returnPressed()), & mChangeUrlAction, SLOT(trigger()));
}

void ViewTab::blurInputBar() {
    mInputBar.setText(mView->url());
    if (mWidget != NULL && mWidget->isVisible()) {
        mWidget->setFocus();
    }
    else {
        mContainer.setFocus();
    }
}

void ViewTab::triggerVimParsing() {
    mVim->parse(mInputBar.text());
}

Vim* ViewTab::vim() const {
    return mVim;
}

QWidget* ViewTab::widget() const {
    return mWidget;
}

InputBar* ViewTab::inputBar() {
    return & mInputBar;
}

void ViewTab::viewGotAnError(View::ErrorType errorType, int errorCode) {
    QString msg;
    if (errorType == View::ProcessError) {
        msg = "The view process probably crashed";
        qDebug() << "Process error: " << errorCode;
    }
    else {
        msg = "An unknown error occurred";
    }
    ErrorView* errorView = new ErrorView(msg);
    setView(errorView);
}
