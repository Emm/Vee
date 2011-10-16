#ifndef VEE_LOCAL_VIEW_H
#define VEE_LOCAL_VIEW_H

#include "vee_view_interface.h"

class VeeLocalView : public VeeViewInterface {
    Q_OBJECT

public:
    explicit VeeLocalView(QObject* parent=0): VeeViewInterface(parent) {};
    virtual ~VeeLocalView() {};
    virtual QWidget* widget() = 0;
};

#endif
