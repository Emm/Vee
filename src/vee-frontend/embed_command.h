#ifndef EMBED_COMMAND_H
#define EMBED_COMMAND_H

#include <QString>
#include <QStringList>


class EmbedCommand {
public:
    const static int WIN_ID = 0;

    explicit EmbedCommand(const QString & executable);
    virtual ~EmbedCommand();

    void addArgument(const QString & argument);
    void addWinId();
    const QString & executable() const;
    QStringList arguments(ulong winId) const;
    void operator<< (QString argument);
    void operator<< (int WIN_ID);

private:
    QString mExecutable;
    QStringList mArguments;
    int mWinIdArgPos;
};

#endif
