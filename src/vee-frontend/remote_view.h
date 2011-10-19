#ifndef VEE_VIEW_REMOTE_INTERFACE_H
#define VEE_VIEW_REMOTE_INTERFACE_H
/*
#include <QtCore/QByteArray>
#include <QtCore/QList>
#include <QtCore/QMap>
#include <QtCore/QString>
#include <QtCore/QStringList>
#include <QtCore/QVariant>
#include <QtDBus/QtDBus>
*/

#include <QDBusInterface>
#include <QProcess>
#include "view.h"

class RemoteView : public View {
    Q_OBJECT

protected:
    QProcess* mProcess;
    QDBusInterface* mRealInterface;

public:
    explicit RemoteView(QProcess* process, const QString &service, const QString &path, const QString & interfaceName, const QDBusConnection &connection, QObject *parent = 0);
    virtual ~RemoteView();

    Q_PROPERTY(QString title READ title)
    QString title() const;

    Q_PROPERTY(QString url READ url)
    QString url() const;

    Q_PROPERTY(QString interface READ interface)
    QString interface() const;
public slots:
    void embed();
    virtual void resolve(const QString &url);
};

#endif