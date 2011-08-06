#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QLineEdit>
#include <QVBoxLayout>
#include <QWidget>
#include "embedtabs.h"
#include "embedcommand.h"

class MainWindow : public QWidget {
    Q_OBJECT

private slots:
    void setUrl(); 
    void setTitle(const QString & title);

public:
    MainWindow();
    void init(const QString& url);

private:
    void addTab();

    QLineEdit* urlBar;
    EmbedTabs *mTabs;
};
#endif
