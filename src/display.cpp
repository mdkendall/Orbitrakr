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

#include "display.h"

Display::Display(WebUI &webUI) :
    webUI(webUI) {

    uint8_t pins[4] = {0};
    webUI.getDisplayPins(pins);
    dsp = new U8G2_SSD1306_128X64_NONAME_F_HW_I2C(U8G2_R0, U8X8_PIN_NONE, pins[0], pins[1]);
    xTaskCreatePinnedToCore(task, "Display", 4096, this, 2, &taskHandle, 1);
}

/** @brief  Main task of the Display.
 */
void Display::task(void *param) {
    Display *display = (Display*)param;

    /* Show splash screen and wait for other module to complete initialisation */
    display->dsp->begin();
    display->showSplash();
    vTaskDelay(3000);

    /* Look up the items we are going to want to display */
    display->itemRotAz = display->webUI.findItem("rotator", "az");
    display->itemRotEl = display->webUI.findItem("rotator", "el");
    display->itemTrkAz = display->webUI.findItem("tracker", "az");
    display->itemTrkEl = display->webUI.findItem("tracker", "el");

    while (true) {
        display->showDashboard();
        vTaskDelay(250);
    }
}

/** @brief  Render a splash screen
 */
void Display::showSplash(void) {

    dsp->clearBuffer();
    dsp->setFont(u8g2_font_helvR12_tr);
    dsp->drawStr(0, 20, "Orbitrakr");
    dsp->sendBuffer();
}

#define POLAR_CENTER_X  96
#define POLAR_CENTER_Y  32
#define POLAR_RADIUS    30

/** @brief  Render a dashboard
 */
void Display::showDashboard(void) {

    uint8_t x, y;
    char s[32];

    dsp->clearBuffer();
    dsp->drawCircle(POLAR_CENTER_X, POLAR_CENTER_Y, POLAR_RADIUS, U8G2_DRAW_ALL);
    dsp->drawCircle(POLAR_CENTER_X, POLAR_CENTER_Y, POLAR_RADIUS*2/3, U8G2_DRAW_ALL);
    dsp->drawCircle(POLAR_CENTER_X, POLAR_CENTER_Y, POLAR_RADIUS*1/3, U8G2_DRAW_ALL);

    pxFromAzEl(itemRotAz->getValue(), itemRotEl->getValue(), x, y);
    dsp->drawCircle(x, y, 5);

    pxFromAzEl(itemTrkAz->getValue(), itemTrkEl->getValue(), x, y);
    dsp->drawDisc(x, y, 3);

    dsp->setFont(u8g2_font_helvR08_tr);
    snprintf(s, sizeof(s), "Rot: %5.1f %4.1f", itemRotAz->getValue(), itemRotEl->getValue());
    dsp->drawStr(0, 12, s);
    snprintf(s, sizeof(s), "Trk: %5.1f %4.1f", itemTrkAz->getValue(), itemTrkEl->getValue());
    dsp->drawStr(0, 24, s);

    dsp->sendBuffer();
}

/** @brief  Calculate cartesian pixel coordinates from azimuth and
 *          elevation for the dashboard polar plot.
 */
void Display::pxFromAzEl(float az, float el, uint8_t &x, uint8_t &y) {

    float theta = HALF_PI - (az * DEG_TO_RAD);
    float r = (HALF_PI - (el * DEG_TO_RAD)) / HALF_PI;

    x = POLAR_CENTER_X + POLAR_RADIUS * r * cosf(theta);
    y = POLAR_CENTER_Y - POLAR_RADIUS * r * sinf(theta);
}
