#ifndef VIEW_H
#define VIEW_H

#include <QWebView>

class View : public QWebView {

public:
    explicit View(QWidget* parent=0);
    void loadUrlOrPath(const QString &value);
};

#endif
