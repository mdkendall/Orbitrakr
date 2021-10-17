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
#include <Webserver.h>
#include <time.h>

#include "webui.h"
#include "rotator.h"
#include "rotctld.h"
#include "predictor.h"
#include "thingpings.h"

DNSServer *dnsServer;   // DNS server for the config WiFi access point
WebServer *webServer;   // webserver for the config web interface
WebUI *webUI;           // config web interface
Rotator *rotator;       // rotator motor controller
Rotctld *rotctld;       // rotcrld-compatible network interface
Predictor *predictor;   // satellite predictor

void onWifiConnected(void) {

    configTime(0, 0, "0.pool.ntp.org", "1.pool.ntp.org", "2.pool.ntp.org");
    Thingpings::ping("Orbitrakr", webUI->getThingName());
    rotctld->restart();
    predictor->init(46494);
}

void setup(void) {
    Serial.begin(115200);
    Serial.println();
    Serial.println("Starting up...");

    rotator = new Rotator;
    rotctld = new Rotctld(4533, *rotator);
    dnsServer = new DNSServer;
    webServer = new WebServer(80);
    webUI = new WebUI(*dnsServer, *webServer, *rotator, onWifiConnected);
    predictor = new Predictor();

    // FIXME
    rotator->azAxis.home();
    rotator->elAxis.home();
}

void loop(void) {
    static unsigned long t, next = 0;
    webUI->doLoop();
    rotctld->doLoop();

    // FIXME testing
    if ((t = millis()) > next) {
        next = t + 1000;
        double latgc, latgd, lon, hellp;
        double rho, az, el;
        predictor->propagate(time(nullptr));
        predictor->position(latgc, latgd, lon, hellp);
        predictor->look(49.315 * DEG_TO_RAD, -123.075 * DEG_TO_RAD, rho, az, el);
    }
}
