#include "view_tabs.h"
#include "vim.h"

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
    Vim* vim = new Vim();
    ViewResolver * viewResolver = mViewResolverFactory.buildViewResolver();
    ViewTab* container = new ViewTab(vim, viewResolver, this);
    int activeTabPosition = currentIndex();
    int newTabPosition = insertTab(activeTabPosition+1, container, QString("Loading..."));
    connect(container, SIGNAL(titleChanged(const QString &)), this, SLOT(updateTabTitle(const QString &)));
    connect(container, SIGNAL(urlChanged(const QString &)), this, SLOT(updateTabUrl(const QString &)));
    connect(container, SIGNAL(openInNewTab(const QString &)), this, SLOT(showUrlInNewTab(const QString &)));
    connect(container, SIGNAL(iconChanged(QIcon)), this, SLOT(updateTabIcon(QIcon)));
    connect(this, SIGNAL(tabCloseRequested(int)), this, SLOT(destroyTab(int)));
    setCurrentIndex(newTabPosition);
    container->setUrl(url);
}

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

void ViewTabs::updateTabIcon(QIcon icon) {
    int tabPosition = getTabPosition();
    if (tabPosition == -1)
        return;
    setTabIcon(tabPosition, icon);
}

bool ViewTabs::closeTab() {
    int tabPosition = getTabPosition();
    if (tabPosition == -1)
        return false;
    return destroyTab(tabPosition);
}

bool ViewTabs::destroyTab(int position) {
    QWidget* page = widget(position);
    if (page == NULL)
        return false;

    page->deleteLater();
    removeTab(position);
    if (count() == 0) {
        emit lastTabClosed();
    }
    return true;
}
