#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QWidget>
#include <QVBoxLayout>
#include "embedtabs.h"

class MainWindow : public QWidget {
    Q_OBJECT

public:
    MainWindow();
    void embed(EmbedCommand& embedCommand);

private:
    EmbedTabs *mTabs;
};
#endif
