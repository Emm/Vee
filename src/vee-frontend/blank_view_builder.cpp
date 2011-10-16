#include "blank_view_builder.h"
#include "vee_blank_view.h"
#include "view_types.h"

BlankViewBuilder::BlankViewBuilder(QObject* parent) : ViewBuilder(parent) {
}

BlankViewBuilder::~BlankViewBuilder() {
}

void BlankViewBuilder::build(const ulong identifier) {
    VeeBlankView* view = new VeeBlankView();
    emit viewBuilt(view);
}

const QString & BlankViewBuilder::viewType() const {
    return BLANK_VIEW_TYPE;
}
