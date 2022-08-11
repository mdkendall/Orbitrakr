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
#include "webui_html.h"

#include <ArduinoJson.h>
#include <uri/UriBraces.h>

static const char thingName[] = "Orbitrakr";
static const char wifiInitialApPassword[] = "password";

class CustomHtmlFormatProvider : public iotwebconf::HtmlFormatProvider {
   protected:
   // Add logo to the top of each page
    String getBodyInner() override {
        return
            HtmlFormatProvider::getBodyInner() +
            String("<div><img display='inherit' src='data:image/png;base64,") +
            String(logoBase64) + String("'/></div><br />\n");
    }
    // Nicer padding on the config page.
    // Small inputs and buttons on the dashboard.
    String getStyle() override {
        String s = HtmlFormatProvider::getStyle();
        s.replace("margin: 0px", "margin-bottom: 20px");
        s.replace("</style>", "div.dash {display: flex; justify-content: space-between;} .dash-req input {padding:2px;width:50px;} .dash-req button {line-height:1.5rem;font-size:1rem;width:40px;}</style>");
        return s;
    }
};

CustomHtmlFormatProvider customHtmlFormatProvider;

WebUI::WebUI(DNSServer &dnsServer, WebServer &webServer, std::function<void()> wifiConnectionCb) :
    m_iotWebConf(thingName, &dnsServer, &webServer, wifiInitialApPassword),
    m_webServer(&webServer) {

    Serial.println("Web UI initialising.");
    groupRotator.addItem(&paramAzMotorType);
    groupRotator.addItem(&paramAzMotorPins);
    groupRotator.addItem(&paramAzStepsPerRev);
    groupRotator.addItem(&paramAzSpeedMax);
    groupRotator.addItem(&paramAzAccelMax);
    groupRotator.addItem(&paramElMotorType);
    groupRotator.addItem(&paramElMotorPins);
    groupRotator.addItem(&paramElStepsPerRev);
    groupRotator.addItem(&paramElSpeedMax);
    groupRotator.addItem(&paramElAccelMax);
    m_iotWebConf.addParameterGroup(&groupRotator);
    groupTracker.addItem(&paramCatalogNumber);
    groupTracker.addItem(&paramSiteLat);
    groupTracker.addItem(&paramSiteLon);
    m_iotWebConf.addParameterGroup(&groupTracker);
    groupTinygs.addItem(&paramTinygsUsername);
    groupTinygs.addItem(&paramTinygsPassword);
    groupTinygs.addItem(&paramTinygsStation);
    m_iotWebConf.addParameterGroup(&groupTinygs);
    groupDisplay.addItem(&paramDisplayPins);
    m_iotWebConf.addParameterGroup(&groupDisplay);

    m_iotWebConf.setHtmlFormatProvider(&customHtmlFormatProvider);
    m_iotWebConf.setWifiConnectionCallback(wifiConnectionCb);
    m_iotWebConf.setupUpdateServer(
        [this](const char* updatePath) { m_httpUpdater.setup(m_webServer, updatePath); },
        [this](const char* userName, char* password) { m_httpUpdater.updateCredentials(userName, password); });
    m_iotWebConf.init();

    // -- Set up required URL handlers on the web server.
    m_webServer->on("/", [this] { handleRoot(); });
    m_webServer->on("/dashboard", [this] { handleDashboard(); });
    m_webServer->on("/api", HTTP_GET, [this] { handleApiGet(); });
    m_webServer->on(UriBraces("/api/{}/items/{}"), HTTP_PUT, [this] { handleApiPut(); });
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

    m_webServer->send(200, "text/html; charset=UTF-8", s);
}

void WebUI::handleDashboard(void) {

    // -- Let IotWebConf test and handle captive portal requests.
    if (m_iotWebConf.handleCaptivePortal()) { return; }

    String s = m_iotWebConf.getHtmlFormatProvider()->getHead();
    s += m_iotWebConf.getHtmlFormatProvider()->getStyle();
    s += "<script src=\"https://cdn.jsdelivr.net/npm/vue@2.6.14/dist/vue.js\"></script>";
    s += m_iotWebConf.getHtmlFormatProvider()->getHeadEnd();
    s += dashboardHtml;
    s += dashboardScript;
    s += m_iotWebConf.getHtmlFormatProvider()->getEnd();
    s.replace("{v}", thingName);

    m_webServer->send(200, "text/html; charset=UTF-8", s);
}

void WebUI::handleApiGet(void) {

    String s;
    DynamicJsonDocument doc(1536);
    for (auto &itemGroup : itemGroups) {
        doc[itemGroup.id]["label"] = itemGroup.label;
        for (auto &item : itemGroup.items) {
            doc[itemGroup.id]["items"][item.id]["label"] = item.label;
            doc[itemGroup.id]["items"][item.id]["value"] = item.getValue();
            doc[itemGroup.id]["items"][item.id]["units"] = item.units;
            doc[itemGroup.id]["items"][item.id]["dp"] = item.dp;
            doc[itemGroup.id]["items"][item.id]["requestable"] = item.requestable;
        }
    }
    serializeJson(doc, s);

    m_webServer->sendHeader("Access-Control-Allow-Origin", "*");
    m_webServer->send(200, "application/json; charset=UTF-8", s);
}

void WebUI::handleApiPut(void) {

    String itemGroupId = m_webServer->pathArg(0);
    String itemId = m_webServer->pathArg(1);
    String postBody = m_webServer->arg("plain");
    DynamicJsonDocument doc(512);
    DeserializationError error = deserializeJson(doc, postBody);

    if (!error) {
        for (auto &itemGroup : itemGroups) {
            if (itemGroupId == itemGroup.id) {
                for (auto &item : itemGroup.items) {
                    if (itemId == item.id) {
                        if (item.requestable) {
                            float value = doc["value"].as<float>();
                            item.setReqValue(value);
                            break;
                        }
                    }
                }
            }
        }
    }
    m_webServer->send(200, "text/plain", "OK");
}

uint8_t* WebUI::parsePins(const char *pinList, uint8_t *pins) {
    sscanf(pinList, "%hhd ,%hhd ,%hhd ,%hhd", &pins[0], &pins[1], &pins[2], &pins[3]);
    return pins;
}

WebUIItemGroup& WebUI::addItemGroup(const char *id, const char *label) {

    WebUIItemGroup itemGroup(id, label);
    this->itemGroups.push_back(itemGroup);
    return this->itemGroups.back();
}

WebUIItemGroup::WebUIItemGroup(const char *id, const char *label) :
    id(id), label(label) {
};

WebUIItem& WebUIItemGroup::addItem(const char *id, const char *label, const char *units, int dp, bool requestable) {

    WebUIItem item(id, label, units, dp, requestable);
    this->items.push_back(item);
    return this->items.back();
}

WebUIItem::WebUIItem(const char *id, const char *label, const char *units, int dp, bool requestable) :
    id(id), label(label), units(units), dp(dp), requestable(requestable) {
    this->value = 0.;
    this->reqValue = 0.;
    this->req = false;
}

void WebUIItem::setValue(float value) {
    this->value = value;
}

float WebUIItem::getValue(void) {
    return this->value;
}

void WebUIItem::setReqValue(float value) {
    this->reqValue = value;
    this->req = true;
}

float WebUIItem::getReqValue(void) {
    this->req = false;
    return this->reqValue;
}
