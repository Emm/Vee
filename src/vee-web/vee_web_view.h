#ifndef VEE_WEB_VIEW_H
#define VEE_WEB_VIEW_H

#include <QWebView>
#include <QX11EmbedWidget>

class VeeWebView : public QObject {

    Q_OBJECT

private:
    QWebView* mWebView;
 /*   QX11EmbedWidget* mEmbedWidget;*/
    const ulong mWindowId;

    bool shouldEmbed();
public:
    explicit VeeWebView(ulong windowId = 0, QObject* parent=0);
    virtual ~VeeWebView();
    Q_PROPERTY(QUrl url READ url)
    QUrl url() const;

    Q_PROPERTY(QString title READ title)
    QString title() const;

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
