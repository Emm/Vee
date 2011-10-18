#ifndef BLANK_VIEW_BUILDER_H
#define BLANK_VIEW_BUILDER_H

#include "view_builder.h"

class BlankViewBuilder : public ViewBuilder {
    Q_OBJECT

private:
    const static QString* msViewType;
public:
    explicit BlankViewBuilder(QObject* parent=0);
    virtual ~BlankViewBuilder();

    virtual void build(const ulong identifier);
    virtual const QString & viewType() const;
};

#endif
