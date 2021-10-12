/***********************************************************************

  Copyright (C) 2021 Matthew Kendall

  This program is free software: you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation, either version 3 of the License, or
  (at your option) any later version.

  This program is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with this program.  If not, see <https://www.gnu.org/licenses/>.

  ***********************************************************************/

#include "predictor.h"
#include "AstroLib.h"

#include <HTTPClient.h>
#include <string.h>
#include <time.h>

Predictor::Predictor(uint32_t catalogNumber) :
    catalogNumber(catalogNumber) {
}

void Predictor::init(void) {

    HTTPClient http;
    String url;
    char tleTemp[2][82];
    int year, mon, day, hr, minute;
    double sec;
    struct tm t = {0};

    // Fetch the latest TLE from Celestrak
    url = String("http://celestrak.com/satcat/tle.php?CATNR=") + String(catalogNumber);
    http.begin(url);
    if (http.GET() == HTTP_CODE_OK) {
        WiFiClient response = http.getStream();
        satName = response.readStringUntil('\n');
        tle[0] = response.readStringUntil('\n');
        tle[1] = response.readStringUntil('\n');

        // Convert the string format TLE to SGP4 elements and initialise the satellite record.
        // twoline2rv needs non-const char arrays (it may modify them), so we make temporary copies.
        strncpy(tleTemp[0], tle[0].c_str(), 80);
        strncpy(tleTemp[1], tle[1].c_str(), 80);
        SGP4Funcs::twoline2rv(tleTemp[0], tleTemp[1], 'a', wgs72, satrec);

        // Convert the TLE epoch to a standard UNIX timestamp
        SGP4Funcs::invjday_SGP4(satrec.jdsatepoch, satrec.jdsatepochF, year, mon, day, hr, minute, sec);
        t.tm_year = year - 1900; t.tm_mon = mon - 1; t.tm_mday = day; t.tm_hour = hr; t.tm_min = minute; t.tm_sec = sec;
        epoch = mktime(&t);

        // Debug
        Serial.println(String("Predictor init complete:"));
        Serial.println(String("  Sat Name: ") + satName);
        Serial.println(String("  TLE[0]: ") + tle[0]);
        Serial.println(String("  TLE[1]: ") + tle[1]);
        Serial.println(String("  Epoch: ") + String(epoch));

    } else {
        Serial.println("Predictor init failed");
    }
    http.end();
}

void Predictor::posn(time_t t, double *pos) {

    double rteme[3], vteme[3], ateme[3] = {0.};
    double recef[3], vecef[3], aecef[3];

    // Calculate the time offset from the TLE epoch
    double tsince = ((double)t - (double)epoch) / 60.0;
    Serial.println(String("t: ") + String(t) + String(" tsince: ") + String(tsince));

    // Calculate the satelite position and velocity vectors in the True Equator,
    // Mean Equinox (TEME) earth-centered inertial (non-rotating) reference frame.
    SGP4Funcs::sgp4(satrec, tsince, rteme, vteme);
    Serial.println(String("TEME ") +
        String(" R: ") + String(rteme[0]) + String(" ") + String(rteme[1]) + String(" ") + String(rteme[2]) +
        String(" V: ") + String(vteme[0]) + String(" ") + String(vteme[1]) + String(" ") + String(vteme[2]));

    // Convert to the Earth-Centered, Earth-Fixed (ECEF) rotating reference frame.
    double jdut1 = satrec.jdsatepoch + satrec.jdsatepochF + tsince/1440.0;
    AstroLib::teme_ecef(rteme, vteme, ateme, MathTimeLib::eTo, recef, vecef, aecef,
        0.0 /* unused when eqeterms is 0 */, jdut1, 0.0, 0.0, 0.0 /* ignore polar motion */, 0);
    pos[0] = recef[0]; pos[1] = recef[1]; pos[2] = recef[2];
    Serial.println(String("ECEF ") +
        String(" R: ") + String(recef[0]) + String(" ") + String(recef[1]) + String(" ") + String(recef[2]) +
        String(" V: ") + String(vecef[0]) + String(" ") + String(vecef[1]) + String(" ") + String(vecef[2]));

    // Convert to latitude, longitude and height
    double latgc, latgd, lon, hellp;
    AstroLib::ecef2ll(recef, latgc, latgd, lon, hellp);
    Serial.println(String("Latgc: ") + String(latgc * RAD_TO_DEG) + String(" Latgd: ") + String(latgd * RAD_TO_DEG) +
        String(" Lon: ") + String(lon * RAD_TO_DEG) + String(" Hellp: ") + String(hellp));
}
