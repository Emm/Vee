#ifndef EMBEDCONTAINER_H
#define EMBEDCONTAINER_H

#include <QX11EmbedContainer>
#include <QProcess>
#include <QLabel>
#include <QVBoxLayout>
#include <QDebug>

#include "embedcommand.h"

class EmbedContainer: public QWidget {
    Q_OBJECT

public:
    
    EmbedContainer(EmbedCommand& embedCommand, QWidget* parent=0);
    void embed();

public slots:

    void clientClosed();
    void clientIsEmbedded();
    void error(QX11EmbedContainer::Error error); 

private:
    QProcess *mProcess;
    QString *mExecutable;
    QStringList *mArguments;
    QLabel *mLabel;
    QX11EmbedContainer *mContainer;
};

#endif
