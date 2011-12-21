#ifndef DUMMY_VIEW_BUILDER_H
#define DUMMY_VIEW_BUILDER_H

#include "view_builder.h"

class DummyViewBuilder : public ViewBuilder {
    Q_OBJECT

private:

    const static QString* msViewType;

public:

    explicit DummyViewBuilder(QObject* parent=0);
    virtual ~DummyViewBuilder();

    virtual const QString & viewType() const;

protected:

    virtual View* buildView();
};

#endif
