#ifndef VEE_VIEW_INTERFACE_H
#define VEE_VIEW_INTERFACE_H

#include <QtCore/QObject>

class VeeViewInterface : public QObject {
    Q_OBJECT

public:
    Q_PROPERTY(QString title READ title)
    virtual QString title() const = 0;

    Q_PROPERTY(QString url READ url)
    virtual QString url() const = 0;

    Q_PROPERTY(QString interface READ interface)
    virtual QString interface() const = 0;

    explicit VeeViewInterface(QObject* parent = 0);

    virtual ~VeeViewInterface();

public slots:
    virtual void resolve(const QString &url) = 0;

signals:
    void titleChanged(QString title);
    void error(int errorType, int errorCode);
    void urlResolved();
    void urlNotResolved();
    void urlChanged(QString url);
};
#endif
