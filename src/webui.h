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

#include <IotWebConf.h>
#include <IotWebConfUsing.h>

#include "rotator.h"

#define STRING_LEN 128
#define NUMBER_LEN 32

class WebUI {
   public:
    WebUI(DNSServer &dnsServer, WebServer &webServer, Rotator &rotator, std::function<void()> wifiConnectionCb);
    void doLoop(void);

    float getAzStepsPerRev() { return atof(azStepsPerRev); }
    float getElStepsPerRev() { return atof(elStepsPerRev); }
    float getAzSpeedMax() { return atof(azSpeedMax); }
    float getElSpeedMax() { return atof(elSpeedMax); }
    float getAzAccelMax() { return atof(azAccelMax); }
    float getElAccelMax() { return atof(elAccelMax); }
    float getSiteLat() { return atof(siteLat); }
    float getSiteLon() { return atof(siteLon); };
    char *getThingName() { return m_iotWebConf.getThingName(); }

   private:
    IotWebConf m_iotWebConf;
    WebServer *m_webServer;
    Rotator *m_rotator;

    char azStepsPerRev[NUMBER_LEN] = "";
    char elStepsPerRev[NUMBER_LEN] = "";
    char azSpeedMax[NUMBER_LEN] = "";
    char elSpeedMax[NUMBER_LEN] = "";
    char azAccelMax[NUMBER_LEN] = "";
    char elAccelMax[NUMBER_LEN] = "";
    char siteLat[NUMBER_LEN] = "";
    char siteLon[NUMBER_LEN] = "";

    iotwebconf::ParameterGroup groupRotator = iotwebconf::ParameterGroup("Rotator", "Rotator");
    iotwebconf::NumberParameter paramAzStepsPerRev = iotwebconf::NumberParameter("Azimuth motor resolution in steps per revolution", "azStepsPerRev", azStepsPerRev, NUMBER_LEN, "64");
    iotwebconf::NumberParameter paramElStepsPerRev = iotwebconf::NumberParameter("Elevation motor resolution in steps per revolution", "elStepsPerRev", elStepsPerRev, NUMBER_LEN, "64");
    iotwebconf::NumberParameter paramAzSpeedMax = iotwebconf::NumberParameter("Azimuth motor max speed in steps per second", "azSpeedMax", azSpeedMax, NUMBER_LEN, "200");
    iotwebconf::NumberParameter paramElSpeedMax = iotwebconf::NumberParameter("Elevation motor max speed in steps per second", "elSpeedMax", elSpeedMax, NUMBER_LEN, "200");
    iotwebconf::NumberParameter paramAzAccelMax = iotwebconf::NumberParameter("Azimuth motor acceleration in steps per second per second", "azAccelMax", azAccelMax, NUMBER_LEN, "400");
    iotwebconf::NumberParameter paramElAccelMax = iotwebconf::NumberParameter("Elevation motor acceleration in steps per second per second", "elAccelMax", elAccelMax, NUMBER_LEN, "400");
    iotwebconf::ParameterGroup groupSite = iotwebconf::ParameterGroup("Site", "Site");
    iotwebconf::NumberParameter paramSiteLat = iotwebconf::NumberParameter("Site latitude in degrees", "siteLat", siteLat, NUMBER_LEN, "0.0", nullptr, "step='0.001'");
    iotwebconf::NumberParameter paramSiteLon = iotwebconf::NumberParameter("Site longitude in degrees", "siteLon", siteLon, NUMBER_LEN, "0.0", nullptr, "step='0.001'");
    void handleRoot(void);
    void handleDashboard(void);
    void handleApi(void);
};

#endif
