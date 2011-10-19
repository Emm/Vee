#include "view_resolver_factory.h"
#include "remote_view_builder.h"
#include "blank_view_builder.h"
#include <QtAlgorithms>
#include "view_types.h"

ViewResolverFactory::ViewResolverFactory(QObject* parent) : QObject(parent), mViewCommands(new QVector<ViewCommand *>()) {
    EmbedCommand* command = new EmbedCommand(QString("src/vee-web/vee-web"));
    command->addArgument("-w");
    command->addWinId();
    ViewCommand* veeViewCommand = new ViewCommand;
    veeViewCommand->embedCommand = command;
    veeViewCommand->interfaceName += WEB_VIEW_TYPE;
    veeViewCommand->serviceIdPattern += "org.vee.WebView_%1";
    veeViewCommand->objectPath += "/WebView";
    mViewCommands->append(veeViewCommand);
}

ViewResolverFactory::~ViewResolverFactory() {
    qDeleteAll(*mViewCommands);
    delete mViewCommands;
}

ViewResolver* ViewResolverFactory::buildViewResolver() {
    QVector<ViewBuilder*>* viewBuilders = new QVector<ViewBuilder*>();
    insertLocalBuilders(viewBuilders);
    insertRemoteBuilders(viewBuilders);
    ViewResolver* viewResolver = new ViewResolver(viewBuilders);
    return viewResolver;
}

void ViewResolverFactory::insertLocalBuilders(QVector<ViewBuilder*>* viewBuilders) {
    BlankViewBuilder* builder = new BlankViewBuilder();
    viewBuilders->append(builder);
}

void ViewResolverFactory::insertRemoteBuilders(QVector<ViewBuilder*>* viewBuilders) {
    for (int i = 0 ; i < mViewCommands->size() ; i++) {
        const ViewCommand & command = *( mViewCommands->at(i) );
        viewBuilders->append(new RemoteViewBuilder(command));
    }
}
