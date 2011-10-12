#include "trafikantenAPI.h"
#include "utmref.h"
#include <QUrl>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QtScript>
#include <QDebug>

#define MAX_RECENT_COUNT 9

QScriptEngine *TrafikantenAPI::m_engine;
QNetworkAccessManager TrafikantenAPI::m_manager;
QHash<int, Stop*> Stop::m_cachedStops;

Stop::Stop(int id, const QString &n, const QString &d, int x, int y)
    : QObject(0)
    , m_stopID(id)
    , m_name(n)
    , m_district(d)
    , m_x(x)
    , m_y(y)
    , m_ready(false)
{
    m_hasBus = m_hasBoat = m_hasTrain = m_hasTram = m_hasMetro = false;

    m_cachedStops.insert(id, this);
}

Stop *Stop::getCachedStop(int id)
{
    return m_cachedStops.value(id);
}

void Stop::processGetLines()
{
    if (!sender())
        return;

    QNetworkReply *reply = qobject_cast<QNetworkReply *>(sender());
    QString data = QString::fromUtf8(reply->readAll());
    QScriptEngine *engine = TrafikantenAPI::getScriptEngine();
    engine->pushContext()->activationObject().setProperty("data", QScriptValue(data));
    QScriptValue result = engine->evaluate("JSON.parse(data)");
    engine->popContext();

    m_lines.clear();
    QVariantList list = result.toVariant().toList();
    int c =  list.count();
    for (int i = 0; i < c; ++i) {
        QVariantMap vm = list.at(i).toMap();
        QString name = vm.value("LineName").toString().trimmed();
        int transportation = vm.value("Transportation").toInt();
        m_lines.insert(name, transportation);
        switch (transportation) {
        case 1:
        case 2: {
            m_hasBus = true;
            break;
        }
        case 4:
        case 6: {
            m_hasTrain = true;
            break;
        }
        case 5: {
            m_hasBoat = true;
            break;
        }
        case 7: {
            m_hasTram = true;
            break;
        }
        case 8: {
            m_hasMetro = true;
            break;
        }
        }
    }

    reply->deleteLater();
    emit linesChanged();

    m_ready = true;
    emit readyChanged();

    m_cachedStops.insert(m_stopID, this);
}

TrafikantenAPI::TrafikantenAPI(const QString &lang)
    : QObject()
    , m_language(lang)
    , m_stopCount(0)
    , m_waitingForData(false)
    , m_reply(0)
{
    m_engine = new QScriptEngine;

    restoreRecentSearches();
    restoreLastTab();
}

QScriptEngine *TrafikantenAPI::getScriptEngine()
{
    return m_engine;
}

QNetworkAccessManager *TrafikantenAPI::getNetworkManager()
{
    return &m_manager;
}

QNetworkReply *TrafikantenAPI::createRequest(const QString &req)
{
    m_waitingForData = true;
    emit waitingForDataChanged();

    delete m_reply;
    m_reply = 0;
    QNetworkRequest request;
    request.setUrl(QUrl(serviceURL() + req));
    QNetworkReply *reply = m_manager.get(request);
    connect(reply, SIGNAL(finished()), this, SLOT(dataReceived()));
    return reply;
}

void TrafikantenAPI::dataReceived()
{
    m_waitingForData = false;
    emit waitingForDataChanged();
}

void TrafikantenAPI::setRealTimeSearch(const QString &s)
{
    if (m_realTimeSearch == s)
        return;

    m_realTimeSearch = s;

    if (s.isEmpty()) {
        delete m_reply;
        m_reply = 0;
        m_realTimeSeachResult.clear();
        emit realTimeSearchResultChanged();
        m_waitingForData = false;
        emit waitingForDataChanged();
    } else {
        m_reply = createRequest("/RealTime/FindMatches/" + s.toLatin1());
        connect(m_reply, SIGNAL(finished()), this, SLOT(processRealTimeSearchResult()));
    }

    emit realTimeSearchChanged();
}

void TrafikantenAPI::processRealTimeSearchResult()
{
    QString data = QString::fromUtf8(m_reply->readAll());
    m_engine->currentContext()->activationObject().setProperty("data", QScriptValue(data));
    QScriptValue result = m_engine->evaluate("JSON.parse(data)");

    m_stopCount = 0;
    m_realTimeSeachResult.clear();
    QVariantList list = result.toVariant().toList();
    int c =  list.count();
    for (int i = 0; i < c; ++i) {
        QVariantMap vm = list.at(i).toMap();
        int id = vm.value("ID").toInt();
        QString name = vm.value("Name").toString().trimmed();
        QString district = vm.value("District").toString().trimmed();
        int x = vm.value("X").toInt();
        int y = vm.value("Y").toInt();

        Stop *stop = Stop::getCachedStop(id);
        if (!stop) {
            ++m_stopCount;
            stop = new Stop(id, name, district, x, y);

            QNetworkReply *reply = m_manager.get(QNetworkRequest(QUrl(serviceURL() + QString("/Place/GetLines/%1").arg(id))));
            connect(reply, SIGNAL(finished()), stop, SLOT(processGetLines()));
            connect(reply, SIGNAL(finished()), this, SLOT(onStopGotLineInfo()));
        }
        m_realTimeSeachResult.append(stop);
    }

    if (!m_stopCount) {
        emit realTimeSearchResultChanged();
    } else {
        m_waitingForData = true;
        emit waitingForDataChanged();
    }
}

void TrafikantenAPI::onStopGotLineInfo()
{
    if (--m_stopCount < 1) {
        m_waitingForData = false;
        emit waitingForDataChanged();
        emit realTimeSearchResultChanged();
        emit nearbySearchResultChanged();
    }
}

void TrafikantenAPI::setNearbySearch(const QPointF &loc)
{
    m_nearbySearch = loc;

    if (loc.isNull()) {
        delete m_reply;
        m_reply = 0;
        m_nearbySearchResult.clear();
        emit nearbySearchResultChanged();
        m_waitingForData = false;
        emit waitingForDataChanged();
    } else {
        UTMRef utm = latLongToUtm(loc);
        QPoint toInt = utm.position.toPoint();
        m_reply = createRequest(QString("/Place/GetClosestStopsAdvancedByCoordinates/?coordinates=(X=%1,Y=%2)&proposals=10&walkingDistance=800").arg(toInt.x()).arg(toInt.y()));
        connect(m_reply, SIGNAL(finished()), this, SLOT(processNearbySearchResult()));
    }

    emit nearbySearchChanged();
}

void TrafikantenAPI::processNearbySearchResult()
{
    QString data = QString::fromUtf8(m_reply->readAll());
    m_engine->currentContext()->activationObject().setProperty("data", QScriptValue(data));
    QScriptValue result = m_engine->evaluate("JSON.parse(data)");

    m_stopCount = 0;
    m_nearbySearchResult.clear();
    QVariantList list = result.toVariant().toList();
    int c =  list.count();
    for (int i = 0; i < c; ++i) {
        QVariantMap vm = list.at(i).toMap();
        int id = vm.value("ID").toInt();
        QString name = vm.value("Name").toString().trimmed();
        QString district = vm.value("District").toString().trimmed();
        int x = vm.value("X").toInt();
        int y = vm.value("Y").toInt();

        Stop *stop = Stop::getCachedStop(id);
        if (!stop) {
            ++m_stopCount;
            stop = new Stop(id, name, district, x, y);

            QNetworkReply *reply = m_manager.get(QNetworkRequest(QUrl(serviceURL() + QString("/Place/GetLines/%1").arg(id))));
            connect(reply, SIGNAL(finished()), stop, SLOT(processGetLines()));
            connect(reply, SIGNAL(finished()), this, SLOT(onStopGotLineInfo()));
        }
        m_nearbySearchResult.append(stop);
    }

    if (!m_stopCount) {
        emit nearbySearchResultChanged();
    } else {
        m_waitingForData = true;
        emit waitingForDataChanged();
    }
}

void TrafikantenAPI::setRealTimeDataQuery(int id)
{
    if (m_realTimeDataQuery == id)
        return;

    m_realTimeDataQuery = id;

    if (id == 0) {
        delete m_reply;
        m_reply = 0;
        m_realTimeDataResult.clear();
        emit realTimeDataResultChanged();
        m_waitingForData = false;
        emit waitingForDataChanged();
    } else {
        m_reply = createRequest(QString("/RealTime/GetRealTimeData/%1").arg(id));
        connect(m_reply, SIGNAL(finished()), this, SLOT(processRealTimeDataResult()));
    }

    emit realTimeDataQueryChanged();

    updateRecentSearches();
}

void TrafikantenAPI::processRealTimeDataResult()
{
    QString data = QString::fromUtf8(m_reply->readAll());
    m_engine->currentContext()->activationObject().setProperty("data", QScriptValue(data));
    QScriptValue result = m_engine->evaluate("JSON.parse(data)");

    qDeleteAll(m_realTimeDataResult);
    m_realTimeDataResult.clear();
    QVariantList list = result.toVariant().toList();
    int c =  list.count();
    for (int i = 0; i < c; ++i) {
        QVariantMap vm = list.at(i).toMap();
        QString name = vm.value("PublishedLineName").toString().trimmed();
        QString dest = vm.value("DestinationName").toString().trimmed();
        QString platform = vm.value("DeparturePlatformName").toString().trimmed();
        bool mon = vm.value("Monitored").toBool();
        QString refTime = vm.value("RecordedAtTime").toString();
        refTime = refTime.mid(6, refTime.indexOf("+") - 6);
        QString time = vm.value("ExpectedDepartureTime").toString();
        time = time.mid(6, time.indexOf("+") - 6);

        RealTimeData *data = dynamic_cast<RealTimeData *>(m_realTimeDataResult.value(name + dest + platform));
        if (!data) {
            Stop *stop = Stop::getCachedStop(m_realTimeDataQuery);
            data = new RealTimeData(m_realTimeDataQuery, "", name, dest, platform, stop->lineType(name));
            m_realTimeDataResult.insert(name + dest + platform, data);
        }
        data->addDepartureTime(new RealTimeDepartureData(mon, refTime, time));
    }

    emit realTimeDataResultChanged();
}

static bool realTimeDataLessThan(QObject *a, QObject *b)
{
    QString platformA = a->property("platformName").toString();
    QString platformB = b->property("platformName").toString();
    if (platformA != platformB)
    {
        bool okA = false;
        int intA = platformA.toInt(&okA);
        bool okB = false;
        int intB = platformB.toInt(&okB);
        if (okA && okB)
            return intA < intB;
        else
            return QString::localeAwareCompare(platformA, platformB) < 0;
    }

    RealTimeData *dA = dynamic_cast<RealTimeData *>(a);
    RealTimeData *dB = dynamic_cast<RealTimeData *>(b);
    QString timeA = dA->departureTimes().at(0)->property("departureTime").toString();
    QString timeB = dB->departureTimes().at(0)->property("departureTime").toString();
    if (timeA != timeB)
    {
        bool okA = false;
        int intA = timeA.toInt(&okA);
        bool okB = false;
        int intB = timeB.toInt(&okB);
        if (okA && okB)
            return intA < intB;
        else
            return QString::localeAwareCompare(timeA, timeB) < 0;
    }

    return QString::localeAwareCompare(dA->lineName(), dB->lineName()) < 0;
}

QList<QObject *> TrafikantenAPI::realTimeDataResult() const
{
    QList<QObject *> list = m_realTimeDataResult.values();
    qSort(list.begin(), list.end(), realTimeDataLessThan);
    return list;
}

void TrafikantenAPI::updateRecentSearches()
{
    if (!Stop::isStopCached(m_realTimeDataQuery))
        return;

    Stop *stop = Stop::getCachedStop(m_realTimeDataQuery);

    int stopIndex = -1;
    int c = m_recentSearches.count();
    for (int i = 0; i < c; ++i) {
        Stop *s = dynamic_cast<Stop *>(m_recentSearches.at(i));
        if (s->stopID() == stop->stopID()) {
            stopIndex = i;
            break;
        }
    }

    if (stopIndex != -1) {
        m_recentSearches.move(stopIndex, 0);
    } else {
        Stop *newStop = new Stop(0, "", "", 0, 0);
        *newStop = *stop;
        m_recentSearches.prepend(newStop);
    }

    if (m_recentSearches.count() > MAX_RECENT_COUNT)
        delete m_recentSearches.takeLast();

    saveRecentSearches();

    emit recentSearchesChanged();
}

void TrafikantenAPI::saveRecentSearches()
{
    QSettings settings;
    settings.remove("recents");
    settings.beginWriteArray("recents");
    int c = m_recentSearches.count();
    for (int i = 0; i < c; ++i) {
        Stop *stop = dynamic_cast<Stop *>(m_recentSearches.at(i));
        settings.setArrayIndex(i);
        settings.setValue("stopID", stop->stopID());
        settings.setValue("name", stop->name());
        settings.setValue("district", stop->district());
        settings.setValue("x", stop->x());
        settings.setValue("y", stop->y());
        settings.setValue("hasBus", stop->m_hasBus);
        settings.setValue("hasBoat", stop->m_hasBoat);
        settings.setValue("hasTram", stop->m_hasTram);
        settings.setValue("hasTrain", stop->m_hasTrain);
        settings.setValue("hasMetro", stop->m_hasMetro);
        settings.beginWriteArray("lines");
        QHash<QString, int>::const_iterator it;
        int j = 0;
        for (it = stop->m_lines.begin(); it != stop->m_lines.end(); ++it) {
            settings.setArrayIndex(j);
            settings.setValue("lineName", it.key());
            settings.setValue("transportation", it.value());
            ++j;
        }
        settings.endArray();
    }
    settings.endArray();
}

void TrafikantenAPI::restoreRecentSearches()
{
    m_recentSearches.clear();

    QSettings settings;
    int size = settings.beginReadArray("recents");
    for (int i = 0; i < size; ++i) {
        settings.setArrayIndex(i);
        int id = settings.value("stopID").toInt();
        QString name = settings.value("name").toString();
        QString district = settings.value("district").toString();
        int x = settings.value("x").toInt();
        int y = settings.value("y").toInt();
        Stop *stop = new Stop(id, name, district, x, y);
        stop->m_hasBus = settings.value("hasBus").toBool();
        stop->m_hasBoat = settings.value("hasBoat").toBool();
        stop->m_hasTram = settings.value("hasTram").toBool();
        stop->m_hasTrain = settings.value("hasTrain").toBool();
        stop->m_hasMetro = settings.value("hasMetro").toBool();
        stop->m_ready = true;
        int nbLines = settings.beginReadArray("lines");
        for (int j = 0; j < nbLines; j++) {
            settings.setArrayIndex(j);
            QString lineName = settings.value("lineName").toString();
            int transportation = settings.value("transportation").toInt();
            stop->m_lines.insert(lineName, transportation);
        }
        settings.endArray();

        m_recentSearches.append(stop);
    }
    settings.endArray();

    emit recentSearchesChanged();
}

void TrafikantenAPI::setLastTab(const QString &tab)
{
    if (m_lastTab == tab)
        return;

    m_lastTab = tab;

    saveLastTab();

    emit lastTabChanged();
}

void TrafikantenAPI::saveLastTab()
{
    QSettings settings;
    settings.setValue("lastTab", m_lastTab);
}

void TrafikantenAPI::restoreLastTab()
{
    QSettings settings;
    m_lastTab = settings.value("lastTab", QString("search")).toString();
    emit lastTabChanged();
}
