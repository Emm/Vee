#include "blank_view_builder.h"
#include "blank_view.h"
#include "view_types.h"

const QString* BlankViewBuilder::msViewType = new QString(BLANK_VIEW_TYPE);

BlankViewBuilder::BlankViewBuilder(QObject* parent) : ViewBuilder(parent) {
}

BlankViewBuilder::~BlankViewBuilder() {
}

void BlankViewBuilder::build(const ulong identifier) {
    BlankView* view = new BlankView();
    emit viewBuilt(view);
}

const QString & BlankViewBuilder::viewType() const {
    return *BlankViewBuilder::msViewType;
}
