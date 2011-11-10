#ifndef VIEW_BULDER_H
#define VIEW_BULDER_H

#include "view.h"

class ViewBuilder : public QObject {

Q_OBJECT

public:

    explicit ViewBuilder(QObject* parent=0);

    virtual ~ViewBuilder();

    virtual void build(const ulong identifier);

    const virtual QString & viewType() const = 0;

private:

    View* mView;

    void cleanup();

protected:

    virtual View* buildView() = 0;


signals:

    void viewBuilt(View* view);

    void error(int errorType, int errorCode);


private slots:

    void viewGotAnError(int errorCode, int errorType);

    void viewInitialized();
};

#endif
