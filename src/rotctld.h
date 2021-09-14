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

#ifndef ROTCTLD_H
#define ROTCTLD_H

#include <WiFiServer.h>

#include "rotator.h"

typedef enum RotctldState_e {
    ROTCTLD_STATE_DISCONNECTED,
    ROTCTLD_STATE_CONNECTED
} RotctldState_t;

class Rotctld : WiFiServer {
   public:
    Rotctld(uint16_t port, Rotator &rotator);
    void doLoop(void);
    void restart(void);

   private:
    Rotator& rotator;
    RotctldState_t state;
    uint16_t port;
    WiFiClient client;
    String currentLine;
    void handleCommand(String request);
    void reportError(int err);
};

#endif
