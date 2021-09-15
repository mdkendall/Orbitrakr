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
#include "hamlib_err.h"

Rotctld::Rotctld(uint16_t port, Rotator &rotator) :
    WiFiServer(port, 1),
    rotator(rotator),
    state(ROTCTLD_STATE_DISCONNECTED),
    port(port) {
        Serial.println(String("Rotctld listening on port ") + String(this->port));
}

void Rotctld::doLoop(void) {

    switch(state) {

        case ROTCTLD_STATE_DISCONNECTED:
        client = available();
        if (client) {
            Serial.println("Rotctld client connected");
            currentLine = "";
            state = ROTCTLD_STATE_CONNECTED;
        }
        break;

        case ROTCTLD_STATE_CONNECTED:
        if (client.connected()) {
            if (client.available()) {
                int c = client.read();
                if (c > 0) {
                    if (c == '\n') {
                        handleCommand(currentLine);
                        currentLine = "";
                    } else if (c != '\r') {
                        currentLine += (char)c;
                    }
                }
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

void Rotctld::handleCommand(String request) {

    char s[32];
    float az, el;

    Serial.println(String("Rotctld received ") + request);

    if (strcmp(request.c_str(), "p") == 0) {
        snprintf(s, sizeof(s), "%.2f\r\n%.2f\r\n", rotator.azAxis.getPosition(), rotator.elAxis.getPosition());
        client.write(s);
    } else if (sscanf(request.c_str(), "P %f %f", &az, &el) == 2) {
        rotator.azAxis.setTarget(az);
        rotator.elAxis.setTarget(el);
        reportError(RIG_OK);
    } else {
        reportError(RIG_EPROTO);
    }
}

void Rotctld::reportError(int error) {
    client.print(String("RPRT ") + String(-error) + String("\r\n"));
}
