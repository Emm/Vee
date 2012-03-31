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
        mNewVimCommandAction(this),
        mBackwardAction(this),
        mForwardAction(this) {
    setLayout(new QVBoxLayout());

    mVim->setParent(this);
    mViewResolver->setParent(this);

    layout()->addWidget(& mInputBar);
    layout()->addWidget(& mContainer);
    addActions(*this);
    addAction(& mChangeUrlAction);
    addActions(mContainer);
    mBackwardAction.setEnabled(false);
    mForwardAction.setEnabled(false);

    mNewVimCommandAction.setShortcut(QKeySequence(Qt::Key_Colon));
    mBackwardAction.setShortcut(QKeySequence(Qt::CTRL + Qt::Key_O));
    mForwardAction.setShortcut(QKeySequence(Qt::CTRL + Qt::Key_I));

    mViewResolver->setParent(this);
    mViewResolver->setIdentifier(mContainer.winId());

    connect(& mInputBar, SIGNAL(returnPressed()), this, SLOT(triggerVimParsing()));
    connect(& mInputBar, SIGNAL(escapePressed()), this, SLOT(focusView()));

    connect(& mChangeUrlAction, SIGNAL(triggered()), this, SLOT(resolveUrl()));
    connect(& mNewVimCommandAction, SIGNAL(triggered()), this, SLOT(newVimCommand()));
    connect(& mBackwardAction, SIGNAL(triggered()), this, SLOT(backward()));
    connect(& mForwardAction, SIGNAL(triggered()), this, SLOT(forward()));


    connect(mViewResolver, SIGNAL(urlResolved(View*)), this, SLOT(setView(View*)));
    connect(mViewResolver, SIGNAL(unresolvableUrl(QString)), this, SLOT(setFailView(QString)));
    connect(& mContainer, SIGNAL(clientIsEmbedded()), this, SLOT(focusContainer()));
    connect(& mContainer, SIGNAL(error(QX11EmbedContainer::Error)), this, SLOT(showEmbedError(QX11EmbedContainer::Error)));

    connect(mVim, SIGNAL(openCommand(QString)), this, SLOT(newUrl(const QString &)));
    connect(mVim, SIGNAL(openInNewTabCommand(QString)), this, SIGNAL(openInNewTab(const QString &)));
    connect(mVim, SIGNAL(openInNewTabCommand(QString)), this, SLOT(blurInputBar()));
    connect(mVim, SIGNAL(closeTabCommand()), this, SIGNAL(closeTab()));
    connect(mVim, SIGNAL(prefixMissing(QString)), this, SLOT(newUrl(const QString &)));
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

void ViewTab::setTitle(const QString & title) {
    mTitle = title;
    emit titleChanged(mView->title());
}

void ViewTab::newUrl(const QString & url) {
    addSkeletonEntryToHistory(url);
    setUrl(url);
}

void ViewTab::setUrl(const QString & url) {
    mInputBar.setText(url);
    mChangeUrlAction.trigger();
}

void ViewTab::addSkeletonEntryToHistory(const QString & url) {
    qDebug() << "Add skeleton history entry with url " << url;
    // It's a 'skeleton' entry because we have neither title nor icon at this
    // stage, we don't even have a proper URL
    HistoryEntry historyEntry(url, QString(), QIcon());
    mHistory.setCurrentEntry(historyEntry);
    mBackwardAction.setEnabled(true);
    mForwardAction.setEnabled(false);
}

void ViewTab::completeHistoryEntry() {
    qDebug() << "Complete history entry - url " << mUrl;
    HistoryEntry historyEntry(mUrl, mTitle, mIcon);
    mHistory.updateCurrentEntry(historyEntry);
}

void ViewTab::setView(View* view) {
    bool sameAsOldView = (view == mView);
    if (!sameAsOldView) {
        discardOldView();
    }
    mView = view;
    mUrl = view->url();
    setTitle(mView->title());

    if (mView != NULL) {
        completeHistoryEntry();
    }
    else {
        if (mView == NULL)
            qDebug() << "Didn't add to history, NULL view";
        else
            qDebug() << "Didn't add to history, identical URLs: " << mUrl << "==" << view->url();
    }
    toggleBackwardAndForwardActionsIfNeeded();

    if (sameAsOldView) {
        focusView();
        return;
    }
    bool wasDisplayUpdated = dispatchUpdateDisplay(view);

    if (wasDisplayUpdated) {
        mView->setParent(this);
        updateInputBar();
        connect(mView, SIGNAL(titleChanged(const QString &)), this, SIGNAL(titleChanged(const QString &)));
        setIcon(mView->icon());
        setTitle(mView->title());
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
    // container will be focused upon loading
}

void ViewTab::updateDisplay(LocalView* view) {
    mWidget = view->widget();
    mWidget->setParent(this);
    mWidget->setObjectName("view");
    mContainer.hide();
    layout()->addWidget(mWidget);
    addActions(*mWidget);
    mWidget->setFocus(Qt::OtherFocusReason);
}

void ViewTab::addActions(QWidget & widget) {
    widget.addAction(& mNewVimCommandAction);
    widget.addAction(& mNewVimCommandAction);
    widget.addAction(& mBackwardAction);
    widget.addAction(& mForwardAction);
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

void ViewTab::focusView() {
    if (mWidget != NULL) {
        mWidget->setFocus(Qt::OtherFocusReason);
    }
    else {
        mContainer.setFocus(Qt::OtherFocusReason);
    }
}

void ViewTab::backward() {
   qDebug() << "---------- Backward";
   const HistoryEntry* historyEntry = mHistory.backward();
   if (historyEntry == NULL) {
       qDebug() << "Warning: trying to move backward but got an empty history entry";
   }
   else {
       setUrl(historyEntry->url());
       toggleBackwardAndForwardActionsIfNeeded();
   }
}

void ViewTab::forward() {
   qDebug() << "Forward";
   const HistoryEntry* historyEntry = mHistory.forward();
   if (historyEntry == NULL) {
       qDebug() << "Warning: trying to move forward but got an empty history entry";
   }
   else {
       setUrl(historyEntry->url());
       toggleBackwardAndForwardActionsIfNeeded();
   }
}

void ViewTab::toggleBackwardAndForwardActionsIfNeeded() {
    qDebug() << "Enabling/disabling actions";
    mBackwardAction.setEnabled(mHistory.canMoveBackward());
    mForwardAction.setEnabled(mHistory.canMoveForward());
    qDebug() << "mBackwardAction enabled? " << mBackwardAction.isEnabled();
    qDebug() << "mForwardAction enabled? " << mForwardAction.isEnabled();
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
