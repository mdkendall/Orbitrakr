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

#ifndef WEBUI_H
#define WEBUI_H

#include <IotWebConf.h>
#include <IotWebConfUsing.h>

class WebUI {
   public:
    WebUI(DNSServer &dnsServer, WebServer &webServer);
    void update(void);

   private:
    IotWebConf m_iotWebConf;
    WebServer *m_webServer;
    iotwebconf::HtmlFormatProvider m_formatProvider;
    void handleRoot(void);
};

#endif
