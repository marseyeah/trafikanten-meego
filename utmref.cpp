#include "utmref.h"
#include <qmath.h>

static char getUTMLatitudeZoneLetter(double latitude) {
    if ((84 >= latitude) && (latitude >= 72))
      return 'X';
    else if ((72 > latitude) && (latitude >= 64))
      return 'W';
    else if ((64 > latitude) && (latitude >= 56))
      return 'V';
    else if ((56 > latitude) && (latitude >= 48))
      return 'U';
    else if ((48 > latitude) && (latitude >= 40))
      return 'T';
    else if ((40 > latitude) && (latitude >= 32))
      return 'S';
    else if ((32 > latitude) && (latitude >= 24))
      return 'R';
    else if ((24 > latitude) && (latitude >= 16))
      return 'Q';
    else if ((16 > latitude) && (latitude >= 8))
      return 'P';
    else if ((8 > latitude) && (latitude >= 0))
      return 'N';
    else if ((0 > latitude) && (latitude >= -8))
      return 'M';
    else if ((-8 > latitude) && (latitude >= -16))
      return 'L';
    else if ((-16 > latitude) && (latitude >= -24))
      return 'K';
    else if ((-24 > latitude) && (latitude >= -32))
      return 'J';
    else if ((-32 > latitude) && (latitude >= -40))
      return 'H';
    else if ((-40 > latitude) && (latitude >= -48))
      return 'G';
    else if ((-48 > latitude) && (latitude >= -56))
      return 'F';
    else if ((-56 > latitude) && (latitude >= -64))
      return 'E';
    else if ((-64 > latitude) && (latitude >= -72))
      return 'D';
    else if ((-72 > latitude) && (latitude >= -80))
      return 'C';
    else
      return 'Z';
  }

QPointF utmToLatLong(const QPointF &utm, char latZ, int lonZ)
{
    qreal easting = utm.x();
    qreal northing = utm.y();
    qreal UTM_F0 = 0.9996;
    qreal a = 6378137.000;
    qreal eSquared = ((a * a) - (6356752.3141 * 6356752.3141)) / (a * a);
    qreal ePrimeSquared = eSquared / (1.0 - eSquared);
    qreal e1 = (1 - qSqrt(1 - eSquared)) / (1 + qSqrt(1 - eSquared));
    qreal x = easting - 500000.0;

    qreal y = northing;
    int zoneNumber = lonZ;
    char zoneLetter = latZ;

    qreal longitudeOrigin = (zoneNumber - 1.0) * 6.0 - 180.0 + 3.0;

    // Correct y for southern hemisphere
    if ((zoneLetter - 'N') < 0) {
        y -= 10000000.0;
    }

    qreal m = y / UTM_F0;
    qreal mu = m / (a * (1.0 - eSquared / 4.0 - 3.0 * eSquared * eSquared / 64.0 - 5.0 * qPow(eSquared, 3.0) / 256.0));

    qreal phi1Rad = mu + (3.0 * e1 / 2.0 - 27.0 * qPow(e1, 3.0) / 32.0)
            * qSin(2.0 * mu)
            + (21.0 * e1 * e1 / 16.0 - 55.0 * qPow(e1, 4.0) / 32.0)
            * qSin(4.0 * mu) + (151.0 * qPow(e1, 3.0) / 96.0)
            * qSin(6.0 * mu);

    qreal n = a / qSqrt(1.0 - eSquared * qSin(phi1Rad) * qSin(phi1Rad));
    qreal t = qTan(phi1Rad) * qTan(phi1Rad);
    qreal c = ePrimeSquared * qCos(phi1Rad) * qCos(phi1Rad);
    qreal r = a * (1.0 - eSquared) / qPow(1.0 - eSquared * qSin(phi1Rad) * qSin(phi1Rad), 1.5);
    qreal d = x / (n * UTM_F0);

    qreal latitude = (phi1Rad - (n * qTan(phi1Rad) / r)
                      * (d * d / 2.0
                         - (5.0 + (3.0 * t) + (10.0 * c) - (4.0 * c * c) - (9.0 * ePrimeSquared))
                         * qPow(d, 4.0) / 24.0 + (61.0 + (90.0 * t) + (298.0 * c)
                                                  + (45.0 * t * t) - (252.0 * ePrimeSquared) - (3.0 * c * c))
                         * qPow(d, 6.0) / 720.0))
            * (180.0 / M_PI);

    qreal longitude = longitudeOrigin
            + ((d - (1.0 + 2.0 * t + c) * qPow(d, 3.0) / 6.0 + (5.0
                                                                - (2.0 * c) + (28.0 * t) - (3.0 * c * c)
                                                                + (8.0 * ePrimeSquared) + (24.0 * t * t))
                * qPow(d, 5.0) / 120.0) / qCos(phi1Rad))
            * (180.0 / M_PI);

    return QPointF(latitude, longitude);
}

UTMRef latLongToUtm(const QPointF &latlong)
{
    UTMRef utm;

    double UTM_F0 = 0.9996;
    qreal a = 6378137.000;
    qreal eSquared = ((a * a) - (6356752.3141 * 6356752.3141)) / (a * a);
    double longitude = latlong.y();
    double latitude = latlong.x();

    double latitudeRad = latitude * (M_PI / 180.0);
    double longitudeRad = longitude * (M_PI / 180.0);
    int longitudeZone = (int) qFloor((longitude + 180.0) / 6.0) + 1;

    // Special zone for Norway
    if (latitude >= 56.0 && latitude < 64.0 && longitude >= 3.0
            && longitude < 12.0) {
        longitudeZone = 32;
    }

    // Special zones for Svalbard
    if (latitude >= 72.0 && latitude < 84.0) {
        if (longitude >= 0.0 && longitude < 9.0) {
            longitudeZone = 31;
        } else if (longitude >= 9.0 && longitude < 21.0) {
            longitudeZone = 33;
        } else if (longitude >= 21.0 && longitude < 33.0) {
            longitudeZone = 35;
        } else if (longitude >= 33.0 && longitude < 42.0) {
            longitudeZone = 37;
        }
    }

    double longitudeOrigin = (longitudeZone - 1) * 6 - 180 + 3;
    double longitudeOriginRad = longitudeOrigin * (M_PI / 180.0);

    char UTMZone = getUTMLatitudeZoneLetter(latitude);

    double ePrimeSquared = (eSquared) / (1 - eSquared);

    double n = a / qSqrt(1 - eSquared * qSin(latitudeRad) * qSin(latitudeRad));
    double t = qTan(latitudeRad) * qTan(latitudeRad);
    double c = ePrimeSquared * qCos(latitudeRad) * qCos(latitudeRad);
    double A = qCos(latitudeRad) * (longitudeRad - longitudeOriginRad);

    double M = a * ((1 - eSquared / 4 - 3 * eSquared * eSquared / 64 - 5 * eSquared * eSquared * eSquared / 256)
                    * latitudeRad
                    - (3 * eSquared / 8 + 3 * eSquared * eSquared / 32 + 45
                       * eSquared * eSquared * eSquared / 1024)
                    * qSin(2 * latitudeRad)
                    + (15 * eSquared * eSquared / 256 + 45 * eSquared * eSquared
                       * eSquared / 1024) * qSin(4 * latitudeRad) - (35
                                                                     * eSquared * eSquared * eSquared / 3072)
                    * qSin(6 * latitudeRad));

    double UTMEasting = (UTM_F0 * n * (A + (1 - t + c) * qPow(A, 3.0) / 6 + (5 - 18 * t + t * t
                                                                             + 72 * c - 58 * ePrimeSquared)
                                       * qPow(A, 5.0) / 120) + 500000.0);

    double UTMNorthing = (UTM_F0 * (M + n * qTan(latitudeRad)
                                    * (A * A / 2 + (5 - t + (9 * c) + (4 * c * c)) * qPow(A, 4.0)
                                       / 24 + (61 - (58 * t) + (t * t) + (600 * c) - (330 * ePrimeSquared))
                                       * qPow(A, 6.0) / 720)));

    // Adjust for the southern hemisphere
    if (latitude < 0) {
        UTMNorthing += 10000000.0;
    }

    utm.position = QPointF(UTMEasting, UTMNorthing);
    utm.latZ = UTMZone;
    utm.lonZ = longitudeZone;
    return utm;
}

