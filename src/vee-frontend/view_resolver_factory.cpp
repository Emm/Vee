#include "view_resolver_factory.h"
#include "web_view_builder.h"
#include "blank_view_builder.h"
#include "view_types.h"

ViewResolverFactory::ViewResolverFactory(const ProcessBuilder & processBuilder, QObject* parent) :
    QObject(parent),
    mViewCommands(new QVector<ViewCommand *>()),
    mProcessBuilder(processBuilder) {
    EmbedCommand command(QString("src/vee-web/vee-web"));
    command.addArgument("-w");
    command.addWinId();
    ViewCommand* viewCommand = new ViewCommand(command, WEB_VIEW_TYPE, "org.vee.WebView_%1", "/WebView");
    mViewCommands->append(viewCommand);
}

ViewResolverFactory::~ViewResolverFactory() {
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
