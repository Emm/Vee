#ifndef VEE_VIEW_INTERFACE_H
#define VEE_VIEW_INTERFACE_H

#include <QtCore/QObject>
#include <QtCore/QByteArray>
#include <QtCore/QList>
#include <QtCore/QMap>
#include <QtCore/QString>
#include <QtCore/QStringList>
#include <QtCore/QVariant>
#include <QtDBus/QtDBus>
#include <QProcess>

class VeeViewInterface : public QDBusAbstractInterface {
    Q_OBJECT

private:
    QProcess* mProcess;

public:
    explicit VeeViewInterface(QProcess* process, const QString &service, const QString &path, const QString & interfaceName, const QDBusConnection &connection, QObject *parent = 0);
    virtual ~VeeViewInterface();

    Q_PROPERTY(QString title READ title)
    QString title() const;

public slots:
    QDBusPendingReply<> resolve(const QString &url);

signals:
    void titleChanged(QString title);
    void error(int errorType, int errorCode);
};
#endif