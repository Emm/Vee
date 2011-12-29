#ifndef INPUT_BAR_H
#define INPUT_BAR_H

#include <QLineEdit>
#include <QKeyEvent>

class InputBar : public QLineEdit {

Q_OBJECT

public:

    InputBar(QWidget* parent = 0);
    InputBar(const QString & contents, QWidget* parent = 0);

    virtual ~InputBar();

protected:

    virtual void keyPressEvent(QKeyEvent* event);
};

#endif
