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
#include "tab_history.h"

class ViewTab: public QWidget {
    Q_OBJECT

private:
    Vim* mVim;
    ViewResolver* mViewResolver;
    View* mView;
    TabHistory mHistory;
    InputBar mInputBar;
    QX11EmbedContainer mContainer;
    QWidget* mWidget;
    QAction mChangeUrlAction;
    QAction mNewVimCommandAction;
    QAction mBackwardAction;
    QAction mForwardAction;
    QIcon mIcon;
    QString mUrl;
    QString mTitle;

    void discardOldView();
    void updateDisplay(RemoteView* view);
    void updateDisplay(LocalView* view);
    bool dispatchUpdateDisplay(View* view);
    void updateInputBar();
    void addSkeletonEntryToHistory(const QString & url);
    void completeHistoryEntry();
    void addActions(QWidget & widget);
    void toggleBackwardAndForwardActionsIfNeeded();

    const static QString VIM_COMMAND_PREFIX;
public:
    
    explicit ViewTab(Vim* mVim, ViewResolver* viewResolver, QWidget* parent=0);
    virtual ~ViewTab();

    Q_PROPERTY(QString viewType READ viewType)
    QString viewType() const;

    Q_PROPERTY(QIcon icon READ icon WRITE setIcon NOTIFY iconChanged)
    QIcon icon() const;
    void setIcon(QIcon icon);
    void setTitle(const QString & title);

    View* view() const;

    Vim* vim() const;

    QWidget* widget() const;

    InputBar* inputBar();

private slots:
    void newVimCommand();
    void blurInputBar();
    void triggerVimParsing();

public slots:
    void setUrl(const QString & url);
    void newUrl(const QString & url);
    void setFailView(QString url);
    void setView(View* view);
    void focusContainer();
    void showEmbedError(QX11EmbedContainer::Error error);
    void resolveUrl();
    void viewIconWasChanged();
    void viewGotAnError(View::ErrorType errorType, int errorCode);
    void focusView();
    void backward();
    void forward();

signals:
    void urlChanged(const QString & title);
    void titleChanged(const QString & title);
    void iconChanged(QIcon icon);
    void openInNewTab(const QString & url);
    void closeTab();
};

#endif
