#ifndef VIEW_RESOLVER_H
#define VIEW_RESOLVER_H

#include <QVector>
#include <QString>
#include "view_builder.h"

class ViewResolver : public QObject {

    Q_OBJECT

private:
    QVector<ViewBuilder*>* mViewBuilders;
    View* mCurrentView;
    int mCurrentViewBuilderPos;
    ulong mIdentifier;
    QString mUrl;
    View* mTabView;

    void tryWithNextBuilder();
    void cleanup();
    void disconnectAll();
    ViewBuilder* currentBuilder();

public:
    explicit ViewResolver(QVector<ViewBuilder*>* viewBuilders, QObject* parent=0);
    virtual ~ViewResolver();
    void resolve(const QString & url, View* currentView = 0);
    void setIdentifier(ulong identifier);
    ulong identifier() const;

private slots:
    void viewResolvedUrl();
    void viewDidntResolveUrl();
    void viewBuilderError(View::ErrorType errorCode, int errorType);

public slots:
    void askViewToResolve(View* view);

signals:
    void urlResolved(View* view, QString viewType);
    void unresolvableUrl(QString & url);
};

#endif
