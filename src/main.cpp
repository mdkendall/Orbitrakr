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
#include "display.h"
#include "rotator.h"
#include "rotctld.h"
#include "tracker.h"
#include "tinygs.h"
#include "thingpings.h"

DNSServer *dnsServer;   // DNS server for the config WiFi access point
WebServer *webServer;   // webserver for the config web interface
WebUI *webUI;           // config web interface
Display *display;       // OLED display
Rotator *rotator;       // rotator motor controller
Rotctld *rotctld;       // rotcrld-compatible network interface
Tracker *tracker;       // satellite tracker
Tinygs *tinygs;         // TinyGS station follower

void onWifiConnected(void) {

    configTime(0, 0, "0.pool.ntp.org", "1.pool.ntp.org", "2.pool.ntp.org");
    Thingpings::ping("Orbitrakr", webUI->getThingName());
    rotctld->restart();
    tracker->restart();
    tinygs->restart();
}

void setup(void) {
    Serial.begin(115200);
    Serial.println();
    Serial.println("Starting up...");

    dnsServer = new DNSServer;
    webServer = new WebServer(80);
    webUI = new WebUI(*dnsServer, *webServer, onWifiConnected);
    display = new Display(*webUI);
    rotator = new Rotator(*webUI);
    rotctld = new Rotctld(4533, *rotator);
    tracker = new Tracker(*webUI, *rotator);
    tinygs = new Tinygs(*webUI, *tracker);

    // FIXME
    rotator->azAxis->home();
    rotator->elAxis->home();
}

void loop(void) {
    webUI->doLoop();
    rotctld->doLoop();
}
