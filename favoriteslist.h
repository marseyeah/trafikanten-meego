#ifndef FAVORITESLIST_H
#define FAVORITESLIST_H

#include <QObject>
#include <QMultiHash>

class RealTimeData;

class FavoritesList : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QList<QObject *> data READ data NOTIFY dataChanged)
    Q_PROPERTY(bool waitingForData READ waitingForData NOTIFY waitingForDataChanged)
public:
    explicit FavoritesList(QObject *parent = 0);
    ~FavoritesList();

    QList<QObject *> data() const {
        QList<QObject *> list;
        foreach (RealTimeData *d, m_data) {
            list.append((QObject *)d);
        }
        return list;
    }

    bool waitingForData() {
        return m_pendingRequests > 0;
    }

signals:
    void dataChanged();
    void waitingForDataChanged();

public Q_SLOTS:
    void addFavorite(int stopID, const QString &stopName, const QString &lineName, const QString &destination, int transportation);
    void removeFavorite(int stopID, const QString &lineName, const QString &destination);
    bool isFavorite(int stopID, const QString &line, const QString &destination);

    void refreshData();

private Q_SLOTS:
    void processReply();

private:
    RealTimeData *getFavorite(int stopID, const QString &line, const QString &destination);
    void save() const;
    void load();

    QList<RealTimeData *> m_data;
    QMultiHash<int, void *> m_stops;

    int m_pendingRequests;
};

#endif // FAVORITESLIST_H
