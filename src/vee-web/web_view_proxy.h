#ifndef WEB_VIEW_PROXY_H
#define WEB_VIEW_PROXY_H

#include <QWebView>
#include <QX11EmbedWidget>

class WebViewProxy : public QObject {

    Q_OBJECT

private:
    QWebView* mWebView;
    QX11EmbedWidget* mEmbedWidget;
    const ulong mWindowId;

    bool shouldEmbed();
public:
    explicit WebViewProxy(ulong windowId = 0, QObject* parent=0);
    virtual ~WebViewProxy();
    Q_PROPERTY(QUrl url READ url)
    QUrl url() const;

    Q_PROPERTY(QString title READ title)
    QString title() const;
    void show();

private slots:
    void broadcastLoadFinished(bool ok);

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
