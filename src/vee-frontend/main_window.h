#ifndef MAIN_WINDOW_H
#define MAIN_WINDOW_H

#include <QVBoxLayout>
#include <QWidget>
#include "view_tabs.h"
#include "view_resolver_factory.h"

class MainWindow : public QWidget {
    Q_OBJECT

private:
    ViewTabs *mTabs;

public:
    explicit MainWindow(ViewResolverFactory & viewManagerFactory);
    virtual ~MainWindow();
    void init(const QString& url);

private slots:
    void setTitle(const QString & title);

signals:
    void showUrlInActiveTab(const QString & url);
};
#endif
