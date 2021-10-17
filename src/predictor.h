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

#ifndef PREDICTOR_H
#define PREDICTOR_H

#include <stdint.h>
#include <Arduino.h>

#include "SGP4.h"

class Predictor {
  public:
    Predictor(void);
    bool init(uint32_t catNum);
    void site(double slatgd, double slon);
    void propagate(time_t t);
    void position(double& latgc, double& latgd, double& lon, double& hellp);
    void look(double& rho, double& az, double& el);

  private:
    // Satellite
    uint32_t catalogNumber;
    String satName;
    String tle[2];      // NORAD two-line element set
    elsetrec satrec;    // SGP4 satellite record
    time_t epoch;       // TLE epoch as UNIX timestamp
    // Prediction
    double tsince;      // time since epoch in minutes
    double recef[3];    // satellite position in ECEF reference frame
    double vecef[3];    // satellite velocity in ECEF reference frame
    double aecef[3];    // satellite acceleration in ECEF reference frame
    // Site
    double slatgd;      // site geodetic latitude in radians
    double slon;        // site longitude in radians
    double rsecef[3];   // site position in ECEF reference frame
    double vsecef[3];   // site velocity in ECEF reference frame
};

#endif
