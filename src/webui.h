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

#ifndef WEBUI_H
#define WEBUI_H

#include <list>
#include <IotWebConf.h>
#include <IotWebConfUsing.h>
#include <IotWebConfESP32HTTPUpdateServer.h>

#define STRING_LEN 128
#define NUMBER_LEN 32

struct WebUIItem {
  public:
    WebUIItem(const char *id, const char *label, const char *units);
    void setValue(float value);
    float getValue(void);

    const char *id;
    const char *label;
    const char *units;

  private:
    float value;
};

class WebUIItemGroup {
  public:
    WebUIItemGroup(const char *id, const char *label);
    WebUIItem &addItem(const char *id, const char *label, const char *units = "");

    const char *id;
    const char *label;
    std::list<WebUIItem> items;
};

class WebUI {
  public:
    WebUI(DNSServer &dnsServer, WebServer &webServer, std::function<void()> wifiConnectionCb);
    void doLoop(void);

    WebUIItemGroup &addItemGroup(const char *id, const char *label);

    /* rotator */
    float getAzStepsPerRev() { return atof(azStepsPerRev); }
    float getElStepsPerRev() { return atof(elStepsPerRev); }
    float getAzSpeedMax() { return atof(azSpeedMax); }
    float getElSpeedMax() { return atof(elSpeedMax); }
    float getAzAccelMax() { return atof(azAccelMax); }
    float getElAccelMax() { return atof(elAccelMax); }

    /* tracker */
    int getCatalogNumber() { return atoi(catalogNumber); };
    float getSiteLat() { return atof(siteLat); }
    float getSiteLon() { return atof(siteLon); };

    /* tinygs */
    char *getTinygsUsername() { return tinygsUsername; };
    char *getTinygsPassword() { return tinygsPassword; };
    char *getTinygsStation() { return tinygsStation; };

    /* general */
    char *getThingName() { return m_iotWebConf.getThingName(); }

  private:
    IotWebConf m_iotWebConf;
    WebServer *m_webServer;
    HTTPUpdateServer m_httpUpdater;

    std::list<WebUIItemGroup> itemGroups;

    char azStepsPerRev[NUMBER_LEN] = "";
    char elStepsPerRev[NUMBER_LEN] = "";
    char azSpeedMax[NUMBER_LEN] = "";
    char elSpeedMax[NUMBER_LEN] = "";
    char azAccelMax[NUMBER_LEN] = "";
    char elAccelMax[NUMBER_LEN] = "";
    char catalogNumber[NUMBER_LEN] = "";
    char siteLat[NUMBER_LEN] = "";
    char siteLon[NUMBER_LEN] = "";
    char tinygsUsername[STRING_LEN] = "";
    char tinygsPassword[STRING_LEN] = "";
    char tinygsStation[STRING_LEN] = "";

    iotwebconf::ParameterGroup groupRotator = iotwebconf::ParameterGroup("Rotator", "Rotator");
    iotwebconf::NumberParameter paramAzStepsPerRev = iotwebconf::NumberParameter("Azimuth motor resolution in steps per revolution", "azStepsPerRev", azStepsPerRev, NUMBER_LEN, "4075.7728");
    iotwebconf::NumberParameter paramElStepsPerRev = iotwebconf::NumberParameter("Elevation motor resolution in steps per revolution", "elStepsPerRev", elStepsPerRev, NUMBER_LEN, "4075.7728");
    iotwebconf::NumberParameter paramAzSpeedMax = iotwebconf::NumberParameter("Azimuth motor max speed in steps per second", "azSpeedMax", azSpeedMax, NUMBER_LEN, "200");
    iotwebconf::NumberParameter paramElSpeedMax = iotwebconf::NumberParameter("Elevation motor max speed in steps per second", "elSpeedMax", elSpeedMax, NUMBER_LEN, "200");
    iotwebconf::NumberParameter paramAzAccelMax = iotwebconf::NumberParameter("Azimuth motor acceleration in steps per second per second", "azAccelMax", azAccelMax, NUMBER_LEN, "50");
    iotwebconf::NumberParameter paramElAccelMax = iotwebconf::NumberParameter("Elevation motor acceleration in steps per second per second", "elAccelMax", elAccelMax, NUMBER_LEN, "50");
    iotwebconf::ParameterGroup groupTracker = iotwebconf::ParameterGroup("Tracker", "Tracker");
    iotwebconf::NumberParameter paramCatalogNumber = iotwebconf::NumberParameter("Satellite catalog number", "catalogNumber", catalogNumber, NUMBER_LEN, "0", nullptr, "step='1'");
    iotwebconf::NumberParameter paramSiteLat = iotwebconf::NumberParameter("Site latitude in degrees", "siteLat", siteLat, NUMBER_LEN, "0.0", nullptr, "step='0.001'");
    iotwebconf::NumberParameter paramSiteLon = iotwebconf::NumberParameter("Site longitude in degrees", "siteLon", siteLon, NUMBER_LEN, "0.0", nullptr, "step='0.001'");
    iotwebconf::ParameterGroup groupTinygs = iotwebconf::ParameterGroup("Tinygs", "TinyGS Station (to track the satellite your station is listening to)");
    iotwebconf::TextParameter paramTinygsUsername = iotwebconf::TextParameter("MQTT Username", "tinygsUsername", tinygsUsername, STRING_LEN);
    iotwebconf::TextParameter paramTinygsPassword = iotwebconf::TextParameter("MQTT Password", "tinygsPassword", tinygsPassword, STRING_LEN);
    iotwebconf::TextParameter paramTinygsStation = iotwebconf::TextParameter("Station Name", "tinygsStation", tinygsStation, STRING_LEN);
    void handleRoot(void);
    void handleDashboard(void);
    void handleApi(void);
};

#endif
