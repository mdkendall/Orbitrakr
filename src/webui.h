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

#define STRING_LEN 128
#define NUMBER_LEN 32

class WebUI {
   public:
    WebUI(DNSServer &dnsServer, WebServer &webServer);
    void doLoop(void);

    float getAzStepsPerRev() { return atof(azStepsPerRev); }
    float getElStepsPerRev() { return atof(elStepsPerRev); }
    float getAzSpeedMax() { return atof(azSpeedMax); }
    float getElSpeedMax() { return atof(elSpeedMax); }
    float getAzAccelMax() { return atof(azAccelMax); }
    float getElAccelMax() { return atof(elAccelMax); }

   private:
    IotWebConf m_iotWebConf;
    WebServer *m_webServer;

    char azStepsPerRev[NUMBER_LEN] = "";
    char elStepsPerRev[NUMBER_LEN] = "";
    char azSpeedMax[NUMBER_LEN] = "";
    char elSpeedMax[NUMBER_LEN] = "";
    char azAccelMax[NUMBER_LEN] = "";
    char elAccelMax[NUMBER_LEN] = "";

    iotwebconf::ParameterGroup groupRotator = iotwebconf::ParameterGroup("Rotator", "Rotator");
    iotwebconf::NumberParameter paramAzStepsPerRev = iotwebconf::NumberParameter("Azimuth motor resolution in steps per revolution", "azStepsPerRev", azStepsPerRev, NUMBER_LEN, "64");
    iotwebconf::NumberParameter paramElStepsPerRev = iotwebconf::NumberParameter("Elevation motor resolution in steps per revolution", "elStepsPerRev", elStepsPerRev, NUMBER_LEN, "64");
    iotwebconf::NumberParameter paramAzSpeedMax = iotwebconf::NumberParameter("Azimuth motor max speed in steps per second", "azSpeedMax", azSpeedMax, NUMBER_LEN, "200");
    iotwebconf::NumberParameter paramElSpeedMax = iotwebconf::NumberParameter("Elevation motor max speed in steps per second", "elSpeedMax", elSpeedMax, NUMBER_LEN, "200");
    iotwebconf::NumberParameter paramAzAccelMax = iotwebconf::NumberParameter("Azimuth motor acceleration in steps per second per second", "azAccelMax", azAccelMax, NUMBER_LEN, "400");
    iotwebconf::NumberParameter paramElAccelMax = iotwebconf::NumberParameter("Elevation motor acceleration in steps per second per second", "elAccelMax", elAccelMax, NUMBER_LEN, "400");
    void handleRoot(void);
};

#endif
