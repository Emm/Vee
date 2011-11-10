#ifndef REMOTE_VIEW_BULDER_H
#define REMOTE_VIEW_BULDER_H

#include "process_builder.h"
#include "view_builder.h"
#include "view.h"
#include "view_command.h"

class RemoteViewBuilder : public ViewBuilder {

Q_OBJECT

public:

    explicit RemoteViewBuilder(const ViewCommand & viewCommand, const ProcessBuilder & processBuilder, QObject* parent=0);
    virtual ~RemoteViewBuilder();

    virtual const QString & viewType() const;

protected:

    const ViewCommand & mViewCommand;
    const ProcessBuilder & mProcessBuilder;
};

#endif
