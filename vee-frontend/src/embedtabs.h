#ifndef EMBEDTABS_H
#define EMBEDTABS_H

#include <QLabel>
#include <QTabWidget>
#include "embedcontainer.h"
#include "embedcommand.h"

class EmbedTabs: public QTabWidget {
    Q_OBJECT

public:

    EmbedTabs(QWidget* parent=0);
    void embed(EmbedCommand& embedCommand);
};

#endif
