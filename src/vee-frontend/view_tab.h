#ifndef VIEW_TAB_H
#define VIEW_TAB_H

#include <QAction>
#include <QX11EmbedContainer>
#include <QProcess>
#include <QLabel>
#include <QVBoxLayout>
#include <QDebug>

#include "vim.h"
#include "input_bar.h"
#include "view_resolver.h"
#include "remote_view.h"
#include "local_view.h"

class ViewTab: public QWidget {
    Q_OBJECT

private:
    Vim* mVim;
    ViewResolver* mViewResolver;
    View* mView;
    QString mOldLineEditValue;
    InputBar *mInputBar;
    QX11EmbedContainer *mContainer;
    QWidget* mWidget;
    QAction* mChangeUrlAction;
    QAction* mSwitchCommandAndNormalModeAction;
    QIcon mIcon;

    void discardOldView();
    void updateDisplay(RemoteView* view);
    void updateDisplay(LocalView* view);
    bool dispatchUpdateDisplay(View* view);
    void updateInputBar();

public:
    
    explicit ViewTab(Vim* mVim, ViewResolver* viewResolver, QWidget* parent=0);
    virtual ~ViewTab();

    Q_PROPERTY(QString viewType READ viewType)
    QString viewType() const;

    Q_PROPERTY(QIcon icon READ icon WRITE setIcon NOTIFY iconChanged)
    QIcon icon() const;
    void setIcon(QIcon icon);

    View* view() const;

    Vim* vim() const;

    QWidget* widget() const;

    InputBar* inputBar() const;

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
    void viewIconWasChanged();
    void viewGotAnError(View::ErrorType errorType, int errorCode);

signals:
    void urlChanged(const QString & title);
    void titleChanged(const QString & title);
    void iconChanged(QIcon icon);
    void openInNewTab(const QString & url);
    void closeTab();
};

#endif
