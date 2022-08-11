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

#ifndef DISPLAY_H
#define DISPLAY_H

#include <Arduino.h>

#include "webui.h"

/* The SPI and Wire headers are not strictly needed here, but we include them
 * to work around a problem with PlatformIO's dependency finder. If they are
 * not referenced somewhere in the project, U8g2lib will fail to compile. */
#include <SPI.h>
#include <Wire.h>
#include <U8g2lib.h>

class Display {
   public:
    Display(WebUI &webUI);

   private:
    static void task(void *param);
    void showSplash(void);
    void showDashboard(void);
    void pxFromAzEl(float az, float el, uint8_t &x, uint8_t &y);

    WebUIItem *itemRotAz, *itemRotEl, *itemTrkAz, *itemTrkEl;

    WebUI& webUI;
    TaskHandle_t taskHandle;
    U8G2_SSD1306_128X64_NONAME_F_HW_I2C *dsp;
};

#endif
