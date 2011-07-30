#include "embedtabs.h"

EmbedTabs::EmbedTabs(QWidget* parent)
    : QTabWidget(parent) {
}

void EmbedTabs::embed(EmbedCommand& embedCommand) {
    EmbedContainer* container = new EmbedContainer(embedCommand);
    int newTabPosition = addTab(container, QString("Loading..."));
    connect(container, SIGNAL(titleChanged(const QString &)), this, SLOT(containerTitleChanged(const QString &)));
    setCurrentIndex(newTabPosition);
    container->embed();
    container->show();
}

void EmbedTabs::containerTitleChanged(const QString & title) {
    QObject * senderObj = QObject::sender();
    if (senderObj == 0) return;
    QWidget * sender = qobject_cast<QWidget *>(senderObj);
    int tabPosition = indexOf(sender);
    if (tabPosition < 0) return;
    setTabText(tabPosition, title);
    if (tabPosition == currentIndex()) {
        // Change window title
    }
}
