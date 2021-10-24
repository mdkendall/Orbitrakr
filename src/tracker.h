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

#ifndef TRACKER_H
#define TRACKER_H

#include <Arduino.h>

#include "webui.h"
#include "rotator.h"
#include "predictor.h"

typedef enum TrackerState_e {
    TRACKER_STATE_STOPPED,
    TRACKER_STATE_STARTED
} TrackerState_t;

class Tracker {
   public:
    Tracker(WebUI &webUI, Rotator &rotator);
    void restart(void);

   private:
    static void task(void *param);

    WebUI& webUI;
    WebUIItem *itemLatgd;
    WebUIItem *itemLon;
    WebUIItem *itemHellp;
    WebUIItem *itemAz;
    WebUIItem *itemEl;
    WebUIItem *itemRho;
    Rotator& rotator;
    TrackerState_t state;
    Predictor predictor;
    TaskHandle_t taskHandle;
};

#endif
