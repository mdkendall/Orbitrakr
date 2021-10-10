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
#include <HTTPClient.h>
#include <string.h>

Predictor::Predictor(uint32_t catalogNumber) :
    catalogNumber(catalogNumber) {
}

void Predictor::init(void) {

    HTTPClient http;
    String url;
    char tleTemp[2][82];

    // fetch the latest TLE from Celestrak
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

        // Debug
        Serial.println(String("Predictor init complete:"));
        Serial.println(String("  Sat Name: ") + satName);
        Serial.println(String("  TLE[0]: ") + tle[0]);
        Serial.println(String("  TLE[1]: ") + tle[1]);

    } else {
        Serial.println("Predictor init failed");
    }
    http.end();
}
