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

#include "thingpings.h"

#include <Arduino.h>
#include <WiFi.h>
#include <HTTPClient.h>

namespace Thingpings {

    void ping(const char* vendor, const char* product) {

        HTTPClient http;
        char url[256];

        uint64_t chipid = ESP.getEfuseMac();

        snprintf(url, sizeof(url), "http://www.thingpings.com/api/ping?l=%s&v=%s&p=%s&s=%04X%08X",
            WiFi.localIP().toString().c_str(),
            vendor, product,
            (uint16_t)(chipid>>32), (uint32_t)chipid );

        http.begin(url);
        http.GET();
        http.end();
    }
}
