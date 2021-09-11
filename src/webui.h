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

    float getAzStepsPerRev() { return atof(m_azStepsPerRev); }
    float getElStepsPerRev() { return atof(m_elStepsPerRev); }
    float getAzSpeedMax() { return atof(m_azSpeedMax); }
    float getElSpeedMax() { return atof(m_elSpeedMax); }
    float getAzAccelMax() { return atof(m_azAccelMax); }
    float getElAccelMax() { return atof(m_elAccelMax); }

   private:
    IotWebConf m_iotWebConf;
    WebServer *m_webServer;

    char m_azStepsPerRev[NUMBER_LEN] = "";
    char m_elStepsPerRev[NUMBER_LEN] = "";
    char m_azSpeedMax[NUMBER_LEN] = "";
    char m_elSpeedMax[NUMBER_LEN] = "";
    char m_azAccelMax[NUMBER_LEN] = "";
    char m_elAccelMax[NUMBER_LEN] = "";

    iotwebconf::ParameterGroup m_groupRotator = iotwebconf::ParameterGroup("Rotator", "Rotator");
    iotwebconf::NumberParameter m_paramAzStepsPerRev = iotwebconf::NumberParameter("Azimuth motor resolution in steps per revolution", "azStepsPerRev", m_azStepsPerRev, NUMBER_LEN, "64");
    iotwebconf::NumberParameter m_paramElStepsPerRev = iotwebconf::NumberParameter("Elevation motor resolution in steps per revolution", "elStepsPerRev", m_elStepsPerRev, NUMBER_LEN, "64");
    iotwebconf::NumberParameter m_paramAzSpeedMax = iotwebconf::NumberParameter("Azimuth motor max speed in steps per second", "azSpeedMax", m_azSpeedMax, NUMBER_LEN, "200");
    iotwebconf::NumberParameter m_paramElSpeedMax = iotwebconf::NumberParameter("Elevation motor max speed in steps per second", "elSpeedMax", m_elSpeedMax, NUMBER_LEN, "200");
    iotwebconf::NumberParameter m_paramAzAccelMax = iotwebconf::NumberParameter("Azimuth motor acceleration in steps per second per second", "azAccelMax", m_azAccelMax, NUMBER_LEN, "400");
    iotwebconf::NumberParameter m_paramElAccelMax = iotwebconf::NumberParameter("Elevation motor acceleration in steps per second per second", "elAccelMax", m_elAccelMax, NUMBER_LEN, "400");
    void handleRoot(void);
};

#endif
