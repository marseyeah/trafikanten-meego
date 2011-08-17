#include "favoriteslist.h"
#include "trafikantenAPI.h"

#include <QNetworkRequest>
#include <QNetworkReply>
#include <QDebug>
#include <QSettings>

static bool favoriteLessThan(const RealTimeData *f1, const RealTimeData *f2)
{
    int c1 = QString::compare(f1->stopName(), f2->stopName(), Qt::CaseInsensitive);
    if (c1 != 0)
        return c1 < 0;

    int c2 = QString::compare(f1->lineName(), f2->lineName(), Qt::CaseInsensitive);
    if (c2 != 0)
        return c2 < 0;

    int c3 = QString::compare(f1->destination(), f2->destination(), Qt::CaseInsensitive);
    return c3 < 0;
}

FavoritesList::FavoritesList(QObject *parent) :
    QObject(parent)
  , m_pendingRequests(0)
{
    load();
}

FavoritesList::~FavoritesList()
{
    qDeleteAll(m_data);
}

void FavoritesList::addFavorite(int stopID, const QString &stopName, const QString &lineName, const QString &destination, int transportation)
{
    if (isFavorite(stopID, lineName, destination))
        return;
    RealTimeData *d = new RealTimeData(stopID, stopName, lineName, destination, QLatin1String(""), transportation);
    m_data.append(d);
    m_stops.insertMulti(stopID, 0);
    qSort(m_data.begin(), m_data.end(), favoriteLessThan);
    emit dataChanged();
    save();
}

void FavoritesList::removeFavorite(int stopID, const QString &lineName, const QString &destination)
{
    int count = m_data.count();
    for (int i = 0; i < count; ++i) {
        RealTimeData *d = m_data.at(i);
        if (d->stopId() == stopID && d->lineName() == lineName && d->destination() == destination) {
            m_data.removeAt(i);
            m_stops.take(stopID);
            emit dataChanged();
            save();
            return;
        }
    }
}

bool FavoritesList::isFavorite(int stopID, const QString &line, const QString &destination)
{
    return getFavorite(stopID, line, destination) != 0;
}

RealTimeData *FavoritesList::getFavorite(int stopID, const QString &line, const QString &destination)
{
    int count = m_data.count();
    for (int i = 0; i < count; ++i) {
        RealTimeData *d = m_data.at(i);
        if (d->stopId() == stopID && d->lineName() == line && d->destination() == destination)
            return d;
    }
    return 0;
}

void FavoritesList::refreshData()
{
    // Clear old data
    foreach (RealTimeData *d, m_data)
        d->clearDepartureTimes();    

    // Request new data
    QList<int> ids = m_stops.uniqueKeys();
    int count = ids.count();
    m_pendingRequests = count;
    emit waitingForDataChanged();
    for (int i = 0; i < count; ++i) {
        QNetworkRequest request;
        QString req = QString("/RealTime/GetRealTimeData/%1").arg(ids.at(i));
        request.setUrl(QUrl(TrafikantenAPI::serviceURL() + req));
        QNetworkReply *reply = TrafikantenAPI::getNetworkManager()->get(request);
        connect(reply, SIGNAL(finished()), this, SLOT(processReply()));
    }
}

void FavoritesList::processReply()
{
    if (!sender())
        return;

    QNetworkReply *reply = qobject_cast<QNetworkReply *>(sender());
    QString data = QString::fromUtf8(reply->readAll());
    QScriptEngine *engine = TrafikantenAPI::getScriptEngine();
    engine->pushContext()->activationObject().setProperty("data", QScriptValue(data));
    QScriptValue result = engine->evaluate("JSON.parse(data)");
    engine->popContext();

    QString url = reply->request().url().path();
    int id = url.mid(url.lastIndexOf("/") + 1).toInt();

    QVariantList list = result.toVariant().toList();
    int c =  list.count();
    for (int i = 0; i < c; ++i) {
        QVariantMap vm = list.at(i).toMap();
        QString name = vm.value("PublishedLineName").toString().trimmed();
        QString dest = vm.value("DestinationName").toString().trimmed();
        bool mon = vm.value("Monitored").toBool();
        QString refTime = vm.value("RecordedAtTime").toString();
        refTime = refTime.mid(6, refTime.indexOf("+") - 6);
        QString time = vm.value("ExpectedDepartureTime").toString();
        time = time.mid(6, time.indexOf("+") - 6);
        RealTimeData *data = getFavorite(id, name, dest);
        if (data)
            data->addDepartureTime(new RealTimeDepartureData(mon, refTime, time));
    }

    if (--m_pendingRequests < 1)
        emit waitingForDataChanged();

    reply->deleteLater();
}

void FavoritesList::save() const
{
    QSettings settings;
    settings.remove("favorites");
    settings.beginWriteArray("favorites");
    int c = m_data.count();
    for (int i = 0; i < c; ++i) {
        RealTimeData *d = m_data.at(i);
        settings.setArrayIndex(i);
        settings.setValue("stopID", d->stopId());
        settings.setValue("stopName", d->stopName());
        settings.setValue("lineName", d->lineName());
        settings.setValue("destination", d->destination());
        settings.setValue("transportation", d->transportation());
    }
    settings.endArray();
}

void FavoritesList::load()
{
    m_data.clear();

    QSettings settings;
    int size = settings.beginReadArray("favorites");
    for (int i = 0; i < size; ++i) {
        settings.setArrayIndex(i);

        int stopId = settings.value("stopID").toInt();
        QString stopName = settings.value("stopName").toString();
        QString lineName = settings.value("lineName").toString();
        QString destination = settings.value("destination").toString();
        int transportation = settings.value("transportation").toInt();

        RealTimeData *d = new RealTimeData(stopId, stopName, lineName, destination, QLatin1String(""), transportation);
        m_data.append(d);
        m_stops.insertMulti(stopId, 0);
    }
    settings.endArray();
    qSort(m_data.begin(), m_data.end(), favoriteLessThan);
    emit dataChanged();
}
