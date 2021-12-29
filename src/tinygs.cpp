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

#include <time.h>

#include "tinygs.h"
#include "tinygs_certs.h"
#include "webui.h"
#include "ArduinoJson.h"

#define MQTT_RETRY_INTERVAL     10

Tinygs::Tinygs(WebUI &webUI, Tracker &tracker) :
    webUI(webUI),
    tracker(tracker),
    state(TINYGS_STATE_STOPPED) {

    espClient.setCACert(tinygsRootCA);
    espClient.setInsecure();
    pubsubclient = new PubSubClient(espClient);

    WebUIItemGroup &itemGroup = webUI.addItemGroup("tinygs", "TinyGS Station");
    itemCatalogNumber = &itemGroup.addItem("catalognumber", "Satellite", "", 0);
    itemCatalogNumber->setValue(0);

    xTaskCreatePinnedToCore(task, "Tinygs", 8192, this, 2, &taskHandle, 1);
}

/** @brief  Restart operation.
 *  Intended to be called after a change in configuration or network connectivity.
 */
void Tinygs::restart(void) {
    state = TINYGS_STATE_STARTED;
}

/** @brief  Main task of the TinyGS follower.
 *  Connects to the TinyGS MQTT broker as the configured user and subscribes
 *  to updates for the congigured station.
 */
void Tinygs::task(void *param) {

    Tinygs *tinygs = (Tinygs*)param;
    time_t now;
    time_t connectRetryTime = (time_t)0;

    while (true) {

        switch (tinygs->state) {
            case TINYGS_STATE_STOPPED:
            break;

            case TINYGS_STATE_STARTED:
            /* If not connected, and credentials supplied, try connecting */
            if (!tinygs->pubsubclient->connected() && (now = time(nullptr)) > connectRetryTime) {
                connectRetryTime = now + MQTT_RETRY_INTERVAL;
                if (strlen(tinygs->webUI.getTinygsUsername()) &&
                    strlen(tinygs->webUI.getTinygsPassword()) &&
                    strlen(tinygs->webUI.getTinygsStation())) {
                    Serial.println("TinyGS connecting...");
                    tinygs->pubsubclient->setServer("mqtt.tinygs.com", 8883);
                    tinygs->pubsubclient->setCallback([tinygs] (char* topic, uint8_t* payload, unsigned int len) {tinygs->handleMessage(topic, payload, len);});
                    if (tinygs->pubsubclient->connect("Orbitrackr", tinygs->webUI.getTinygsUsername(), tinygs->webUI.getTinygsPassword())) {
                        Serial.println("TinyGS connected");
                        char topic[128];
                        snprintf(topic, sizeof(topic), "tinygs/%s/%s/cmnd/begine", tinygs->webUI.getTinygsUsername(), tinygs->webUI.getTinygsStation());
                        tinygs->pubsubclient->subscribe(topic);
                    } else {
                        Serial.println("TinyGS connection failed");
                    }
                }
            } else {
                tinygs->pubsubclient->loop();
            }
            break;
        }

        vTaskDelay(1);
    }
}

/** @brief  Handle receipt of a message from the TinyGS MQTT broker.
 *  @param  topic   message topic (expected to be tinygs/USERNAME/STATION/cmnd/begine)
 *  @param  payload message body (expected to be JSON)
 *  @param  len     message body length
 */
void Tinygs::handleMessage(char* topic, uint8_t* payload, unsigned int len) {

    size_t size = JSON_ARRAY_SIZE(10) + 10 * JSON_OBJECT_SIZE(2) + JSON_OBJECT_SIZE(16) + JSON_ARRAY_SIZE(8) + JSON_ARRAY_SIZE(8) + 64;
    DynamicJsonDocument doc(size);
    DeserializationError error = deserializeJson(doc, payload, len);
    if (error.code() == DeserializationError::Ok) {
        uint32_t catNum = doc["NORAD"];
        Serial.println(String("TinyGS tracking NORAD ") + String(catNum));
        tracker.setSat(catNum);
        itemCatalogNumber->setValue(catNum);
    }
}
