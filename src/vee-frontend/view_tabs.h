#ifndef VIEW_TABS_H
#define VIEW_TABS_H

#include <QLabel>
#include <QTabWidget>

#include "view_tab.h"
#include "embed_command.h"
#include "view_resolver_factory.h"

class ViewTabs: public QTabWidget {
    Q_OBJECT

private:
    ViewResolverFactory & mViewResolverFactory;
    int getTabPosition();

public:
    ViewTabs(ViewResolverFactory & viewManagerFactory, QWidget* parent=0);

public slots:
    void updateTabTitle(const QString & title);
    void updateTabUrl(const QString & url);
    void updateTabIcon(QIcon icon);
    void showUrlInNewTab(const QString & url);
    void showUrlInActiveTab(const QString & url);
    bool closeTab();

private slots:
    bool destroyTab(int position);

signals:
    void titleChanged(const QString & title);
    void urlChanged(const QString & url);
    void lastTabClosed();
};

#endif
