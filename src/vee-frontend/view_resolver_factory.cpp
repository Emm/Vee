#include "view_resolver_factory.h"
#include "web_view_builder.h"
#include "blank_view_builder.h"
#include "view_types.h"

ViewResolverFactory::ViewResolverFactory(const ProcessBuilder & processBuilder, QObject* parent) :
    QObject(parent),
    mViewCommands(new QVector<ViewCommand *>()),
    mProcessBuilder(processBuilder) {
    EmbedCommand* command = new EmbedCommand(QString("src/vee-web/vee-web"));
    command->addArgument("-w");
    command->addWinId();
    ViewCommand* viewCommand = new ViewCommand;
    viewCommand->embedCommand = command;
    viewCommand->interfaceName = QString(WEB_VIEW_TYPE);
    viewCommand->serviceIdPattern = QString("org.vee.WebView_%1");
    viewCommand->objectPath = QString("/WebView");
    mViewCommands->append(viewCommand);
}

ViewResolverFactory::~ViewResolverFactory() {
    for (int i = 0 ; i < mViewCommands->size() ; i++) {
        ViewCommand* command = mViewCommands->at(i);
        delete command->embedCommand;
    }
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
        viewBuilders->append(new WebViewBuilder(command, mProcessBuilder));
    }
}
