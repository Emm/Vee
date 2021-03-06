#include "dummy_view_builder.h"
#include "dummy_view.h"

const QString DummyViewBuilder::msViewType = "org.vee.DummyView";

DummyViewBuilder::DummyViewBuilder(QObject* parent) : ViewBuilder(parent) {
}

DummyViewBuilder::~DummyViewBuilder() {
}

View* DummyViewBuilder::buildView() {
    DummyView* view = new DummyView();
    return view;
}

const QString & DummyViewBuilder::viewType() const {
    return msViewType;
}
