#include "view_tab.h"
#include <QVBoxLayout>
#include "remote_view.h"
#include "local_view.h"
#include "error_view.h"

ViewTab::ViewTab(Vim* vim, ViewResolver* viewResolver, QWidget* parent):
        QWidget(parent),
        mVim(vim),
        mViewResolver(viewResolver),
        mView(NULL),
        mInputBar(new InputBar(this)),
        mContainer(new QX11EmbedContainer(this)),
        mWidget(NULL),
        mChangeUrlAction(new QAction(this)),
        mSwitchCommandAndNormalModeAction(new QAction(this)) {
    setLayout(new QVBoxLayout());

    mVim->setParent(this);
    mViewResolver->setParent(this);

    layout()->addWidget(mInputBar);
    layout()->addWidget(mContainer);
    addAction(mChangeUrlAction);
    addAction(mSwitchCommandAndNormalModeAction);

    mSwitchCommandAndNormalModeAction->setCheckable(true);
    mSwitchCommandAndNormalModeAction->setShortcut(QKeySequence(Qt::Key_Colon));

    mViewResolver->setParent(this);
    mViewResolver->setIdentifier(mContainer->winId());

    connect(mInputBar, SIGNAL(returnPressed()), mChangeUrlAction, SLOT(trigger()));
    connect(mChangeUrlAction, SIGNAL(triggered()), this, SLOT(resolveUrl()));
    connect(mSwitchCommandAndNormalModeAction, SIGNAL(toggled(bool)), this, SLOT(switchCommandAndNormalModes(bool)));
    connect(mViewResolver, SIGNAL(urlResolved(View*)), this, SLOT(setView(View*)));
    connect(mViewResolver, SIGNAL(unresolvableUrl(QString)), this, SLOT(setFailView(QString)));
    connect(mContainer, SIGNAL(clientIsEmbedded()), this, SLOT(focusContainer()));
    connect(mContainer, SIGNAL(error(QX11EmbedContainer::Error)), this, SLOT(showEmbedError(QX11EmbedContainer::Error)));

    connect(mVim, SIGNAL(openCommand(QString)), this, SLOT(setUrl(const QString &)));
    connect(mVim, SIGNAL(openInNewTabCommand(QString)), this, SIGNAL(openInNewTab(const QString &)));
}

ViewTab::~ViewTab() {
}

void ViewTab::setUrl(const QString & url) {
    // Switch back to normal mode if we were in command mode
    if (mSwitchCommandAndNormalModeAction->isChecked()) {
        mSwitchCommandAndNormalModeAction->toggle();
    }
    mInputBar->setText(url);
    mChangeUrlAction->trigger();
}

void ViewTab::setView(View* view) {
    if (view != mView) {
        if (mView) {
            mView->disconnect();
            if (mWidget != NULL) {
                layout()->removeWidget(mWidget);
                delete mWidget;
                mWidget = NULL;
                qDebug() << "Removed old widget";
            }
            delete mView;
        }
        mView = view;
        mInputBar->setText(view->url());
        qDebug() << "New view type: " << mView->interface();
        RemoteView* remoteInt = qobject_cast<RemoteView *>(mView);
        if (remoteInt) {
            mView->setParent(mContainer);
            mContainer->show();
            remoteInt->embed();
        }
        else {
            LocalView* localView = qobject_cast<LocalView *>(mView);
            localView->setParent(this);
            mWidget = localView->widget();
            mWidget->setParent(this);
            mContainer->hide();
            layout()->addWidget(mWidget);
        }
        connect(mView, SIGNAL(titleChanged(const QString &)), this, SIGNAL(titleChanged(const QString &)));
        connect(mView, SIGNAL(urlChanged(const QString &)), mInputBar, SLOT(setText(const QString &)));
        connect(mView, SIGNAL(iconChanged()), this, SIGNAL(iconChanged()));
        emit titleChanged(mView->title());
        emit iconChanged();
    }
}

const View * ViewTab::view() {
    return mView;
}

void ViewTab::setFailView(QString url) {
    QString msg = QString("The URL '%1' is not valid and cannot be loaded").arg(url);
    ErrorView* errorView = new ErrorView(msg);
    setView(errorView);
}

void ViewTab::focusContainer() {
    qDebug() << "embedded";
    mContainer->setFocus(Qt::OtherFocusReason);
}

void ViewTab::showEmbedError(QX11EmbedContainer::Error error) {
    qDebug() << "error while embedding: " << error;
    // FIXME actually handle error
}

void ViewTab::resolveUrl() {
    qDebug() << "resolveUrl";
    const QString & url = mInputBar->text();
    mViewResolver->resolve(url, mView);
}

void ViewTab::switchCommandAndNormalModes(bool switchToCommandMode) {
    if (switchToCommandMode) {
        mVim->setMode(Vim::CommandMode);
        mSwitchCommandAndNormalModeAction->setShortcut(Qt::Key_Escape);
        mOldLineEditValue = mInputBar->text();
        mInputBar->clear();
        mInputBar->setFocus(Qt::ShortcutFocusReason);
        disconnect(mInputBar, SIGNAL(returnPressed()), mChangeUrlAction, SLOT(trigger()));
        connect(mInputBar, SIGNAL(returnPressed()), this, SLOT(triggerVimParsing()));


        qDebug() << "Switch to command mode";
    }
    else {
        mSwitchCommandAndNormalModeAction->setShortcut(Qt::Key_Colon);
        mVim->setMode(Vim::NormalMode);
        mInputBar->setText(mOldLineEditValue);
        mInputBar->clearFocus();
        connect(mInputBar, SIGNAL(returnPressed()), mChangeUrlAction, SLOT(trigger()));
        qDebug() << "Switch to normal mode";
    }
}

void ViewTab::triggerVimParsing() {
    mVim->parse(mInputBar->text());
}
