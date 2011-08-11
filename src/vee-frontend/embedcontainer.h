#ifndef EMBEDCONTAINER_H
#define EMBEDCONTAINER_H

#include <QX11EmbedContainer>
#include <QProcess>
#include <QLabel>
#include <QVBoxLayout>
#include <QDebug>

#include "vee_web_view_interface.h"
#include "embedcommand.h"

class EmbedContainer: public QWidget {
    Q_OBJECT

public:
    
    EmbedContainer(EmbedCommand& embedCommand, QWidget* parent=0);
    void embed();

public slots:

    void clientIsEmbedded();
    void error(QX11EmbedContainer::Error error); 
    void setUrl(const QString & url);

signals:
    void urlChanged(const QString & title);
    void titleChanged(const QString & title);
    void clientClosed();

private:
    QProcess *mProcess;
    QString *mExecutable;
    QStringList *mArguments;
    QLabel *mLabel;
    QX11EmbedContainer *mContainer;
    OrgVeeWebVeeWebViewInterface *mInterface;
};

#endif
