#include "vee_web_service_builder.h"

#include <QTextStream>

VeeWebServiceBuilder::VeeWebServiceBuilder(const QString & urlOrFile, const ulong windowId, const DBusManager* dbusManager) : mUrlOrFile(urlOrFile), mWindowId(windowId), mDBusManager(dbusManager)  {
}

VeeWebServiceBuilder::~VeeWebServiceBuilder() {
}

VeeWebService* VeeWebServiceBuilder::build() {
    VeeWebService* view = buildView();
    return view;
}

VeeWebService* VeeWebServiceBuilder::buildView() {
    VeeWebService* view = new VeeWebService(mWindowId);
    if (shouldEmbed())
        mDBusManager->registerWidget(*view);
    if (mUrlOrFile.compare("-") == 0) {
        const QString & html = readHtmlFromStdin();
        view->setHtml(html);
    }
    else if (!mUrlOrFile.isEmpty()) {
        view->resolve(mUrlOrFile);
    }
    return view;
}

/**
 Builds HTML from the contents of stdin
*/
const QString & VeeWebServiceBuilder::readHtmlFromStdin() const {
    QString* html = new QString();
    QTextStream stdinStream(stdin);
    QString line;
    do {
        line = stdinStream.readLine();
        html->append(line);
    }
    while (!line.isNull());
    return *html;
}

bool VeeWebServiceBuilder::shouldEmbed() const {
    return mWindowId != 0 && mDBusManager != NULL;
}
