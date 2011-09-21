#ifndef VEE_WEB_VIEW_H
#define VEE_WEB_VIEW_H

#include <QWebView>

class VeeWebView : public QWebView {

    Q_OBJECT

public:
    explicit VeeWebView(QWidget* parent=0);

private slots:
    void broadcastLoadFinished(bool ok);

public slots:
    void resolve(const QString &value);

signals:
    void urlResolved();
    void urlNotResolved();
};

#endif
