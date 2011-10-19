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

class ViewTab: public QWidget {
    Q_OBJECT

private:
    ViewResolver* mViewResolver;
    View* mView;
    QString mViewType;
    QLineEdit *mInputBar;
    QX11EmbedContainer *mContainer;
    QWidget* mWidget;
    QAction* mChangeUrlAction;

    void disconnectView();
public:
    
    explicit ViewTab(ViewResolver* viewResolver, QWidget* parent=0);
    virtual ~ViewTab();

public slots:
    void setUrl(const QString & url);
    void setFailView(QString & url);
    void setView(View* view, QString viewType);
    void focusContainer();
    void showEmbedError(QX11EmbedContainer::Error error);
    void resolveUrl();

signals:
    void urlChanged(const QString & title);
    void titleChanged(const QString & title);
};

#endif
