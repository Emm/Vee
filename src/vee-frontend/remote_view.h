#ifndef REMOTE_VIEW_H
#define REMOTE_VIEW_H
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
#include "process.h"
#include "view.h"

class RemoteView : public View {
    Q_OBJECT

protected:
    Process* mProcess;
    QDBusInterface* mRealInterface;

public:
    explicit RemoteView(Process* process, const QString &service, const QString &path, const QString & interfaceName, const QDBusConnection &connection, QObject *parent = 0);
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
