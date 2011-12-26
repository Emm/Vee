#ifndef ERROR_VIEW_H
#define ERROR_VIEW_H

#include "local_view.h"

class ErrorView : public LocalView {
    Q_OBJECT

private:
    const static QString msInterface;
    const static QString msTitle;
    const static QString msUrl;

    QString mErrorMessage;

public:
    explicit ErrorView(const QString errorMessage, QObject* parent = 0);
    virtual ~ErrorView();

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
