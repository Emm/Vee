#ifndef VEE_LOCAL_VIEW_H
#define VEE_LOCAL_VIEW_H

#include "view.h"

class VeeLocalView : public View {
    Q_OBJECT

public:
    explicit VeeLocalView(QObject* parent=0): View(parent) {};
    virtual ~VeeLocalView() {};
    virtual QWidget* widget() = 0;
};

#endif
