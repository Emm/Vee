#ifndef WEB_VIEW_PROXY_H
#define WEB_VIEW_PROXY_H

#include <QWebView>
#include <QX11EmbedWidget>
#include <QAction>

class WebViewProxy : public QObject {

    Q_OBJECT

private:
    QWebView* mWebView;
    QX11EmbedWidget* mEmbedWidget;
    QAction* mScrollDownAction;
    QAction* mScrollUpAction;
    QAction* mScrollLeftAction;
    const ulong mWindowId;
    int mScrollIncrement;

    bool shouldEmbed();
    void scroll(int dx, int dy);


public:
    explicit WebViewProxy(ulong windowId = 0, int scrollIncrement = 10, QObject* parent=0);
    virtual ~WebViewProxy();
    Q_PROPERTY(QUrl url READ url)
    QUrl url() const;

    Q_PROPERTY(QString title READ title)
    QString title() const;
    void show();

private slots:
    void broadcastLoadFinished(bool ok);
    void scrollDown();
    void scrollUp();
    void scrollLeft();

public slots:
    void resolve(const QString & value);
    void reload();
    void stop();
    void setHtml(const QString & html);
    void embed();

signals:
    void iconChanged();
    void linkClicked(const QUrl & url);
    void loadProgress(int progress);
    void loadStarted();
    void selectionChanged();
    void titleChanged(const QString & title);
    void urlChanged(const QUrl & url);
    void urlResolved();
    void urlNotResolved();
    void error(int errorType, int errorCode);
};

#endif
