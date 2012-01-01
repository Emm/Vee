#ifndef VIEW_H
#define VIEW_H

#include <QtCore/QObject>
#include <QIcon>

class View : public QObject {
    Q_OBJECT

protected:
    QIcon mIcon;

public:

    enum ErrorType { ProcessError, UnknownError };

    Q_PROPERTY(QString title READ title)
    virtual QString title() const = 0;

    Q_PROPERTY(QString url READ url)
    virtual QString url() const = 0;

    Q_PROPERTY(QString interface READ interface)
    virtual QString interface() const = 0;

    explicit View(QObject* parent = 0);

    virtual void init(const ulong identifier);

    virtual ~View();

    virtual QIcon icon() const;

public slots:
    virtual void resolve(const QString &url) = 0;

signals:
    void titleChanged(QString title);
    void error(View::ErrorType errorType, int errorCode);
    void urlResolved();
    void urlNotResolved();
    void urlChanged(QString url);
    void initialized();
    void iconChanged();
    void loadStarted();
    void loadProgress(int progress);
    void loadFinished(bool ok);
};

#endif
