#ifndef TRAFIKANTENAPI_H
#define TRAFIKANTENAPI_H

#include <QObject>
#include <QtNetwork/QNetworkAccessManager>
#include <QtScript/QScriptEngine>
#include <QPointF>
#include "favoriteslist.h"

class Stop : public QObject
{
    Q_OBJECT

    Q_PROPERTY(int stopID READ stopID NOTIFY stopIDChanged)
    Q_PROPERTY(QString name READ name NOTIFY nameChanged)
    Q_PROPERTY(QString district READ district NOTIFY districtChanged)
    Q_PROPERTY(int x READ x NOTIFY xChanged)
    Q_PROPERTY(int y READ y NOTIFY yChanged)
    Q_PROPERTY(bool hasBus READ hasBus NOTIFY linesChanged)
    Q_PROPERTY(bool hasBoat READ hasBoat NOTIFY linesChanged)
    Q_PROPERTY(bool hasTrain READ hasTrain NOTIFY linesChanged)
    Q_PROPERTY(bool hasTram READ hasTram NOTIFY linesChanged)
    Q_PROPERTY(bool hasMetro READ hasMetro NOTIFY linesChanged)
    Q_PROPERTY(bool ready READ ready NOTIFY readyChanged)
public:
    Stop(int id = 0, const QString &n = QString(), const QString &d = QString(), int x = 0, int y = 0);

    int stopID() const { return m_stopID; }
    QString name() const { return m_name; }
    QString district() const { return m_district; }
    int x() const { return m_x; }
    int y() const { return m_y; }
    bool hasBus() const { return m_hasBus; }
    bool hasBoat() const { return m_hasBoat; }
    bool hasTrain() const { return m_hasTrain; }
    bool hasTram() const { return m_hasTram; }
    bool hasMetro() const { return m_hasMetro; }
    bool ready() const { return m_ready; }

    int lineType(const QString &lineName) const
    {
        return m_lines.value(lineName, 0);
    }

    static Stop *getCachedStop(int id);
    static bool isStopCached(int id) { return m_cachedStops.contains(id); }

    Stop &operator=(const Stop &other)
    {
        if (this != &other) {
            m_stopID = other.stopID();
            m_name = other.name();
            m_district = other.district();
            m_x = other.x();
            m_y = other.y();
            m_lines = other.m_lines;
            m_hasBus = other.m_hasBus;
            m_hasBoat = other.m_hasBoat;
            m_hasTrain = other.m_hasTrain;
            m_hasTram = other.m_hasTram;
            m_hasMetro = other.m_hasMetro;
            m_ready = true;
            m_cachedStops.insert(m_stopID, this);
        }
        return *this;
    }

protected Q_SLOTS:
    void processGetLines();

Q_SIGNALS:
    void stopIDChanged();
    void nameChanged();
    void districtChanged();
    void xChanged();
    void yChanged();
    void linesChanged();
    void readyChanged();

private:
    friend class TrafikantenAPI;

    int m_stopID;
    QString m_name;
    QString m_district;
    int m_x;
    int m_y;
    QHash<QString, int> m_lines;
    bool m_hasBus;
    bool m_hasBoat;
    bool m_hasTrain;
    bool m_hasTram;
    bool m_hasMetro;
    bool m_ready;

    static QHash<int, Stop*> m_cachedStops;
};

class RealTimeDepartureData : public QObject
{
    Q_OBJECT
    Q_PROPERTY(bool monitored READ monitored NOTIFY monitoredChanged)
    Q_PROPERTY(QString referenceTime READ referenceTime NOTIFY referenceTimeChanged)
    Q_PROPERTY(QString departureTime READ departureTime NOTIFY departureTimeChanged)
public:
    RealTimeDepartureData(bool m, const QString &ref, const QString &dt)
        : QObject()
        , m_referenceTime(ref)
        , m_departureTime(dt)
        , m_monitored(m)
    {
    }

    QString referenceTime() const { return m_referenceTime; }
    QString departureTime() const { return m_departureTime; }
    bool monitored() const { return m_monitored; }

Q_SIGNALS:
    void monitoredChanged();
    void referenceTimeChanged();
    void departureTimeChanged();

private:
    QString m_referenceTime;
    QString m_departureTime;
    bool m_monitored;
};

class RealTimeData : public QObject
{
    Q_OBJECT

    Q_PROPERTY(int stopId READ stopId NOTIFY lineNameChanged)
    Q_PROPERTY(QString stopName READ stopName NOTIFY lineNameChanged)
    Q_PROPERTY(QString lineName READ lineName NOTIFY lineNameChanged)
    Q_PROPERTY(QString destination READ destination NOTIFY destinationChanged)
    Q_PROPERTY(QString platformName READ platformName NOTIFY platformNameChanged)
    Q_PROPERTY(int transportation READ transportation NOTIFY transportationChanged)
    Q_PROPERTY(QList<QObject *> departureTimes READ departureTimes NOTIFY departureTimesChanged)

public:
    RealTimeData(int stopId, const QString &stopName, const QString &ln, const QString &d, const QString &pn, int t)
        : QObject()
        , m_stopId(stopId)
        , m_stopName(stopName)
        , m_lineName(ln)
        , m_destination(d)
        , m_platformName(pn)
        , m_transportation(t)
    {
    }

    ~RealTimeData()
    {
        qDeleteAll(m_times);
    }

    int stopId() const { return m_stopId; }
    QString stopName() const { return m_stopName; }
    QString lineName() const { return m_lineName; }
    QString destination() const { return m_destination; }
    QString platformName() const { return m_platformName; }
    int transportation() const { return m_transportation; }
    QList<QObject *> departureTimes() const { return m_times; }

    void addDepartureTime(RealTimeDepartureData *d)
    {
        m_times.append(d);
        emit departureTimesChanged();
    }

    void clearDepartureTimes()
    {
        m_times.clear();
        emit departureTimesChanged();;
    }

Q_SIGNALS:
    void lineNameChanged();
    void destinationChanged();
    void platformNameChanged();
    void transportationChanged();
    void departureTimesChanged();

private:
    int m_stopId;
    QString m_stopName;
    QString m_lineName;
    QString m_destination;
    QString m_platformName;
    int m_transportation;
    QList<QObject *> m_times;
};

class TrafikantenAPI : public QObject
{
    Q_OBJECT

    Q_PROPERTY(QString language READ language NOTIFY languageChanged)
    Q_PROPERTY(QString realTimeSearch READ realTimeSearch WRITE setRealTimeSearch NOTIFY realTimeSearchChanged)
    Q_PROPERTY(QList<QObject *> realTimeSearchResult READ realTimeSearchResult NOTIFY realTimeSearchResultChanged)
    Q_PROPERTY(int realTimeDataQuery READ realTimeDataQuery WRITE setRealTimeDataQuery NOTIFY realTimeDataQueryChanged)
    Q_PROPERTY(QList<QObject *> realTimeDataResult READ realTimeDataResult NOTIFY realTimeDataResultChanged)
    Q_PROPERTY(bool waitingForData READ waitingForData NOTIFY waitingForDataChanged)

    Q_PROPERTY(QList<QObject *> recentSearches READ recentSearches NOTIFY recentSearchesChanged)

    Q_PROPERTY(QPointF nearbySearch READ nearbySearch WRITE setNearbySearch NOTIFY nearbySearchChanged)
    Q_PROPERTY(QList<QObject *> nearbySearchResult READ nearbySearchResult NOTIFY nearbySearchResultChanged)

    Q_PROPERTY(QObject *favorites READ favorites NOTIFY favoritesChanged)

public:
    TrafikantenAPI(const QString &lang);

    QString language() const { return m_language; }

    QString realTimeSearch() const { return m_realTimeSearch; }
    void setRealTimeSearch(const QString &s);

    int realTimeDataQuery() const { return m_realTimeDataQuery; }
    void setRealTimeDataQuery(int id);

    QList<QObject *> realTimeSearchResult() const { return m_realTimeSeachResult; }
    QList<QObject *> realTimeDataResult() const;

    bool waitingForData() const { return m_waitingForData; }

    QList<QObject *> recentSearches() const { return m_recentSearches; }

    QPointF nearbySearch() const { return m_nearbySearch; }
    void setNearbySearch(const QPointF &loc);
    QList<QObject *> nearbySearchResult() const { return m_nearbySearchResult; }

    QObject *favorites() { return &m_favorites; }

    static QString serviceURL() { return QString("http://api-test.trafikanten.no"); }

    static QScriptEngine *getScriptEngine();
    static QNetworkAccessManager *getNetworkManager();

Q_SIGNALS:
    void languageChanged();
    void realTimeSearchChanged();
    void realTimeSearchResultChanged();
    void realTimeDataQueryChanged();
    void realTimeDataResultChanged();
    void waitingForDataChanged();
    void recentSearchesChanged();
    void nearbySearchChanged();
    void nearbySearchResultChanged();
    void favoritesChanged();

protected Q_SLOTS:
    void processRealTimeSearchResult();
    void onStopGotLineInfo();
    void processRealTimeDataResult();
    void processNearbySearchResult();
    void dataReceived();

private:
    QNetworkReply *createRequest(const QString &req);

    void updateRecentSearches();
    void saveRecentSearches();
    void restoreRecentSearches();

    QString m_language;

    QString m_realTimeSearch;
    QList<QObject *> m_realTimeSeachResult;
    int m_stopCount;

    int m_realTimeDataQuery;
    QHash<QString, QObject *> m_realTimeDataResult;

    bool m_waitingForData;

    QList<QObject *> m_recentSearches;

    QPointF m_nearbySearch;
    QList<QObject *> m_nearbySearchResult;

    FavoritesList m_favorites;

    static QNetworkAccessManager m_manager;
    QNetworkReply *m_reply;
    static QScriptEngine *m_engine;
};

#endif // TRAFIKANTENAPI_H
