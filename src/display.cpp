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

    u8g2 = new U8G2_SSD1306_128X64_NONAME_F_HW_I2C(U8G2_R0);
    xTaskCreatePinnedToCore(task, "Display", 4096, this, 2, &taskHandle, 1);
}

/** @brief  Main task of the Display.
 */
void Display::task(void *param) {
    Display *display = (Display*)param;

    display->u8g2->begin();
    display->u8g2->clearBuffer();
    display->u8g2->setFont(u8g2_font_helvR12_tr);
    display->u8g2->drawStr(0,20,"Orbitrakr");
    display->u8g2->sendBuffer();

    while (true) {

        vTaskDelay(1000);
    }
}
