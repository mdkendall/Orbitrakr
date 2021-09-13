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

#include "rotctld.h"

Rotctld::Rotctld(uint16_t port, Rotator &rotator) :
    WiFiServer(port, 1),
    rotator(rotator),
    state(ROTCTLD_STATE_DISCONNECTED),
    port(port) {
        Serial.println(String("Rotctld listening on port ") + String(this->port));
}

void Rotctld::doLoop(void) {

    static WiFiClient client;

    switch(state) {

        case ROTCTLD_STATE_DISCONNECTED:
        client = available();
        if (client) {
            Serial.println("Rotctld client connected");
            state = ROTCTLD_STATE_CONNECTED;
        }
        break;

        case ROTCTLD_STATE_CONNECTED:
        if (client.connected()) {
            if (client.available()) {
                char c = client.read();
                Serial.write(c);
            }
        } else {
            Serial.println("Rotctld client disconnected");
            state = ROTCTLD_STATE_DISCONNECTED;
        }
        break;
    }
}

void Rotctld::restart(void) {
    end();
    state = ROTCTLD_STATE_DISCONNECTED;
    Serial.println("Rotctld restarted");
    begin(port, 1);
}
