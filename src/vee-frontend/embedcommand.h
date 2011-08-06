#ifndef EMBEDCOMMAND_H
#define EMBEDCOMMAND_H

#include <QString>
#include <QStringList>


class EmbedCommand {
public:
    const static int WIN_ID = 0;

    EmbedCommand(QString& executable);

    void addArgument(QString& argument);
    void addWinId();
    QString& executable();
    QStringList* arguments(ulong winId);
    void operator<< (QString argument);
    void operator<< (int WIN_ID);

private:
    int mWinIdArgPos;
    QString *mExecutable;
    QStringList *mArguments;
};

#endif
