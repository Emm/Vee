#ifndef VIM_H
#define VIM_H

#include <QObject>

class Vim : public QObject {

Q_OBJECT


public:

    enum Mode { NormalMode, CommandMode };

    explicit Vim(Mode mode=NormalMode, QObject* parent=0);

    virtual ~Vim();

    void setMode(Mode mode);

    Mode mode() const;

private:

    Mode mMode; 

public slots:
    
    bool parse(const QString & command);

signals:

    void openCommand(QString url);

    void openInNewTabCommand(QString url);
};

#endif
