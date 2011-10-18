#include "blank_view_builder.h"
#include "vee_blank_view.h"
#include "view_types.h"

const QString* BlankViewBuilder::msViewType = new QString(BLANK_VIEW_TYPE);

BlankViewBuilder::BlankViewBuilder(QObject* parent) : ViewBuilder(parent) {
}

BlankViewBuilder::~BlankViewBuilder() {
}

void BlankViewBuilder::build(const ulong identifier) {
    VeeBlankView* view = new VeeBlankView();
    emit viewBuilt(view);
}

const QString & BlankViewBuilder::viewType() const {
    return *BlankViewBuilder::msViewType;
}
