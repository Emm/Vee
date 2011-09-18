#include "view_resolver_factory.h"

ViewResolverFactory::ViewResolverFactory(QObject* parent) : QObject(parent) {
}

ViewResolverFactory::~ViewResolverFactory() {
}

ViewResolver* ViewResolverFactory::buildViewResolver() {
    QVector<ViewBuilder*>* viewBuilders = new QVector<ViewBuilder*>();
    EmbedCommand* command = new EmbedCommand(QString("src/vee-web/vee-web"));
    command->addArgument("-w");
    command->addWinId();
    viewBuilders->append(new ViewBuilder(command,
                QString("org.vee.VeeWebView"), QString("org.vee.VeeWebView_%1"),
                QString("/VeeWebView")));
    ViewResolver* viewResolver = new ViewResolver(viewBuilders);
    return viewResolver;
}
