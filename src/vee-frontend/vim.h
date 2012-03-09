#ifndef VIM_H
#define VIM_H

#include <QObject>

class Vim : public QObject {

Q_OBJECT


public:

    explicit Vim(QObject* parent=0);

    virtual ~Vim();

public slots:
    
    bool parse(QString command);

signals:

    void openCommand(QString url);

    void openInNewTabCommand(QString url);

    void closeTabCommand();

    void prefixMissing(QString command);

    void parsingFailed(QString command);
};

#endif
