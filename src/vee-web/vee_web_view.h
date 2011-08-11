#ifndef VEE_WEB_VIEW_H
#define VEE_WEB_VIEW_H

#include <QWebView>

class VeeWebView : public QWebView {

public:
    explicit VeeWebView(QWidget* parent=0);
    void loadUrlOrPath(const QString &value);
};

#endif
