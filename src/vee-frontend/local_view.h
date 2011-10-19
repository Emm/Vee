#ifndef LOCAL_VIEW_H
#define LOCAL_VIEW_H

#include "view.h"

class LocalView : public View {
    Q_OBJECT

public:
    explicit LocalView(QObject* parent=0): View(parent) {};
    virtual ~LocalView() {};
    virtual QWidget* widget() = 0;
};

#endif
