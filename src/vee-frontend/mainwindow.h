#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QLineEdit>
#include <QVBoxLayout>
#include <QWidget>
#include "embedtabs.h"
#include "view_resolver_factory.h"

class MainWindow : public QWidget {
    Q_OBJECT

private:
    QLineEdit* urlBar;
    EmbedTabs *mTabs;

public:
    explicit MainWindow(ViewResolverFactory & viewManagerFactory);
    virtual ~MainWindow();
    void init(const QString& url);

private slots:
    void urlChanged();
    void setTitle(const QString & title);

signals:
    void showUrlInActiveTab(const QString & url);
};
#endif
