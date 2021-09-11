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

#include <Arduino.h>
#include <DNSServer.h>
#include <EEPROM.h>
#include <ESPmDNS.h>
#include <IotWebConf.h>
#include <Webserver.h>
#include <WiFi.h>

#include "webui.h"

DNSServer dnsServer;
WebServer webServer(80);
WebUI webUI(dnsServer, webServer);

void setup(void) {
    Serial.begin(115200);
    Serial.println();
    Serial.println("Starting up...");
}

void loop(void) {
    webUI.doLoop();
}
