#ifndef VIEW_MANAGER_H
#define VIEW_MANAGER_H

#include <QX11EmbedContainer>
#include <QVector>
#include "view_command.h"
#include "view_resolver.h"

class ViewResolverFactory : public QObject {

    Q_OBJECT

private:
    QVector<ViewCommand *>* mViewCommands;
    void insertLocalBuilders(QVector<ViewBuilder*>* viewBuilders);
    void insertRemoteBuilders(QVector<ViewBuilder*>* viewBuilders);

public:
    explicit ViewResolverFactory(QObject* parent=0);
    virtual ~ViewResolverFactory();
    ViewResolver* buildViewResolver();
};

#endif
