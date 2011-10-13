#include "trafikantenAPI.h"
#include <QtGui/QApplication>
#include <QtDeclarative>
#include <QtCore/QTranslator>
#include <QtCore/QtGlobal>
#include <MDeclarativeCache>

Q_DECL_EXPORT int main(int argc, char *argv[])
{
    QApplication::setOrganizationName("TrafikantenAS");
    QApplication::setOrganizationDomain("trafikanten.no");
    QApplication::setApplicationName("Trafikanten");

    QApplication *app = MDeclarativeCache::qApplication(argc, argv);
    QDeclarativeView *view = MDeclarativeCache::qDeclarativeView();

    QTranslator appTranslator;
    QString language = QLocale::system().name().left(2);
    bool res = appTranslator.load(QLatin1String(":/translations/trafikanten-") + language);
    if (res)
        app->installTranslator(&appTranslator);
    else
        language = QLatin1String("en");

    TrafikantenAPI api(language);
    view->rootContext()->setContextProperty("trafikanten", &api);

    view->setSource(QUrl("qrc:/qml/main.qml"));
    view->showFullScreen();
    return app->exec();
}
