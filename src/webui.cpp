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

#include <webui.h>

const char thingName[] = "Orbitrakr";
const char wifiInitialApPassword[] = "password";

WebUI::WebUI(DNSServer &dnsServer, WebServer &webServer) :
    m_iotWebConf(thingName, &dnsServer, &webServer, wifiInitialApPassword),
    m_webServer(&webServer) {

    Serial.println("Web UI initialising.");
    m_iotWebConf.init();

    // -- Set up required URL handlers on the web server.
    m_webServer->on("/", [this] { handleRoot(); });
    m_webServer->on("/config", [this] { m_iotWebConf.handleConfig(); });
    m_webServer->onNotFound([this] { m_iotWebConf.handleNotFound(); });

    Serial.println("Web UI ready.");
}

void WebUI::update(void) {
    m_iotWebConf.doLoop();
}

void WebUI::handleRoot(void) {
    // -- Let IotWebConf test and handle captive portal requests.
    if (m_iotWebConf.handleCaptivePortal()) { return; }

    String s = "<!DOCTYPE html><html lang=\"en\"><head><meta name=\"viewport\" content=\"width=device-width, initial-scale=1, user-scalable=no\"/>";
    s += "<title>IotWebConf 01 Minimal</title></head><body>";
    s += "Go to <a href='config'>configure page</a> to change settings.";
    s += "</body></html>\n";

    m_webServer->send(200, "text/html", s);
}
