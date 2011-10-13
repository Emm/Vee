#ifndef EMBEDCONTAINER_H
#define EMBEDCONTAINER_H

#include <QAction>
#include <QX11EmbedContainer>
#include <QProcess>
#include <QLabel>
#include <QVBoxLayout>
#include <QLineEdit>
#include <QDebug>

#include "view_resolver.h"

class EmbedContainer: public QWidget {
    Q_OBJECT

private:
    ViewResolver* mViewResolver;
    VeeViewInterface* mView;
    QString mViewType;
    QLineEdit *mInputBar;
    QX11EmbedContainer *mContainer;
    QAction* mChangeUrlAction;

    void disconnectView();
public:
    
    explicit EmbedContainer(ViewResolver* viewResolver, QWidget* parent=0);
    virtual ~EmbedContainer();

public slots:
    void setUrl(const QString & url);
    void setFailView(QString & url);
    void setView(VeeViewInterface* view, QString viewType);
    void focusContainer();
    void showEmbedError(QX11EmbedContainer::Error error);
    void resolveUrl();

signals:
    void urlChanged(const QString & title);
    void titleChanged(const QString & title);
};

#endif
