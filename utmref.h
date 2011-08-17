#ifndef UTMREF_H
#define UTMREF_H

#include <QPointF>

struct UTMRef
{
    QPointF position;
    char latZ;
    int lonZ;
};

QPointF utmToLatLong(const QPointF &utm, char latZ, int lonZ);
UTMRef latLongToUtm(const QPointF &latlong);

#endif // UTMREF_H
