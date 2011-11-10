#ifndef VIEW_RESOLVER_FACTORY_H
#define VIEW_RESOLVER_FACTORY_H

#include <QX11EmbedContainer>
#include <QVector>

#include "process_builder.h"
#include "view_command.h"
#include "view_resolver.h"

class ViewResolverFactory : public QObject {

    Q_OBJECT

private:

    QVector<ViewCommand *>* mViewCommands;
    const ProcessBuilder& mProcessBuilder;

    void insertLocalBuilders(QVector<ViewBuilder*>* viewBuilders);

    void insertRemoteBuilders(QVector<ViewBuilder*>* viewBuilders);

public:

    explicit ViewResolverFactory(const ProcessBuilder & processBuilder, QObject* parent=0);

    virtual ~ViewResolverFactory();

    ViewResolver* buildViewResolver();
};

#endif
