#include "embedtabs.h"

EmbedTabs::EmbedTabs(QWidget* parent)
    : QTabWidget(parent) {
}

void EmbedTabs::embed(EmbedCommand& embedCommand) {
    EmbedContainer* container = new EmbedContainer(embedCommand);
    int newTabPosition = addTab(container, QString("Loading..."));
    connect(container, SIGNAL(titleChanged(const QString &)), this, SLOT(updateTabTitle(const QString &)));
    connect(container, SIGNAL(urlChanged(const QString &)), this, SLOT(updateTabUrl(const QString &)));
    setCurrentIndex(newTabPosition);
    container->embed();
    container->show();
}

int EmbedTabs::getTabPosition() {
    QObject * senderObj = QObject::sender();
    if (senderObj == 0) return -1;
    QWidget * sender = qobject_cast<QWidget *>(senderObj);
    int tabPosition = indexOf(sender);
    if (tabPosition < 0) return -1;
    return tabPosition;
}

void EmbedTabs::updateTabTitle(const QString & title) {
    int tabPosition = getTabPosition();
    if (tabPosition == -1)
        return;
    setTabText(tabPosition, title);
    if (tabPosition == currentIndex()) {
        emit titleChanged(title);
    }
}

void EmbedTabs::updateTabUrl(const QString & url) {
    qDebug() << "updateTabUrl(" << url << ")";
    int tabPosition = getTabPosition();
    if (tabPosition == -1)
        return;
    if (tabPosition == currentIndex()) {
        emit urlChanged(url);
    }
}

void EmbedTabs::setUrl(const QString & url) {
    EmbedContainer * widget = qobject_cast<EmbedContainer *>(currentWidget());
    widget->setUrl(url);
}
