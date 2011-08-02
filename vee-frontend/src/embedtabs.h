#ifndef EMBEDTABS_H
#define EMBEDTABS_H

#include <QLabel>
#include <QTabWidget>

#include "embedcontainer.h"
#include "embedcommand.h"

class EmbedTabs: public QTabWidget {
    Q_OBJECT

private:
    int getTabPosition();

public:
    EmbedTabs(QWidget* parent=0);
    void embed(EmbedCommand& embedCommand);

public slots:
    void updateTabTitle(const QString & title);
    void updateTabUrl(const QString & url);
    void setUrl(const QString & url);

signals:
    void titleChanged(const QString & title);
    void urlChanged(const QString & url);
};

#endif
