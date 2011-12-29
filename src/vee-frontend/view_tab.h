#ifndef VIEW_TAB_H
#define VIEW_TAB_H

#include <QAction>
#include <QX11EmbedContainer>
#include <QProcess>
#include <QLabel>
#include <QVBoxLayout>
#include <QLineEdit>
#include <QDebug>

#include "vim.h"
#include "view_resolver.h"

class ViewTab: public QWidget {
    Q_OBJECT

private:
    Vim* mVim;
    ViewResolver* mViewResolver;
    View* mView;
    QString mOldLineEditValue;
    QLineEdit *mInputBar;
    QX11EmbedContainer *mContainer;
    QWidget* mWidget;
    QAction* mChangeUrlAction;
    QAction* mSwitchCommandAndNormalModeAction;

public:
    
    explicit ViewTab(Vim* mVim, ViewResolver* viewResolver, QWidget* parent=0);
    virtual ~ViewTab();

    const View* view();

private slots:
    void switchCommandAndNormalModes(bool switchToCommandMode);
    void triggerVimParsing();

public slots:
    void setUrl(const QString & url);
    void setFailView(QString url);
    void setView(View* view);
    void focusContainer();
    void showEmbedError(QX11EmbedContainer::Error error);
    void resolveUrl();

signals:
    void urlChanged(const QString & title);
    void titleChanged(const QString & title);
    void iconChanged();
    void openInNewTab(const QString & url);
};

#endif
