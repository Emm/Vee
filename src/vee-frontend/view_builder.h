#ifndef VIEW_BULDER_H
#define VIEW_BULDER_H

#include <QDBusServiceWatcher>
#include <QProcess>
#include "embedcommand.h"
#include "vee_view_interface.h"
#include "vee_view_command.h"

class ViewBuilder : public QObject {
    Q_OBJECT

public:
    enum BuilderError {
        WrongServiceNameError = 0,
        WrongInterfaceNameError = 1,
        ProcessFailedToStart = 2,
        ProcessCrashed = 3,
        UnknownError = 4
    };
    explicit ViewBuilder(QObject* parent=0) : QObject(parent) {};
    virtual ~ViewBuilder() {};
    virtual void build(const ulong identifier) = 0;
    virtual const QString & viewType() const = 0;

signals:
    void error(ViewBuilder::BuilderError error);
    void viewBuilt(VeeViewInterface* view);
};

#endif
