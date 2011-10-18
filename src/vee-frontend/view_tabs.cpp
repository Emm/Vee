#include "view_tabs.h"

ViewTabs::ViewTabs(ViewResolverFactory & viewManagerFactory, QWidget* parent)
    : QTabWidget(parent), mViewResolverFactory(viewManagerFactory) {
}

void ViewTabs::showUrlInActiveTab(const QString & url) {
    if (count() >0) {
        ViewTab * container = qobject_cast<ViewTab *>(currentWidget());
        if (container != NULL) {
            container->setUrl(url);
        }
    }
    else {
        showUrlInNewTab(url);
    }
}

void ViewTabs::showUrlInNewTab(const QString & url) {
    ViewResolver * viewResolver = mViewResolverFactory.buildViewResolver();
    ViewTab* container = new ViewTab(viewResolver, this);
    int newTabPosition = addTab(container, QString("Loading..."));
    connect(container, SIGNAL(titleChanged(const QString &)), this, SLOT(updateTabTitle(const QString &)));
    connect(container, SIGNAL(urlChanged(const QString &)), this, SLOT(updateTabUrl(const QString &)));
    setCurrentIndex(newTabPosition);
    container->setUrl(url);
}

/*
void ViewTabs::embed(EmbedCommand& embedCommand) {
    ViewTab* container = new ViewTab(embedCommand);
    int newTabPosition = addTab(container, QString("Loading..."));
    connect(container, SIGNAL(titleChanged(const QString &)), this, SLOT(updateTabTitle(const QString &)));
    connect(container, SIGNAL(urlChanged(const QString &)), this, SLOT(updateTabUrl(const QString &)));
    setCurrentIndex(newTabPosition);
    container->embed();
    container->show();
}
*/

int ViewTabs::getTabPosition() {
    QObject * senderObj = QObject::sender();
    if (senderObj == NULL) return -1;
    QWidget * sender = qobject_cast<QWidget *>(senderObj);
    if (sender == NULL) return -1;
    int tabPosition = indexOf(sender);
    if (tabPosition < 0) return -1;
    return tabPosition;
}

void ViewTabs::updateTabTitle(const QString & title) {
    int tabPosition = getTabPosition();
    if (tabPosition == -1)
        return;
    setTabText(tabPosition, title);
    if (tabPosition == currentIndex()) {
        emit titleChanged(title);
    }
}

void ViewTabs::updateTabUrl(const QString & url) {
    qDebug() << "updateTabUrl(" << url << ")";
    int tabPosition = getTabPosition();
    if (tabPosition == -1)
        return;
    if (tabPosition == currentIndex()) {
        emit urlChanged(url);
    }
}
