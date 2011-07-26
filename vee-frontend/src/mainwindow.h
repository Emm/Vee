#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QLineEdit>
#include <QVBoxLayout>
#include <QWidget>
#include "embedtabs.h"
#include "embedcommand.h"

class MainWindow : public QWidget {
    Q_OBJECT

public slots:
    void addTab();

public:
    MainWindow();
    void init(const QString& url);

private:
    QLineEdit* urlBar;
    EmbedTabs *mTabs;
};
#endif
