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

#include "webui.h"
#include "webui_logo.h"

static const char thingName[] = "Orbitrakr";
static const char wifiInitialApPassword[] = "password";

class CustomHtmlFormatProvider : public iotwebconf::HtmlFormatProvider {
   protected:
    String getBodyInner() override {
        return
            HtmlFormatProvider::getBodyInner() +
            String("<div><img display='inherit' src='data:image/png;base64,") +
            String(logoBase64) + String("'/></div><br />\n");
    }
};

CustomHtmlFormatProvider customHtmlFormatProvider;

WebUI::WebUI(DNSServer &dnsServer, WebServer &webServer) :
    m_iotWebConf(thingName, &dnsServer, &webServer, wifiInitialApPassword),
    m_webServer(&webServer) {

    Serial.println("Web UI initialising.");
    m_groupRotator.addItem(&m_paramAzStepsPerRev);
    m_groupRotator.addItem(&m_paramAzSpeedMax);
    m_groupRotator.addItem(&m_paramAzAccelMax);
    m_groupRotator.addItem(&m_paramElStepsPerRev);
    m_groupRotator.addItem(&m_paramElSpeedMax);
    m_groupRotator.addItem(&m_paramElAccelMax);
    m_iotWebConf.addParameterGroup(&m_groupRotator);
    m_iotWebConf.setHtmlFormatProvider(&customHtmlFormatProvider);
    m_iotWebConf.init();

    // -- Set up required URL handlers on the web server.
    m_webServer->on("/", [this] { handleRoot(); });
    m_webServer->on("/config", [this] { m_iotWebConf.handleConfig(); });
    m_webServer->onNotFound([this] { m_iotWebConf.handleNotFound(); });

    Serial.println("Web UI ready.");
}

void WebUI::doLoop(void) {
    m_iotWebConf.doLoop();
}

void WebUI::handleRoot(void) {

    // -- Let IotWebConf test and handle captive portal requests.
    if (m_iotWebConf.handleCaptivePortal()) { return; }

    String s = m_iotWebConf.getHtmlFormatProvider()->getHead();
    s += m_iotWebConf.getHtmlFormatProvider()->getStyle();
    s += m_iotWebConf.getHtmlFormatProvider()->getHeadEnd();
    s += "<button onclick=\"window.location.href='/dashboard';\">Dashboard</button><br /><br />";
    s += "<button onclick=\"window.location.href='/config';\">Configure</button><br /><br />";
    s += "<button onclick=\"window.location.href='/firmware';\">Update</button><br /><br />";
    s += "<button onclick=\"window.location.href='/restart';\">Restart</button><br /><br />";
    s += m_iotWebConf.getHtmlFormatProvider()->getEnd();
    s.replace("{v}", thingName);

    m_webServer->sendHeader("Content-length", String(s.length()));
    m_webServer->send(200, "text/html; charset=UTF-8", s);
}
