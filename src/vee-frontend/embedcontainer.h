#ifndef EMBEDCONTAINER_H
#define EMBEDCONTAINER_H

#include <QX11EmbedContainer>
#include <QProcess>
#include <QLabel>
#include <QVBoxLayout>
#include <QDebug>

#include "view_resolver.h"

class EmbedContainer: public QWidget {
    Q_OBJECT

private:
    ViewResolver* mViewResolver;
    VeeViewInterface* mView;
    QX11EmbedContainer *mContainer;

    void disconnectView();
public:
    
    explicit EmbedContainer(ViewResolver* viewResolver, QWidget* parent=0);
    virtual ~EmbedContainer();

public slots:
    void setUrl(const QString & url);
    void setFailView(QString & url);
    void setView(VeeViewInterface* view);

signals:
    void urlChanged(const QString & title);
    void titleChanged(const QString & title);
};

#endif
