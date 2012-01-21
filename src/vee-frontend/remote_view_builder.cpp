#include "remote_view_builder.h"

RemoteViewBuilder::RemoteViewBuilder(const ViewCommand & viewCommand,
        const ProcessBuilder & processBuilder, QObject* parent) :
    ViewBuilder(parent),
    mViewCommand(viewCommand),
    mProcessBuilder(processBuilder)
    {
}

RemoteViewBuilder::~RemoteViewBuilder() {
}

const QString & RemoteViewBuilder::viewType() const {
    return mViewCommand.interfaceName;
}
