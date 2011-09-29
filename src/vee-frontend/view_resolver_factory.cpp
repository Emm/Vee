#include "view_resolver_factory.h"
#include <QtAlgorithms>

ViewResolverFactory::ViewResolverFactory(QObject* parent) : QObject(parent), mViewCommands(new QVector<VeeViewCommand *>()) {
    EmbedCommand* command = new EmbedCommand(QString("src/vee-web/vee-web"));
    command->addArgument("-w");
    command->addWinId();
    VeeViewCommand* veeViewCommand = new VeeViewCommand;
    veeViewCommand->embedCommand = command;
    veeViewCommand->interfaceName += "org.vee.VeeWebView";
    veeViewCommand->serviceIdPattern += "org.vee.VeeWebView_%1";
    veeViewCommand->objectPath += "/VeeWebView";
    mViewCommands->append(veeViewCommand);
}

ViewResolverFactory::~ViewResolverFactory() {
    qDeleteAll(*mViewCommands);
    delete mViewCommands;
}

ViewResolver* ViewResolverFactory::buildViewResolver() {
    QVector<ViewBuilder*>* viewBuilders = new QVector<ViewBuilder*>();
    for (int i = 0 ; i < mViewCommands->size() ; i++) {
        const VeeViewCommand & command = *( mViewCommands->at(i) );
        viewBuilders->append(new ViewBuilder(command));
    }
    ViewResolver* viewResolver = new ViewResolver(viewBuilders);
    return viewResolver;
}
