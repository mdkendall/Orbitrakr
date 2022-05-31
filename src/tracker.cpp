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

#include "tracker.h"
#include "webui.h"

Tracker::Tracker(WebUI &webUI, Rotator &rotator) :
    webUI(webUI),
    rotator(rotator),
    state(TRACKER_STATE_STOPPED),
    predictor() {

    WebUIItemGroup &itemGroup = webUI.addItemGroup("tracker", "Tracker");
    itemCatalogNumber = &itemGroup.addItem("catalognumber", "Satellite", "", 0, true);
    itemLatgd = &itemGroup.addItem("latgd", "Latitude", "&deg;");
    itemLon = &itemGroup.addItem("lon", "Longitude", "&deg;");
    itemHellp = &itemGroup.addItem("hellp", "Altitude", "km", 0);
    itemAz = &itemGroup.addItem("az", "Azimuth", "&deg;");
    itemEl = &itemGroup.addItem("el", "Elevation", "&deg;");
    itemRho = &itemGroup.addItem("rho", "Range", "km", 0);

    xTaskCreatePinnedToCore(task, "Tracker", 4096, this, 2, &taskHandle, 1);
}

/** @brief  Restart operation.
 *  Intended to be called after a change in configuration or network connectivity.
 */
void Tracker::restart(void) {
    setSat(webUI.getCatalogNumber());
    predictor.site(webUI.getSiteLat() * DEG_TO_RAD, webUI.getSiteLon() * DEG_TO_RAD);
    state = TRACKER_STATE_STARTED;
}

/** @brief  Set the tracked satellite by NORAD catalog number.
 */
void Tracker::setSat(uint32_t catalogNumber) {
    itemCatalogNumber->setValue(catalogNumber);
    predictor.init(catalogNumber);
}

/** @brief  Main task of the Tracker.
 *  Propagates the tracked satellite and calculates the look angle from the
 *  site. Commands the rotator if the satellite is above the horizon.
 */
void Tracker::task(void *param) {
    Tracker *tracker = (Tracker*)param;

    while (true) {

        switch(tracker->state) {
            case TRACKER_STATE_STOPPED:
                break;
            case TRACKER_STATE_STARTED:
                double latgc, latgd, lon, hellp;
                double rho, az, el;
                tracker->predictor.propagate(time(nullptr));
                tracker->predictor.position(latgc, latgd, lon, hellp);
                tracker->predictor.look(rho, az, el);
                if (el > 0) {
                    tracker->rotator.azAxis->setTarget(az*RAD_TO_DEG);
                    tracker->rotator.elAxis->setTarget(el*RAD_TO_DEG);
                }
                tracker->itemLatgd->setValue(latgd*RAD_TO_DEG);
                tracker->itemLon->setValue(lon*RAD_TO_DEG);
                tracker->itemHellp->setValue(hellp);
                tracker->itemAz->setValue(az*RAD_TO_DEG);
                tracker->itemEl->setValue(el*RAD_TO_DEG);
                tracker->itemRho->setValue(rho);

                if (tracker->itemCatalogNumber->req) tracker->setSat(tracker->itemCatalogNumber->getReqValue());
                break;
        }
        vTaskDelay(1000);
    }
}
