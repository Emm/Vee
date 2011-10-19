#ifndef BLANK_VIEW_H
#define BLANK_VIEW_H

#include <QWidget>
#include "local_view.h"

class BlankView : public LocalView {
    Q_OBJECT

private:
    const static QString msInterface;
    const static QString msTitle;
    const static QString msUrl;

public:
    explicit BlankView(QObject* parent = 0);
    virtual ~BlankView();

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
