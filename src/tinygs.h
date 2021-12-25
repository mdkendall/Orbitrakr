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

#ifndef TINYGS_H
#define TINYGS_H

#include <Arduino.h>
#include <PubSubClient.h>
#include <WiFiClientSecure.h>

#include "webui.h"
#include "tracker.h"

typedef enum TinygsState_e {
    TINYGS_STATE_STOPPED,
    TINYGS_STATE_STARTED
} TinygsState_t;

class Tinygs {
   public:
    Tinygs(WebUI &webUI, Tracker &tracker);
    void restart(void);
    void handleMessage(char* topic, uint8_t* payload, unsigned int len);

   private:
    static void task(void *param);

    WebUI& webUI;
    WebUIItem *itemCatalogNumber;
    Tracker& tracker;
    TinygsState_t state;
    WiFiClientSecure espClient;
    PubSubClient* pubsubclient;
    TaskHandle_t taskHandle;
};

#endif
