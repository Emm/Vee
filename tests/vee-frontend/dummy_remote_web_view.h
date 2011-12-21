#ifndef DUMMY_REMOTE_WEB_VIEW_H
#define DUMMY_REMOTE_WEB_VIEW_H

#include <QObject>
#include <QString>

class DummyRemoteWebView: public QObject {

Q_OBJECT

private:

    bool mEmbedded;
    bool mReloaded;
    bool mStopped;
    QString * mResolvedUrl;
    QString * mHtml;

public:

    explicit DummyRemoteWebView(QObject* parent = 0);

    virtual ~DummyRemoteWebView();

    Q_PROPERTY(QString title READ title)
    QString title() const;

    Q_PROPERTY(QString url READ url)
    QString url() const;

    void emitUrlResolved();

    void emitUrlNotResolved();

    void emitUrlChanged(const QString & url);

    void emitTitleChanged(const QString & title);

    void emitError(int errorType, int errorCode);

    bool embedded() const;

    bool reloaded() const;

    bool stopped() const;

    QString * resolvedUrl() const;

    QString * html() const;

public slots:

    void embed();

    void resolve(const QString & url);

    void reload();

    void setHtml(const QString & html);

    void stop();

signals:

    void error(int errorType, int errorCode);
    void iconChanged();
    void linkClicked(const QString &url);
    void loadProgress(int progress);
    void loadStarted();
    void selectionChanged();
    void titleChanged(const QString &title);
    void urlChanged(const QString &url);
    void urlNotResolved();
    void urlResolved();
};

#endif
