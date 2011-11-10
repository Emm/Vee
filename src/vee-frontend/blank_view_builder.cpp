#include "blank_view_builder.h"
#include "blank_view.h"
#include "view_types.h"

const QString* BlankViewBuilder::msViewType = new QString(BLANK_VIEW_TYPE);

BlankViewBuilder::BlankViewBuilder(QObject* parent) : ViewBuilder(parent) {
}

BlankViewBuilder::~BlankViewBuilder() {
}

View* BlankViewBuilder::buildView() {
    BlankView* view = new BlankView();
    return view;
}

const QString & BlankViewBuilder::viewType() const {
    return *BlankViewBuilder::msViewType;
}
