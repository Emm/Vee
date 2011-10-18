#ifndef VEE_BLANK_VIEW_H
#define VEE_BLANK_VIEW_H

#include <QWidget>
#include "vee_local_view.h"

class VeeBlankView : public VeeLocalView {
    Q_OBJECT

private:
    const static QString msInterface;
    const static QString msTitle;
    const static QString msUrl;

public:
    explicit VeeBlankView(QObject* parent = 0);
    virtual ~VeeBlankView();

    Q_PROPERTY(QString title READ title)
    virtual QString title() const;

    Q_PROPERTY(QString url READ url)
    virtual QString url() const;

    Q_PROPERTY(QString interface READ interface)
    virtual QString interface() const;

    virtual QWidget* widget();

public slots:
    virtual void resolve(const QString &url);
};

#endif
