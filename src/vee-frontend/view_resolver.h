#ifndef VIEW_RESOLVER_H
#define VIEW_RESOLVER_H

#include <QVector>
#include <QString>
#include "view_builder.h"

class ViewResolver : public QObject {

    Q_OBJECT

private:
    QVector<ViewBuilder*>* mViewBuilders;
    VeeViewInterface* mCurrentView;
    int mCurrentViewBuilderPos;
    ulong mIdentifier;
    QString mUrl;

    void tryWithNextBuilder();
    void cleanup();
    void disconnectAll();

public:
    explicit ViewResolver(QVector<ViewBuilder*>* viewBuilders, QObject* parent=0);
    virtual ~ViewResolver();
    void resolve(const QString & url, VeeViewInterface* currentView = 0);
    void setIdentifier(ulong identifier);
    ulong identifier() const;

private slots:
    void viewResolvedUrl();
    void viewDidntResolveUrl();
    void viewBuilderError(ViewBuilder::BuilderError error);

public slots:
    void askViewToResolve(VeeViewInterface* view);

signals:
    void urlResolved(VeeViewInterface* view);
    void unresolvableUrl(QString & url);
};

#endif
